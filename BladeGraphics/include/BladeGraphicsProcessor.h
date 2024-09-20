/* BladeGraphicsProcessor.h
*/

#ifndef BLADE_GRAPHICS_PROCESSOR_H 
#define BLADE_GRAPHICS_PROCESSOR_H

#include <vector>
#include <deque>

#include "DisplayDriver.h"
#include "MasterSpriteData.h"
#include "SpriteInstance.h"


class BladeGraphicsProcessor {
public:
	BladeGraphicsProcessor();
	~BladeGraphicsProcessor();

	void DisplayGraphics();
	void UpdateGraphics();


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

#endif // BLADE_GRAPHICS_PROCESSOR_H
