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
#include "GfxCommand.h"


GraphicsLink::GraphicsLink()
	: isInstructionsReceived(false), isConsoleObjectResolveComplete(false), graphicsLinkStopSignal(false), currentPosition(0)
{
	packedInstructions = new char[GRAPHICS_BUFFER_LENGTH]();

	hfinishedConsoleInstructionTransferSignal = CreateOrConnectEvent("BitBladeConsoleUpdateSendFinish");
	hGraphicsFinishedProcessingSignal = CreateOrConnectEvent("BitBladeGraphicsProcessingFinished");
	hGraphicsResolvedObjectSendFinishSignal = CreateOrConnectEvent("GraphicsResolvedObjectsSendFinish");
	hfinishedConsoleResolvedObjectsFinishSignal = CreateOrConnectEvent("ConsoleResolvedObjectsFinish");

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

	CreateOrOpenMemoryMap(graphicsOutputFileName, hOutputBufferHandle, outputMessageBuffer);
	CreateOrOpenMemoryMap(consoleOutputFileName, hInputBufferHandle, inputMessageBuffer);

	// start simulated irq listening
	triggerListenerInstructionsReceivedGpioIrqAsync();
	triggerListenerConsoleResolvedObjectsGpioIrqAsync();
}

GraphicsLink::~GraphicsLink()
{
	graphicsLinkStopSignal = true;

	SetEvent(hfinishedConsoleInstructionTransferSignal); // Unblock any waiting
	if (futureInstructionsReceivedListener.valid())
		futureInstructionsReceivedListener.wait(); // Ensure the task completes before destruction

	SetEvent(hfinishedConsoleResolvedObjectsFinishSignal); // Unblock any waiting
	if (futureConsoleResolvedObjectsListener.valid())
		futureConsoleResolvedObjectsListener.wait(); // Ensure the task completes before destruction

	if (outputMessageBuffer != NULL) UnmapViewOfFile(outputMessageBuffer);
	if (inputMessageBuffer != NULL) UnmapViewOfFile(inputMessageBuffer);

	if (hOutputBufferHandle != NULL) CloseHandle(hOutputBufferHandle);
	if (hInputBufferHandle != NULL) CloseHandle(hInputBufferHandle);

	if (hfinishedConsoleInstructionTransferSignal != NULL) CloseHandle(hfinishedConsoleInstructionTransferSignal);
	if (hfinishedConsoleResolvedObjectsFinishSignal != NULL) CloseHandle(hfinishedConsoleResolvedObjectsFinishSignal);

	if (hGraphicsFinishedProcessingSignal != NULL) CloseHandle(hGraphicsFinishedProcessingSignal);
	if (hGraphicsResolvedObjectSendFinishSignal != NULL) CloseHandle(hGraphicsResolvedObjectSendFinishSignal);

	delete[] packedInstructions;
}

void GraphicsLink::PackInstruction(char functionCode, const char* data, size_t length)
{
	if (currentPosition + length + 2 > GRAPHICS_BUFFER_LENGTH) { // Check buffer overflow (+1 for function code, +1 for EOF code)
		std::cerr << "Buffer overflow prevented." << std::endl;
		return;
	}

	// Write the function code to the buffer
	packedInstructions[currentPosition++] = functionCode;

	// Write the data to the buffer
	memcpy(packedInstructions + currentPosition, data, length);
	currentPosition += length;
}

const char* GraphicsLink::GetGraphicsInstructions()
{
	return inputMessageBuffer;
}

void GraphicsLink::SendResolvedGraphicsObjects() {

	// add EOF code
	packedInstructions[currentPosition] = +GfxCommand::End;

	// On spi implementation, start DMA transfer
	memcpy(outputMessageBuffer, packedInstructions, currentPosition);
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
		if (isInstructionsReceived) {
			break;
		}
	}

	isInstructionsReceived = false;
}

void GraphicsLink::AwaitConsoleFinishedResolvingObjectsSignal()
{
	while (true) {
		if (isConsoleObjectResolveComplete) {
			break;
		}
	}

	isConsoleObjectResolveComplete = false;
}

void GraphicsLink::SignalGraphicsFinishedProcessing()
{
	gpioSignalFinishedProcessingGraphics();
}


///  PRIVATE

