/* BladeGraphics.h
*/

#ifndef BLADE_GRAPHICS_H 
#define BLADE_GRAPHICS_H

#include "IGraphicsLink.h"

#include <iostream> // unique_ptr

using std::string;

class BladeGraphics {
public:
	BladeGraphics();
	~BladeGraphics();

	void StartGraphics();
	void UpdateGraphics();


private:
	std::unique_ptr<GraphicsLink> bladeLink;

	void processConsoleMessage();
	void sendGraphicsMetadata();
};

#endif // BLADE_GRAPHICS_H