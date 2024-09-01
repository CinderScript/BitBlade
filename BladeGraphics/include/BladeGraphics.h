/* BladeGraphics.h
*/

#ifndef BLADE_GRAPHICS_H 
#define BLADE_GRAPHICS_H

#include "IBladeLink.h"

#include <iostream> // unique_ptr

using std::string;

class BladeGraphics {
public:
	BladeGraphics();
	~BladeGraphics();

	void UpdateGraphics();


private:
	std::unique_ptr<IBladeLink> bladeLink;

	void processConsoleMessage(const char* message);
	void sendGraphicsMetadata();
};

#endif // BLADE_GRAPHICS_H