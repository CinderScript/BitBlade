/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include "IBladeLink.h"
#include <iostream>

class BladeConsole {
public:
	BladeConsole();
	~BladeConsole();

	void UpdateConsole();

private:
	std::unique_ptr<IBladeLink> bladeLink;

	void processGraphicsMessage(const char* message);
};

#endif // BLADE_CONSOLE_H
