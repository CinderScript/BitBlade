/* BladeGraphics.h
*/

#ifndef BLADE_GRAPHICS_H 
#define BLADE_GRAPHICS_H

#include <vector>
#include "SpriteObject.h"

class BladeGraphics {
public:
	BladeGraphics();
	~BladeGraphics();

	void DisplayGraphics();
	void UpdateGraphics();

	int DisplayBufferTest();

	// Methods for managing sprites
	size_t AddSprite(ImageData* imageData);
	void RemoveSprite(int spriteId);

private:
	ImageData frameBuffer;
	std::vector<SpriteObject*> sprites;
};

#endif // BLADE_GRAPHICS_H
