/* BladeGraphics.h
*/

#ifndef BLADE_GRAPHICS_H 
#define BLADE_GRAPHICS_H

#include "GraphicsLink.h"

class BladeGraphics {
public:
	BladeGraphics();
	~BladeGraphics();

	bool FirstUpdate();
	bool Update();


private:
	GraphicsLink link;
	bool ProcessGfxInstructions();
};

#endif // BLADE_GRAPHICS_H