/* BladeLink.cpp
*/

#include <Windows.h>
#include <functional>
#include <string>
#include <iostream>

#include "BladeLink.h"


using std::string;

BladeLink::BladeLink(bool isServer)
	: fileHandle(NULL), messageBuffer(NULL)
{

	hThisThreadReady = isServer ? ConnectEvent("BitBladeGraphics98801") : ConnectEvent("BitBladeConsole98801");
	hConnectingThreadReady = isServer ? ConnectEvent("BitBladeConsole98801") : ConnectEvent("BitBladeGraphics98801");


	if (hThisThreadReady == NULL) {
		std::cerr << "Failed to create ThisThreadReady event: " << GetLastError() << std::endl;
		return;
	}
	if (hConnectingThreadReady == NULL) {
		std::cerr << "Failed to create ConnectingThreadReady event: " << GetLastError() << std::endl;
		return;
	}


	fileHandle = CreateOrOpenMemoryMap();
	if (fileHandle != NULL) {
		messageBuffer = MapView();
	}
	if (messageBuffer == NULL) {
		std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
		if (fileHandle != NULL) {
			CloseHandle(fileHandle);
		}
	}
}

BladeLink::~BladeLink() {
	if (messageBuffer != NULL) UnmapViewOfFile(messageBuffer);
	if (fileHandle != NULL) CloseHandle(fileHandle);
	if (hThisThreadReady != NULL) CloseHandle(hThisThreadReady);
	if (hConnectingThreadReady != NULL) CloseHandle(hConnectingThreadReady);
}

HANDLE BladeLink::ConnectEvent(const char* eventName) {
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, eventName);
	if (hEvent == NULL && GetLastError() == ERROR_ALREADY_EXISTS) {
		hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName);
	}

	if (hEvent == NULL) {
		std::cerr << "Failed to create or open event " << eventName << ": " << GetLastError() << std::endl;
	}
	return hEvent;
}

HANDLE BladeLink::CreateOrOpenMemoryMap() {
	// Try to open existing memory-mapped file
	HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, memoryFileName);
	if (hMap == NULL) {
		// If it doesn't exist, create a new one
		std::cout << "No existing memory map found, creating a new one." << std::endl;
		hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, static_cast<DWORD>(mapSize), memoryFileName);
		if (hMap == NULL) {
			std::cerr << "Could not create file mapping object: " << GetLastError() << std::endl;
		}
	}
	else {
		std::cout << "Opened existing memory map." << std::endl;
	}
	return hMap;
}

char* BladeLink::MapView() {
	if (fileHandle == NULL) return NULL;
	char* pMap = static_cast<char*>(MapViewOfFile(fileHandle, FILE_MAP_ALL_ACCESS, 0, 0, mapSize));
	if (pMap == NULL) {
		std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
	}
	return pMap;
}

void BladeLink::SignalThisThreadReady() {
	SetEvent(hThisThreadReady);
}

void BladeLink::WaitForConnectedThreadReady()
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

const char* BladeLink::GetBladeMessage() {
	return messageBuffer;
}

void BladeLink::SendBladeMessage(const char* message, const size_t size) {
	if (size > mapLength) {
		std::cerr << "Message size exceeds buffer capacity." << std::endl;
		return;
	}
	memcpy(messageBuffer, message, size * sizeof(uint16_t));
}
