
#ifdef _WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif

#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeGraphics.h"
#include "BladeGraphicsProcessor.h"
#include "ImageLoader.h"

using std::cout;


size_t LoadMasterSprite(BladeGraphicsProcessor& bladeGraphics, const std::string& imageName)
{

	ImageData* imageData = ImageLoader::LoadImageData(imageName);

	size_t imageIndex = bladeGraphics.AddMasterSprite(imageData);

	return imageIndex;
}


void DisplayImageTest(BladeGraphicsProcessor& bladeGraphics)
{
	size_t masterID = LoadMasterSprite(bladeGraphics, "SunsetBackground.bmp");

	SpriteInstance* sprite = bladeGraphics.AddSpriteInstance(masterID);

	bladeGraphics.DisplayGraphics();
}

int main()
{
#ifdef _WIN32
#ifdef _DEBUG
	// Set debug heap flags to track memory allocations and detect leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif

	BladeGraphics graphics;

	while (true) {
		graphics.UpdateGraphics();
	}

	cout << "\n" << "Graphics Done" << "\n";

}