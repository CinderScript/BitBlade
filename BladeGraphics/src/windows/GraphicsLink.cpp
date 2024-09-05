/* BladeLink.cpp
*/

#include <Windows.h>
#include <functional>
#include <string>
#include <iostream>

#include "GraphicsLink.h"
#include "GfxCommand.h"


GraphicsLink::GraphicsLink(bool isServer)
	: hOutputBufferHandle(NULL), hInputBufferHandle(NULL),
	hThisThreadReady(NULL), hConnectingThreadReady(NULL),
	inputMessageBuffer(NULL), outputMessageBuffer(NULL),
	isServer(isServer)
{
	packedInstructions = new char[GRAPHICS_BUFFER_LENGTH]();

	hThisThreadReady = isServer ? CreateOrConnectEvent("BitBladeGraphics98801") : CreateOrConnectEvent("BitBladeConsole98801");
	hConnectingThreadReady = isServer ? CreateOrConnectEvent("BitBladeConsole98801") : CreateOrConnectEvent("BitBladeGraphics98801");

	if (hThisThreadReady == NULL) {
		std::cerr << "Failed to create ThisThreadReady event: " << GetLastError() << std::endl;
		return;
	}
	if (hConnectingThreadReady == NULL) {
		std::cerr << "Failed to create ConnectingThreadReady event: " << GetLastError() << std::endl;
		return;
	}

	if (isServer) {
		CreateOrOpenMemoryMap(graphicsOutputFileName, hOutputBufferHandle, outputMessageBuffer);
		CreateOrOpenMemoryMap(consoleOutputFileName, hInputBufferHandle, inputMessageBuffer);
	}
	else {
		CreateOrOpenMemoryMap(graphicsOutputFileName, hInputBufferHandle, inputMessageBuffer);
		CreateOrOpenMemoryMap(consoleOutputFileName, hOutputBufferHandle, outputMessageBuffer);
	}
}

GraphicsLink::~GraphicsLink() {
	if (outputMessageBuffer != NULL) UnmapViewOfFile(outputMessageBuffer);
	if (hOutputBufferHandle != NULL) CloseHandle(hOutputBufferHandle);
	if (hInputBufferHandle != NULL) CloseHandle(hInputBufferHandle);
	if (hThisThreadReady != NULL) CloseHandle(hThisThreadReady);
	if (hConnectingThreadReady != NULL) CloseHandle(hConnectingThreadReady);
}

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

void GraphicsLink::SignalGraphicsUpdateSent() {
	SetEvent(hThisThreadReady);
}

void GraphicsLink::WaitForConnectedThreadFinish()
{
	DWORD waitResult = WaitForSingleObject(hConnectingThreadReady, INFINITE);

	switch (waitResult) {
	case WAIT_OBJECT_0:
		std::cout << "connectingThread is now ready." << std::endl;
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
}

const char* GraphicsLink::GetBladeMessage() {
	return inputMessageBuffer;
}

void GraphicsLink::PackInstruction(char functionCode, const char* data, size_t length) {
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

void GraphicsLink::SendBladeMessage() {

	// add EOF code
	packedInstructions[currentPosition] = +GfxCommand::End;

	// On spi implementation, start DMA transfer
	memcpy(outputMessageBuffer, packedInstructions, currentPosition);
	currentPosition = 0;
}
