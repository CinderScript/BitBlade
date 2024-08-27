/* BladeGraphics.cpp
*/

#include "BladeGraphics.h"
#include "PixelOperations.h"
#include "ImageData.h"

BladeGraphics::BladeGraphics()
	: frameBuffer(1024, 600)
{
	uint16_t backgroundColor = 0xB5BF; // Light blue-gray color in RGB565
	for (int y = 0; y < frameBuffer.GetHeight(); ++y) {
		for (int x = 0; x < frameBuffer.GetWidth(); ++x) {
			frameBuffer.SetPixel(x, y, backgroundColor);
		}
	}
}

BladeGraphics::~BladeGraphics() {
	// Clean up any dynamically allocated spriteDataPool
	for (auto sprite : spriteDataPool) {
		delete sprite;
	}
}

void BladeGraphics::DisplayGraphics() {
	// Render the internal frame buffer on the screen using DisplayDriver
	UpdateGraphics();

	DisplayDriver displayDriver;
	displayDriver.Render(frameBuffer);
}

void BladeGraphics::UpdateGraphics() {
	// Clear the framebuffer with the background color (light blue-gray)
	uint16_t backgroundColor = 0xB5BF; // Light blue-gray color in RGB565

	for (int y = 0; y < frameBuffer.GetHeight(); ++y) {
		for (int x = 0; x < frameBuffer.GetWidth(); ++x) {
			frameBuffer.SetPixel(x, y, backgroundColor);
		}
	}

	// Draw each sprite onto the framebuffer
	PixelOperations pixelOps;
	for (auto& sprite : spriteInstancePool) {
		if (sprite && sprite->GetImageData()) {
			pixelOps.DrawImage(frameBuffer, sprite->GetX(), sprite->GetY(), *(sprite->GetImageData()));
		}
	}
}

size_t BladeGraphics::AddMasterSprite(ImageData* imageData) {
	MasterSpriteData* newSprite = new MasterSpriteData(imageData);
	spriteDataPool.push_back(newSprite);
	return spriteDataPool.size() - 1; // Return the ID of the newly added sprite
}

void BladeGraphics::RemoveMasterSprite(int spriteId) {
	if (spriteId >= 0 && spriteId < spriteDataPool.size()) {
		delete spriteDataPool[spriteId];  // Free the memory
		spriteDataPool.erase(spriteDataPool.begin() + spriteId);
	}
}
SpriteInstance* BladeGraphics::AddSpriteInstance(size_t masterSpriteID) {
	SpriteInstance* newSprite = new SpriteInstance(spriteDataPool[masterSpriteID]);
	spriteInstancePool.push_back(newSprite);
	return newSprite;
}

void BladeGraphics::RemoveSpriteInstance(int spriteInstanceID) {
	if (spriteInstanceID >= 0 && spriteInstanceID < spriteInstancePool.size()) {
		delete spriteInstancePool[spriteInstanceID];  // Free the memory
		spriteInstancePool.erase(spriteInstancePool.begin() + spriteInstanceID);
	}
}