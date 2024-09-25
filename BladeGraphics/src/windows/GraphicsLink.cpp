/* BladeLink.cpp
*/

#include <Windows.h>
#include <string>
#include <iostream>
#include <future>
#include <chrono>  // For std::chrono::microseconds
#include <random>  // For std::random_device and std::uniform_int_distribution
#include <thread>  // For std::this_thread::sleep_for
#include <mutex>

#include "GraphicsLink.h"

using gfxLink::GfxCode;
using gfxLinkConfig::MESSAGE_BUFFER_LENGTH;

GraphicsLink::GraphicsLink()
	: isInstructionsReceived( false ),
	isConsoleObjectResolveComplete( false ),
	linkStopSignal( false ),
	currentPosition( 0 )
{
	packedInstructions = new char[MESSAGE_BUFFER_LENGTH]();

	hfinishedConsoleInstructionTransferSignal = CreateOrConnectEvent( "BitBladeConsoleUpdateSendFinish" );
	hGraphicsFinishedProcessingSignal = CreateOrConnectEvent( "BitBladeGraphicsProcessingFinished" );
	hGraphicsResolvedObjectSendFinishSignal = CreateOrConnectEvent( "GraphicsResolvedObjectsSendFinish" );
	hfinishedConsoleResolvedObjectsFinishSignal = CreateOrConnectEvent( "ConsoleResolvedObjectsFinish" );

	if (hfinishedConsoleInstructionTransferSignal == NULL) {
		std::cerr << "Failed to create hfinishedConsoleInstructionTransferSignal: " << GetLastError() << std::endl;
		return;
	}
	if (hGraphicsFinishedProcessingSignal == NULL) {
		std::cerr << "Failed to create hGraphicsFinishedProcessingSignal: " << GetLastError() << std::endl;
		return;
	}
	if (hGraphicsResolvedObjectSendFinishSignal == NULL) {
		std::cerr << "Failed to create hGraphicsResolvedObjectSendFinishSignal: " << GetLastError() << std::endl;
		return;
	}
	if (hfinishedConsoleResolvedObjectsFinishSignal == NULL) {
		std::cerr << "Failed to create hfinishedProcessingResolvedObjectsSignal: " << GetLastError() << std::endl;
		return;
	}

	CreateOrOpenMemoryMap( graphicsOutputFileName, hGraphicsOutputBuffer, graphicsOutputBuffer );
	CreateOrOpenMemoryMap( consoleOutputFileName, hConsoleOutputBuffer, consoleOutputBuffer );

	// start simulated irq listening
	futureInstructionsReceivedListener = triggerListenerInstructionsReceivedGpioIrqAsync();
	futureConsoleObjectsResolvedListener = triggerListenerConsoleResolvedObjectsGpioIrqAsync();
}

GraphicsLink::~GraphicsLink()
{
	linkStopSignal = true;

	SetEvent( hfinishedConsoleInstructionTransferSignal ); // Unblock any waiting
	if (futureInstructionsReceivedListener.valid())
		futureInstructionsReceivedListener.wait(); // Ensure the task completes before destruction

	SetEvent( hfinishedConsoleResolvedObjectsFinishSignal ); // Unblock any waiting
	if (futureConsoleObjectsResolvedListener.valid())
		futureConsoleObjectsResolvedListener.wait(); // Ensure the task completes before destruction

	if (graphicsOutputBuffer != NULL) UnmapViewOfFile( graphicsOutputBuffer );
	if (consoleOutputBuffer != NULL) UnmapViewOfFile( consoleOutputBuffer );

	if (hGraphicsOutputBuffer != NULL) CloseHandle( hGraphicsOutputBuffer );
	if (hConsoleOutputBuffer != NULL) CloseHandle( hConsoleOutputBuffer );

	if (hfinishedConsoleInstructionTransferSignal != NULL) CloseHandle( hfinishedConsoleInstructionTransferSignal );
	if (hfinishedConsoleResolvedObjectsFinishSignal != NULL) CloseHandle( hfinishedConsoleResolvedObjectsFinishSignal );

	if (hGraphicsFinishedProcessingSignal != NULL) CloseHandle( hGraphicsFinishedProcessingSignal );
	if (hGraphicsResolvedObjectSendFinishSignal != NULL) CloseHandle( hGraphicsResolvedObjectSendFinishSignal );

	delete[] packedInstructions;
}

