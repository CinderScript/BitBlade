/* ConsoleConnector.h
*/

#ifndef CONSOLE_CONNECTOR_H 
#define CONSOLE_CONNECTOR_H

#include <Windows.h>
#include <iostream>
#include <string>

using std::string;

class ConsoleConnector {
public:
	ConsoleConnector();
	~ConsoleConnector();

	void sendMessage(const std::string& message);
	string receiveMessage();

private:
	HANDLE hPipe;
	LPCWSTR pipeName = L"\\\\.\\pipe\\BitBladePipe";
};

#endif // CONSOLE_CONNECTOR_H
