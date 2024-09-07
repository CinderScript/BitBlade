/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include "ConsoleLink.h"

#include <iostream>

class BladeConsole {
public:
	BladeConsole();
	~BladeConsole();

	void StartConsole();
	void UpdateConsole();

private:
	ConsoleLink* console;
	void resolvedObjectsReceivedHandler(const char* message);
};

#endif // BLADE_CONSOLE_H
