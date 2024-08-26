/* BladeConsole.cpp
*
* Updates the game loop
* Keeps track of each object
* Updates objects positions and rotations
* Sets objects height visibility
*/

#include "BladeConsole.h"

#include <BladeGraphics.h>
#include "ImageLoader.h"


BladeConsole::BladeConsole() : bladeGraphics() {
}

BladeConsole::~BladeConsole() {
}

size_t BladeConsole::LoadImage(const std::string& imageName) {
	// Use the ImageLoader to load the image from the specified file
	ImageData* imageData = ImageLoader::LoadImage(imageName);

	// Send the loaded ImageData to BladeGraphics and get the index
	size_t imageIndex = bladeGraphics.AddSprite(imageData);

	// Return the index of the loaded image
	return imageIndex;
}


/* ************************** TESTS ****************************** */

void BladeConsole::DisplayImageTest()
{
	LoadImage("SunsetBackground.bmp");
	bladeGraphics.DisplayGraphics();
}

void BladeConsole::DisplayBufferTest()
{
	BladeGraphics test;
	test.DisplayBufferTest();
}

// GameUpdateLoop() { }