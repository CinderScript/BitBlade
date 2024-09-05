/* ConsoleLink.cpp
*/

#include <Windows.h>
#include <functional>
#include <string>
#include <iostream>
#include <future>
#include <chrono>  // For std::chrono::microseconds
#include <random>  // For std::random_device and std::uniform_int_distribution
#include <thread>  // For std::this_thread::sleep_for
#include <mutex>


#include "ConsoleLink.h"
#include "GfxCommand.h"


///  PUBLIC

ConsoleLink::ConsoleLink()
	: isGraphicsReady(true), currentPosition(0)
{
	// we want isGraphicsReady = true, because the first time 
	packedInstructions = new char[GRAPHICS_BUFFER_LENGTH]();

	hfinishedConsoleInstructionTransferSignal = CreateOrConnectEvent("BitBladeConsoleUpdateSendFinish");
	hGraphicsFinishedProcessingSignal = CreateOrConnectEvent("BitBladeGraphicsProcessingFinished");
	hGraphicsResolvedObjectSendFinishSignal = CreateOrConnectEvent("GraphicsResolvedObjectsSendFinish");
	hfinishedProcessingResolvedObjectsSignal = CreateOrConnectEvent("ConsoleResolvedObjectsFinish");

	if (hfinishedConsoleInstructionTransferSignal == NULL) {
		std::cerr << "Failed to create ThisThreadReady event: " << GetLastError() << std::endl;
		return;
	}
	if (hGraphicsFinishedProcessingSignal == NULL) {
		std::cerr << "Failed to create ConnectingThreadReady event: " << GetLastError() << std::endl;
		return;
	}

	CreateOrOpenMemoryMap(graphicsOutputFileName, hOutputBufferHandle, outputMessageBuffer);
	CreateOrOpenMemoryMap(consoleOutputFileName, hInputBufferHandle, inputMessageBuffer);
}

ConsoleLink::~ConsoleLink() {
	if (outputMessageBuffer != NULL) UnmapViewOfFile(outputMessageBuffer);
	if (hOutputBufferHandle != NULL) CloseHandle(hOutputBufferHandle);
	if (hInputBufferHandle != NULL) CloseHandle(hInputBufferHandle);
	if (hfinishedConsoleInstructionTransferSignal != NULL) CloseHandle(hfinishedConsoleInstructionTransferSignal);
	if (hGraphicsFinishedProcessingSignal != NULL) CloseHandle(hGraphicsFinishedProcessingSignal);

	delete[] packedInstructions;
}