void GraphicsLink::AddPackedInstruction( gfxLink::GfxCode functionCode, const char* data, uint16_t length )
{
	gfxLink::packGfxInstruction(
		packedInstructions, functionCode, data, length, currentPosition );
}

const char* GraphicsLink::GetGraphicsInstructions()
{
	return consoleOutputBuffer;
}

void GraphicsLink::SendResolvedGraphicsObjects() {

	// add EOF code
	packedInstructions[currentPosition] = +GfxCode::EndMessage;

	// On spi implementation, start DMA transfer
	memcpy( graphicsOutputBuffer, packedInstructions, currentPosition );
	currentPosition = 0;

	// trigger simulated irq (starts a background task or thread)
	triggerResolvedObjectsTransferFinishDmaIrqAsync();
}

void GraphicsLink::SendGraphicsStartupEvent()
{
	gpioSignalFinishedResolvedObjectsTransfer();
}

void GraphicsLink::AwaitConsoleInstructionsReceivedSignal() // blocking
{
	while (true) {
		{
			std::lock_guard<std::mutex> lock( mtxInstructionsReceived );
			if (isInstructionsReceived) {
				isInstructionsReceived = false;
				break;
			}
		}
	}

}

void GraphicsLink::AwaitConsoleFinishedResolvingObjectsSignal()
{
	while (true) {
		{
			std::lock_guard<std::mutex> lock( mtxConsoleObjectResolve );
			if (isConsoleObjectResolveComplete) {
				isConsoleObjectResolveComplete = false;
				break;
			}
		}
	}
}

void GraphicsLink::SignalGraphicsFinishedProcessing()
{
	gpioSignalFinishedProcessingGraphics();
}


///  PRIVATE

HANDLE GraphicsLink::CreateOrConnectEvent( const char* eventName ) {
	HANDLE hEvent = CreateEventA( NULL, FALSE, FALSE, eventName );
	if (hEvent == NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
		hEvent = OpenEventA( EVENT_ALL_ACCESS, FALSE, eventName );
	}

	if (hEvent == NULL) {
		std::cerr << "Failed to create or open event " << eventName << ": " << GetLastError() << std::endl;
	}
	return hEvent;
}
void GraphicsLink::CreateOrOpenMemoryMap( const LPCSTR& mapName, HANDLE& handleOut, char*& bufferOut )
{
	// Try to open existing memory-mapped file
	handleOut = OpenFileMappingA( FILE_MAP_ALL_ACCESS, FALSE, mapName );
	if (handleOut == NULL)
	{
		// If it doesn't exist, create a new one
		std::cout << "No existing memory map found, creating a new one." << std::endl;
		handleOut = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
			static_cast<DWORD>(MESSAGE_BUFFER_LENGTH), mapName );
		if (handleOut == NULL)
		{
			std::cerr << "Could not create file mapping object: " << GetLastError() << std::endl;
		}
	}
	else
	{
		std::cout << "Opened existing memory map:" << mapName << std::endl;
	}

	if (handleOut != NULL)
	{
		bufferOut = static_cast<char*>(MapViewOfFile( handleOut, FILE_MAP_ALL_ACCESS, 0, 0, MESSAGE_BUFFER_LENGTH ));

		if (bufferOut == NULL)
		{
			std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
		}
	}
	else
	{
		std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
		if (handleOut != NULL)
		{
			CloseHandle( handleOut );
			handleOut = NULL;  // Prevent using an invalid handle
			return;
		}
	}
}


/// * * * * IRQ Handlers * * * * *///

