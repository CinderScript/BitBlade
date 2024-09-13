/* ConsoleLink.cpp
 */

#include <Windows.h>
#include <chrono> // For std::chrono::microseconds
#include <future>
#include <iostream>
#include <mutex>
#include <random> // For std::random_device and std::uniform_int_distribution
#include <string>
#include <thread> // For std::this_thread::sleep_for

#include "ConsoleLink.h"
#include "BitBladeCommon.h"


using bladeLinkCommon::MESSAGE_BUFFER_LENGTH;

///  PUBLIC

ConsoleLink::ConsoleLink()
	: linkStopSignal( false ),
	isGraphicsReady( true ),
	isResolvedObjectsReceived( false ),
	currentPosition( 0 )
{
	// isGraphicsReady:
	// we want isGraphicsReady = true, because the first time

	packedInstructions = new char[MESSAGE_BUFFER_LENGTH]();

	hfinishedConsoleInstructionTransferSignal = CreateOrConnectEvent( "BitBladeConsoleUpdateSendFinish" );
	hGraphicsFinishedProcessingSignal = CreateOrConnectEvent( "BitBladeGraphicsProcessingFinished" );
	hGraphicsResolvedObjectSendFinishSignal = CreateOrConnectEvent( "GraphicsResolvedObjectsSendFinish" );
	hfinishedProcessingResolvedObjectsSignal = CreateOrConnectEvent( "ConsoleResolvedObjectsFinish" );

	if (hfinishedConsoleInstructionTransferSignal == NULL)
	{
		std::cerr << "Failed to create ThisThreadReady event: " << GetLastError() << std::endl;
		return;
	}
	if (hGraphicsFinishedProcessingSignal == NULL)
	{
		std::cerr << "Failed to create ConnectingThreadReady event: " << GetLastError() << std::endl;
		return;
	}
	if (hGraphicsResolvedObjectSendFinishSignal == NULL)
	{
		std::cerr << "Failed to create hGraphicsResolvedObjectSendFinishSignal: " << GetLastError() << std::endl;
		return;
	}
	if (hfinishedProcessingResolvedObjectsSignal == NULL)
	{
		std::cerr << "Failed to create hfinishedProcessingResolvedObjectsSignal: " << GetLastError() << std::endl;
		return;
	}

	CreateOrOpenMemoryMap( graphicsOutputFileName, hOutputBufferHandle, inputMessageBuffer );
	CreateOrOpenMemoryMap( consoleOutputFileName, hInputBufferHandle, outputMessageBuffer );

	// start simulated irq listening
	futureGraphicsReadyListener = triggerListenerGraphicsReadyGpioIrqAsync();
	futureResolvedObjectsListener = triggerListenerResolvedObjectsReceivedGpioIrqAsync();
}

ConsoleLink::~ConsoleLink()
{
	linkStopSignal = true;

	SetEvent( hGraphicsFinishedProcessingSignal ); // Unblock any waiting
	if (futureGraphicsReadyListener.valid())
		futureGraphicsReadyListener.wait(); // Ensure the task completes before destruction

	SetEvent( hGraphicsResolvedObjectSendFinishSignal ); // Unblock any waiting
	if (futureResolvedObjectsListener.valid())
		futureResolvedObjectsListener.wait(); // Ensure the task completes before destruction

	if (outputMessageBuffer != NULL)
		UnmapViewOfFile( outputMessageBuffer );
	if (inputMessageBuffer != NULL)
		UnmapViewOfFile( inputMessageBuffer );

	if (hOutputBufferHandle != NULL)
		CloseHandle( hOutputBufferHandle );
	if (hInputBufferHandle != NULL)
		CloseHandle( hInputBufferHandle );

	if (hfinishedConsoleInstructionTransferSignal != NULL)
		CloseHandle( hfinishedConsoleInstructionTransferSignal );
	if (hfinishedProcessingResolvedObjectsSignal != NULL)
		CloseHandle( hfinishedProcessingResolvedObjectsSignal );

	if (hGraphicsFinishedProcessingSignal != NULL)
		CloseHandle( hGraphicsFinishedProcessingSignal );
	if (hGraphicsResolvedObjectSendFinishSignal != NULL)
		CloseHandle( hGraphicsResolvedObjectSendFinishSignal );

	delete[] packedInstructions;
}

