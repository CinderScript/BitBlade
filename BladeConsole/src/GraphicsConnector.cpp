/* GraphicsConnector.cpp
*/

#include "GraphicsConnector.h"

GraphicsConnector::GraphicsConnector()
{
	// Create a named pipe to simulate SPI communication
	hPipe = CreateNamedPipeW(
		pipeName,             // pipe name
		PIPE_ACCESS_DUPLEX,   // read/write access
		PIPE_TYPE_BYTE |      // message type pipe
		PIPE_READMODE_BYTE |  // message-read mode
		PIPE_WAIT,            // blocking mode
		1,                    // max. instances
		1024,                 // output buffer size
		1024,                 // input buffer size
		5,                    // client time-out
		NULL);                // default security attribute

	if (hPipe == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to create named pipe." << std::endl;
		return;
	}

	// Wait for the client to connect
	if (!ConnectNamedPipe(hPipe, NULL)) {
		CloseHandle(hPipe);
		std::cerr << "Failed to connect named pipe." << std::endl;
		return;
	}
}

GraphicsConnector::~GraphicsConnector()
{
	CloseHandle(hPipe);
}

void GraphicsConnector::sendMessage(const std::string& message)
{
	DWORD written;
	WriteFile(hPipe, message.c_str(), message.size(), &written, NULL);
}

string GraphicsConnector::receiveMessage()
{
	char buffer[1024];
	DWORD bytesRead;
	ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL);
	return std::string(buffer, bytesRead);
}