void ConsoleLink::PackInstruction(char functionCode, const char* data, size_t length) {
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
void ConsoleLink::SendGraphicsInstructions() {

	// add EOF code
	packedInstructions[currentPosition] = +GfxCommand::End;

	// On spi implementation, start DMA transfer
	memcpy(outputMessageBuffer, packedInstructions, currentPosition);
	currentPosition = 0;

	// trigger simulated irq (starts a background task or thread)
	triggerConsoleTransferFinishDmaIrqAsync();
}
// used by BladeConsole to block until graphics is ready for the next send.  BladeGraphics
// wont send finished signal until it receives the next instruction message, processes the message,
// and then receives confirmation that BladeConsole has resolved objects from previous Graphics 
// message sent.
void ConsoleLink::WaitForGraphicsReadySignal() // blocking
{
	// start listening for resolved objects sent
	// will start listening 
	listenForResolvedObjectsReceivedGpioIrqBlocking();

	// start listening for received graphics message
	listenForGraphicsReadyGpioIrqAsync();

	// graphics is ready after finished processing and 
	// receiving finishedProcessingResolvedObjectsSignal
	while (true) {
		// Beginning of the inner scope
		std::lock_guard<std::mutex> lock(mtx); // Mutex is locked each time the block is entered
		if (isGraphicsReady) {
			isGraphicsReady = false;           // Protected access and modification of shared variable
			break;  // Exits the loop and scope, releasing the mutex
		} // lock_guard is destroyed here, and mutex is released
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
}

template<typename T>
void ConsoleLink::SetOnResolvedObjectsReceivedHandler(T* instance, OnMessageReceivedEvent<T> messageReceivedHandler) {
	handleResolvedObjectsInstance = (void*)instance;
	handleResolvedObjectsMethod = reinterpret_cast<void (*)(void*, const char*)>(messageReceivedHandler);
}

///  PRIVATE

HANDLE ConsoleLink::CreateOrConnectEvent(const char* eventName) {
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, eventName);
	if (hEvent == NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
		hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName);
	}

	if (hEvent == NULL) {
		std::cerr << "Failed to create or open event " << eventName << ": " << GetLastError() << std::endl;
	}
	return hEvent;
}
void ConsoleLink::CreateOrOpenMemoryMap(const LPCSTR& mapName, HANDLE& handleOut, char* bufferOut) {
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
void ConsoleLink::irqHandlerOnConsoleTransferFinish() {
	// wait between 50 and 100 microseconds before sending to simulate spi write time
	gpioSignalFinishedConsoleTransferSignal();
}

void ConsoleLink::irqHandlerOnResolvedObjectsReceived() {
	(*handleResolvedObjectsMethod)(handleResolvedObjectsInstance, inputMessageBuffer); // invoke callback
	gpioSignalFinishedProcessingResolvedObjects();
}

void ConsoleLink::irqHandlerOnGraphicsReady() {
	std::lock_guard<std::mutex> lock(mtx); // Mutex is locked here
	isGraphicsReady = true;               // Access to shared variable is protected
} // lock_guard goes out of scope here and automatically releases the mutex

/// * * * * MCU IRQ SIMULATION: WAIT IN ANOTHER THREAD (NON BLOCKING) * * * * *///

// irq is triggered by self dma  (originates here - Console mcu)
void ConsoleLink::triggerConsoleTransferFinishDmaIrqAsync() {

	// start a task that waits for 400 microseconds
	auto future = std::async(std::launch::async, [this]() {
		std::random_device rd;  // Non-deterministic random number generator
		std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distrib(100, 600);

		int waitTime = distrib(gen);  // Generate a random time between 100 and 600 microseconds
		std::this_thread::sleep_for(std::chrono::microseconds(waitTime));

		// simulate irq triggering
		irqHandlerOnConsoleTransferFinish();
		});
}

// irq is triggered by connected gpio (originates on Graphics mcu)
void ConsoleLink::listenForResolvedObjectsReceivedGpioIrqBlocking()
{
	DWORD waitResult = WaitForSingleObject(hGraphicsResolvedObjectSendFinishSignal, INFINITE);

	switch (waitResult) {
	case WAIT_OBJECT_0:
		std::cout << "Console has received resolved graphics objects." << std::endl;
		// Proceed with reading file contents
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

	irqHandlerOnResolvedObjectsReceived();
}

// irq is triggered by connected gpio (originates on Graphics mcu)
void ConsoleLink::listenForGraphicsReadyGpioIrqAsync() // blocking
{
	// wait in another thread (non blocking)
	auto future = std::async(std::launch::async, [this]() {

		DWORD waitResult = WaitForSingleObject(hGraphicsFinishedProcessingSignal, INFINITE);

		switch (waitResult) {
		case WAIT_OBJECT_0:
			std::cout << "Graphics has finished processing the update." << std::endl;
			// Proceed with reading file contents
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

		irqHandlerOnGraphicsReady();
		});

}

/// * * * * GPIO PIN SIMULATION * * * * *///

void ConsoleLink::gpioSignalFinishedProcessingResolvedObjects()
{
	SetEvent(hfinishedProcessingResolvedObjectsSignal);
}
void ConsoleLink::gpioSignalFinishedConsoleTransferSignal()
{
	SetEvent(hfinishedConsoleInstructionTransferSignal);
}