void ConsoleLink::PackInstruction( char functionCode, const char* data, uint16_t length )
{
	bladeLinkCommon::packGfxInstruction(
		packedInstructions, functionCode, data, length, currentPosition );
}
const char* ConsoleLink::GetReceivedResolvedObjectsInstructions()
{
	return inputMessageBuffer;
}

bool ConsoleLink::HasReceivedResolvedObjects()
{
	std::lock_guard<std::mutex> lock( mutexResolvedObjectsReceived );
	return isResolvedObjectsReceived;
}

void ConsoleLink::SendGraphicsInstructions()
{
	{
		std::lock_guard<std::mutex> lock( mutexIsGraphicsReady );
		isGraphicsReady = false;
	}

	// add EOF code
	packedInstructions[currentPosition] = +bladeLinkCommon::GfxCommand::End;

	// On spi implementation, start DMA transfer
	memcpy( outputMessageBuffer, packedInstructions, currentPosition );
	currentPosition = 0;

	// trigger simulated irq (starts a background task or thread)
	triggerConsoleTransferFinishDmaIrqAsync();
}

void ConsoleLink::SignalObjectsResolvedComplete()
{
	{
		std::lock_guard<std::mutex> lock( mutexResolvedObjectsReceived );
		isResolvedObjectsReceived = false;
	}
	gpioSignalFinishedProcessingResolvedObjects();
}

// used by BladeConsole to block until graphics is ready for the next send.
// BladeGraphics wont send finished signal until it receives the next
// instruction message, processes the message, and then receives confirmation
// that BladeConsole has resolved objects from previous Graphics message sent.
void ConsoleLink::WaitForGraphicsReadySignal() // blocking
{
	// graphics is ready after finished processing and
	// receiving finishedProcessingResolvedObjectsSignal
	while (true)
	{
		{ // Scope of the lock_guard starts here
			std::lock_guard<std::mutex> lock( mutexIsGraphicsReady );
			if (isGraphicsReady)
			{
				break; // If condition is met, breaks out of the loop
			}
			// Scope of the lock_guard ends here, and the mutex is unlocked
		}
		std::this_thread::sleep_for(
			std::chrono::microseconds( 10 ) ); // Sleep is outside the lock_guard scope but inside the while loop
	}
}
void ConsoleLink::WaitForResolvedObjectsReceived()
{
	while (true)
	{
		{ // Scope of the lock_guard starts here
			std::lock_guard<std::mutex> lock( mutexResolvedObjectsReceived );
			if (isResolvedObjectsReceived)
			{
				break; // If condition is met, breaks out of the loop
			}
			// Scope of the lock_guard ends here, and the mutex is unlocked
		}
		std::this_thread::sleep_for(
			std::chrono::microseconds( 10 ) ); // Sleep is outside the lock_guard scope but inside the while loop
	}
}

void ConsoleLink::WaitForGraphicsStartupEvent()
{
	// this event is not yet in use - reusing it for a startup signal
	triggerListenerResolvedObjectsReceivedGpioIrqAsync();
}

///  PRIVATE

HANDLE ConsoleLink::CreateOrConnectEvent( const char* eventName )
{
	// Convert const char* to std::wstring
	std::wstring wideEventName = std::wstring( eventName, eventName + strlen( eventName ) );

	HANDLE hEvent = CreateEventA( NULL, FALSE, FALSE, eventName );

	if (hEvent == NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		hEvent = OpenEventA( EVENT_ALL_ACCESS, FALSE, eventName );
	}

	if (hEvent == NULL)
	{
		std::cerr << "Failed to create or open event " << eventName << ": " << GetLastError() << std::endl;
	}
	return hEvent;
}
void ConsoleLink::CreateOrOpenMemoryMap( const LPCSTR& mapName, HANDLE& handleOut, char* bufferOut )
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
	if (bufferOut == NULL)
	{
		std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
		if (handleOut != NULL)
		{
			CloseHandle( handleOut );
			return;
		}
	}
}

/// * * * * IRQ Handlers * * * * *///

// triggered via DMA irq transfer complete
void ConsoleLink::irqHandlerOnConsoleTransferFinish()
{
	// wait between 50 and 100 microseconds before sending to simulate spi write
	// time
	gpioSignalFinishedConsoleTransfer();
}

