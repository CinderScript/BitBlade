/* BladeGraphics.h
*/

#ifndef BLADE_GRAPHICS_H 
#define BLADE_GRAPHICS_H

#include <vector>
#include <deque>

#include "DisplayDriver.h"
#include "MasterSpriteData.h"
#include "SpriteInstance.h"


class BladeGraphics {
public:
	BladeGraphics();
	~BladeGraphics();

	void DisplayGraphics();
	void UpdateGraphics();

	int DisplayBufferTest();

	// Methods for managing Sprites
	size_t AddMasterSprite(ImageData* imageData);
	void RemoveMasterSprite(int spriteId);

	SpriteInstance* AddSpriteInstance(size_t masterSpriteID);
	void RemoveSpriteInstance(int spriteInstanceID);

private:
	ImageData frameBuffer;
	std::vector<MasterSpriteData*> spriteDataPool;	// master copies of sprites - owns image data
	std::deque<SpriteInstance*> spriteInstancePool;	// sprites to be displayed to screen
};

#endif // BLADE_GRAPHICS_H
