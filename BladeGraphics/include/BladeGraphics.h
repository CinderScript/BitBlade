/* BladeGraphics.h
*/

#ifndef BLADE_GRAPHICS_H 
#define BLADE_GRAPHICS_H

#include "GraphicsLink.h"

class BladeGraphics {
public:
	BladeGraphics();
	~BladeGraphics();

	void StartGraphics();
	void UpdateGraphics();


private:
	GraphicsLink link;
	void ProcessGraphics();
};

#endif // BLADE_GRAPHICS_H