HANDLE GraphicsLink::CreateOrConnectEvent(const char* eventName) {
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, eventName);
	if (hEvent == NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
		hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName);
	}

	if (hEvent == NULL) {
		std::cerr << "Failed to create or open event " << eventName << ": " << GetLastError() << std::endl;
	}
	return hEvent;
}
void GraphicsLink::CreateOrOpenMemoryMap(const LPCSTR& mapName, HANDLE& handleOut, char* bufferOut) {
	// Try to open existing memory-mapped file
	handleOut = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, mapName);
	if (handleOut == NULL) {
		// If it doesn't exist, create a new one
		std::cout << "No existing memory map found, creating a new one." << std::endl;
		handleOut = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, static_cast<DWORD>(GRAPHICS_BUFFER_LENGTH), mapName);
		if (handleOut == NULL) {
			std::cerr << "Could not create file mapping object: " << GetLastError() << std::endl;
		}
	}
	else {
		std::cout << "Opened existing memory map:" << mapName << std::endl;
	}

	if (handleOut != NULL) {
		bufferOut = static_cast<char*>(MapViewOfFile(handleOut, FILE_MAP_ALL_ACCESS, 0, 0, GRAPHICS_BUFFER_LENGTH));
		if (bufferOut == NULL) {
			std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
		}
	}
	if (bufferOut == NULL) {
		std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
		if (handleOut != NULL) {
			CloseHandle(handleOut);
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
	isInstructionsReceived = true;
}

void GraphicsLink::irqHandlerOnConsoleResolvedFinishedSignal()
{
	isConsoleObjectResolveComplete = true;
}

/// * * * * GPIO PIN SIMULATION * * * * *///

void GraphicsLink::gpioSignalFinishedProcessingGraphics()
{
	SetEvent(hGraphicsFinishedProcessingSignal);
}
void GraphicsLink::gpioSignalFinishedResolvedObjectsTransfer()
{
	SetEvent(hGraphicsResolvedObjectSendFinishSignal);
}


/// * * * * MCU IRQ TRIGGER SIMULATION: WAIT IN ANOTHER THREAD (NON BLOCKING) * * * * *///

// irq is triggered by self dma  (originates here - Console mcu)
// triggers irq handler after random time period
void GraphicsLink::triggerResolvedObjectsTransferFinishDmaIrqAsync() {

	// start a task that waits for 400 microseconds
	auto future = std::async(std::launch::async, [this]() {
		std::random_device rd;  // Non-deterministic random number generator
		std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib(100, 600);

		int waitTime = distrib(gen);  // Generate a random time between 100 and 600 microseconds
		std::this_thread::sleep_for(std::chrono::microseconds(waitTime));

		// simulate irq triggering
		irqHandlerOnObjectsTransferFinish();
		});
}

void GraphicsLink::triggerListenerInstructionsReceivedGpioIrqAsync()
{
	// wait in another thread (non blocking)
	futureInstructionsReceivedListener = std::async(std::launch::async, [this]() {

		while (!graphicsLinkStopSignal)
		{
			DWORD waitResult = WaitForSingleObject(hfinishedConsoleInstructionTransferSignal, 20);

			if (graphicsLinkStopSignal)
				break;

			switch (waitResult) {
			case WAIT_OBJECT_0:
				std::cout << "Graphics received instructions from console." << std::endl;
				irqHandlerOnInstructionsReceivedSignal();
				break;
			case WAIT_ABANDONED:
				std::cerr << "The wait was abandoned, potentially due to an error in another thread." << std::endl;
				break;
			case WAIT_FAILED:
				std::cerr << "Wait failed with error: " << GetLastError() << std::endl;
				break;
			case WAIT_TIMEOUT:
				std::cerr << "Wait timed out. Listening for graphics instructions again." << std::endl;
			default:
				std::cerr << "Unexpected wait result." << std::endl;
				break;
			}
		}
		});
}

void GraphicsLink::triggerListenerConsoleResolvedObjectsGpioIrqAsync()
{
	// wait in another thread (non blocking)
	futureConsoleResolvedObjectsListener = std::async(std::launch::async, [this]() {

		while (!graphicsLinkStopSignal) {
			DWORD waitResult = WaitForSingleObject(hfinishedConsoleResolvedObjectsFinishSignal, 20);

			if (graphicsLinkStopSignal)
				break;

			switch (waitResult) {
			case WAIT_OBJECT_0:
				std::cout << "Console has resolved graphics objects." << std::endl;
				irqHandlerOnConsoleResolvedFinishedSignal();
				break;
			case WAIT_TIMEOUT:
				std::cerr << "Wait timed out. Listening for console resolving objects again." << std::endl;
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
		});
}
