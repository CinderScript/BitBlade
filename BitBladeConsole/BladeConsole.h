/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include <string>
#include "BladeGraphics/include/BladeGraphics.h"
#include "BladeGraphics/include/ImageLoader.h"

class BladeConsole {
public:
	BladeConsole();
	~BladeConsole();

	size_t LoadImage(const std::string& imageName);

	void DisplayBufferTest();
	void DisplayImageTest();

private:
	BladeGraphics bladeGraphics;
};

#endif // BLADE_CONSOLE_H
