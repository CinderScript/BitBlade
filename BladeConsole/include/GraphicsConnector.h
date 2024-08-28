/* GraphicsConnector.h
*/

#ifndef GRAPHICS_CONNECTOR_H 
#define GRAPHICS_CONNECTOR_H

#include <Windows.h>
#include <iostream>
#include <string>

using std::string;

class GraphicsConnector {
public:
	GraphicsConnector();
	~GraphicsConnector();

	void sendMessage(const std::string& message);
	string receiveMessage();

private:
	HANDLE hPipe;
	LPCWSTR pipeName = L"\\\\.\\pipe\\BitBladePipe";
};

#endif // GRAPHICS_CONNECTOR_H
