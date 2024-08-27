#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeGraphics.h"
#include "ImageLoader.h"



using std::cout;


size_t LoadMasterSprite(BladeGraphics& bladeGraphics, const std::string& imageName)
{

	ImageData* imageData = ImageLoader::LoadImageData(imageName);

	size_t imageIndex = bladeGraphics.AddMasterSprite(imageData);

	return imageIndex;
}


void DisplayImageTest(BladeGraphics& bladeGraphics)
{
	size_t masterID = LoadMasterSprite(bladeGraphics, "SunsetBackground.bmp");

	SpriteInstance* sprite = bladeGraphics.AddSpriteInstance(masterID);

	bladeGraphics.DisplayGraphics();
}

int main()
{
	BladeGraphics graphics;
	DisplayImageTest(graphics);

	cout << 55 << "\n";
}