void ConsoleLink::irqHandlerOnResolvedObjectsReceived()
{
	std::lock_guard<std::mutex> lock( mutexResolvedObjectsReceived );
	isResolvedObjectsReceived = true;
}

void ConsoleLink::irqHandlerOnGraphicsReady()
{
	std::lock_guard<std::mutex> lock( mutexIsGraphicsReady ); // Mutex is locked here
	isGraphicsReady = true;									// Access to shared variable is protected
} // lock_guard goes out of scope here and automatically releases the mutex

/// * * * * MCU IRQ TRIGGER SIMULATION * * * * *///

// irq is triggered by self dma  (originates here - Console mcu)
void ConsoleLink::triggerConsoleTransferFinishDmaIrqAsync()
{

	// start a task that waits for 400 microseconds
	auto future = std::async( std::launch::async, [this]()
		{
			std::random_device rd;  // Non-deterministic random number generator
			std::mt19937 gen( rd() ); // Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distrib( 100, 600 );

			int waitTime = distrib( gen ); // Generate a random time between 100 and 600 microseconds
			std::this_thread::sleep_for( std::chrono::microseconds( waitTime ) );

			// simulate irq triggering
			irqHandlerOnConsoleTransferFinish(); } );
}

// irq is triggered by connected gpio (originates on Graphics mcu)
// in windows version, this is blocking to avoid processing graphics objects
// received in another thread or polling somewhere else in code to check if this
// irq has been triggered.
std::future<void> ConsoleLink::triggerListenerResolvedObjectsReceivedGpioIrqAsync()
{
	// wait in another thread (non blocking)
	return std::async( std::launch::async, [this]()
		{
			while (!linkStopSignal)
			{
				DWORD waitResult = WaitForSingleObject( hGraphicsResolvedObjectSendFinishSignal, 20 );

				if (linkStopSignal)
					break;

				switch (waitResult)
				{
				case WAIT_OBJECT_0:
					std::cout << "Console has received resolved graphics objects." << std::endl;

					irqHandlerOnResolvedObjectsReceived();

					break;
				case WAIT_TIMEOUT:
					// std::cerr << "Wait timed out. Listening for console resolving objects
					// again." << std::endl;
					break;
				case WAIT_ABANDONED:
					std::cerr << "The wait was abandoned, potentially due to an error in "
						"another thread."
						<< std::endl;
					break;
				case WAIT_FAILED:
					std::cerr << "Wait failed with error: " << GetLastError() << std::endl;
					break;
				default:
					std::cerr << "Unexpected wait result." << std::endl;
					break;
				}
			} } );
}

// irq is triggered by connected gpio (originates on Graphics mcu)
std::future<void> ConsoleLink::triggerListenerGraphicsReadyGpioIrqAsync()
{
	// wait in another thread (non blocking)
	return std::async( std::launch::async, [this]()
		{
			while (!linkStopSignal)
			{
				DWORD waitResult = WaitForSingleObject( hGraphicsFinishedProcessingSignal, 20 );

				if (linkStopSignal)
					break;

				switch (waitResult)
				{
				case WAIT_OBJECT_0:
					std::cout << "Graphics has finished processing the update." << std::endl;

					irqHandlerOnGraphicsReady();

					break;
				case WAIT_TIMEOUT:
					// std::cerr << "Wait timed out. Listening for console resolving objects
					// again." << std::endl;
					break;
				case WAIT_ABANDONED:
					std::cerr << "The wait was abandoned, potentially due to an error in "
						"another thread."
						<< std::endl;
					break;
				case WAIT_FAILED:
					std::cerr << "Wait failed with error: " << GetLastError() << std::endl;
					break;
				default:
					std::cerr << "Unexpected wait result." << std::endl;
					break;
				}
			} } );
}

/// * * * * GPIO PIN SIMULATION * * * * *///

void ConsoleLink::gpioSignalFinishedProcessingResolvedObjects()
{
	SetEvent( hfinishedProcessingResolvedObjectsSignal );
}
void ConsoleLink::gpioSignalFinishedConsoleTransfer()
{
	SetEvent( hfinishedConsoleInstructionTransferSignal );
}