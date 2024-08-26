/* BladeGraphics.cpp
*/

#include "BladeGraphics.h"
#include "DisplayDriver.h"
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
	// Clean up any dynamically allocated sprites
	for (auto sprite : sprites) {
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
	for (auto& sprite : sprites) {
		if (sprite && sprite->GetImageData()) {
			pixelOps.DrawImage(frameBuffer, sprite->GetX(), sprite->GetY(), *(sprite->GetImageData()));
		}
	}
}

size_t BladeGraphics::AddSprite(ImageData* imageData) {
	SpriteObject* newSprite = new SpriteObject(imageData);
	sprites.push_back(newSprite);
	return sprites.size() - 1; // Return the ID of the newly added sprite
}

void BladeGraphics::RemoveSprite(int spriteId) {
	if (spriteId >= 0 && spriteId < sprites.size()) {
		delete sprites[spriteId];  // Free the memory
		sprites.erase(sprites.begin() + spriteId);
	}
}

/* ************************** TESTS ****************************** */

int BladeGraphics::DisplayBufferTest() {
	const int screenWidth = 1024;
	const int screenHeight = 600;

	// Create a frame buffer with BGRA format
	ImageData frameBuffer(screenWidth, screenHeight);

	// Define the color for the rectangle using the BGRA format
	uint16_t rectangleColor = { 255 };

	// Calculate the position and size of the rectangle to center it
	int rectWidth = 200;
	int rectHeight = 100;
	int rectX = (screenWidth - rectWidth) / 2;
	int rectY = (screenHeight - rectHeight) / 2;

	// Create a PixelOperations instance for BGRA format and draw the rectangle
	PixelOperations pixelOps;
	pixelOps.DrawRectangle(frameBuffer, rectX, rectY, rectWidth, rectHeight, rectangleColor);

	// Render the buffer on the screen using DisplayDriver
	DisplayDriver displayDriver;
	displayDriver.Render(frameBuffer); // Render the buffer on the screen

	return 123; // Return a test value
}