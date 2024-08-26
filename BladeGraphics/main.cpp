#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeGraphics.h"
#include "ImageLoader.h"



using std::cout;


size_t LoadImageData(BladeGraphics& bladeGraphics, const std::string& imageName) {
	// Use the ImageLoader to load the image from the specified file
	ImageData* imageData = ImageLoader::LoadImageData(imageName);

	// Send the loaded ImageData to BladeGraphics and get the index
	size_t imageIndex = bladeGraphics.AddSprite(imageData);

	// Return the index of the loaded image
	return imageIndex;
}


void DisplayImageTest(BladeGraphics& bladeGraphics)
{
	LoadImageData(bladeGraphics, "SunsetBackground.bmp");
	bladeGraphics.DisplayGraphics();
}

int main()
{
	BladeGraphics graphics;
	DisplayImageTest(graphics);

	cout << 55 << "\n";
}