/* BladeGraphics.h
*/

#ifndef BLADE_GRAPHICS_H 
#define BLADE_GRAPHICS_H

#include "GraphicsLink.h"

#include <iostream> // unique_ptr

using std::string;

class BladeGraphics {
public:
	BladeGraphics();
	~BladeGraphics();

	void StartGraphics();
	void UpdateGraphics();


private:
	std::unique_ptr<GraphicsLink> console;
	void ProcessGraphics();
};

#endif // BLADE_GRAPHICS_H