// triggered via DMA irq transfer complete
void GraphicsLink::irqHandlerOnObjectsTransferFinish() {
	gpioSignalFinishedResolvedObjectsTransfer();
}

void GraphicsLink::irqHandlerOnInstructionsReceivedSignal()
{
	std::lock_guard<std::mutex> lock( mtxInstructionsReceived );
	isInstructionsReceived = true;
}

void GraphicsLink::irqHandlerOnConsoleResolvedFinishedSignal()
{
	std::lock_guard<std::mutex> lock( mtxConsoleObjectResolve );
	isConsoleObjectResolveComplete = true;
}


/// * * * * GPIO PIN SIMULATION * * * * *///

void GraphicsLink::gpioSignalFinishedProcessingGraphics()
{
	SetEvent( hGraphicsFinishedProcessingSignal );
}
void GraphicsLink::gpioSignalFinishedResolvedObjectsTransfer()
{
	SetEvent( hGraphicsResolvedObjectSendFinishSignal );
}


/// * * * * MCU IRQ TRIGGER SIMULATION: WAIT IN ANOTHER THREAD (NON BLOCKING) * * * * *///

// irq is triggered by self dma  (originates here - Console mcu)
// triggers irq handler after random time period
void GraphicsLink::triggerResolvedObjectsTransferFinishDmaIrqAsync() {

	// start a task that waits for 400 microseconds
	auto future = std::async( std::launch::async, [this]() {
		std::random_device rd;  // Non-deterministic random number generator
		std::mt19937 gen( rd() ); // Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib( 100, 600 );

		int waitTime = distrib( gen );  // Generate a random time between 100 and 600 microseconds
		std::this_thread::sleep_for( std::chrono::microseconds( waitTime ) );

		// simulate irq triggering
		irqHandlerOnObjectsTransferFinish();
		} );
}

std::future<void> GraphicsLink::triggerListenerInstructionsReceivedGpioIrqAsync()
{
	// wait in another thread (non blocking)
	return std::async( std::launch::async, [this]() {

		while (!linkStopSignal)
		{
			DWORD waitResult = WaitForSingleObject( hfinishedConsoleInstructionTransferSignal, 20 );

			if (linkStopSignal)
				break;

			switch (waitResult) {
			case WAIT_OBJECT_0:
				std::cout << "Graphics received instructions from console." << std::endl;

				irqHandlerOnInstructionsReceivedSignal();

				break;
			case WAIT_TIMEOUT:
				//std::cerr << "Wait timed out. Listening for graphics instructions again." << std::endl;
				break;
			case WAIT_ABANDONED:
				std::cerr << "The wait was abandoned, potentially due to an error in another thread." << std::endl;
				break;
			case WAIT_FAILED:
				std::cerr << "Wait failed with error: " << GetLastError() << std::endl;
				break;
			default:
				std::cerr << "Unexpected wait result." << std::endl;
				break;
			}
		}
		} );
}

std::future<void> GraphicsLink::triggerListenerConsoleResolvedObjectsGpioIrqAsync()
{
	// wait in another thread (non blocking)
	return std::async( std::launch::async, [this]() {

		while (!linkStopSignal) {
			DWORD waitResult = WaitForSingleObject( hfinishedConsoleResolvedObjectsFinishSignal, 20 );

			if (linkStopSignal)
				break;

			switch (waitResult) {
			case WAIT_OBJECT_0:
				std::cout << "Console has resolved graphics objects." << std::endl;

				irqHandlerOnConsoleResolvedFinishedSignal();

				break;
			case WAIT_TIMEOUT:
				//std::cerr << "Wait timed out. Listening for console resolving objects again." << std::endl;
				break;
			case WAIT_ABANDONED:
				std::cerr << "The wait was abandoned, potentially due to an error in another thread." << std::endl;
				break;
			case WAIT_FAILED:
				std::cerr << "Wait failed with error: " << GetLastError() << std::endl;
				break;
			default:
				std::cerr << "Unexpected wait result." << std::endl;
				break;
			}
		}
		} );
}
