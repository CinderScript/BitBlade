
#ifdef _WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif

#include "main.h"

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
#ifdef _WIN32
#ifdef _DEBUG
	// Set debug heap flags to track memory allocations and detect leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif

	ConsoleConnector connector;
	//connector.sendMessage("LoadSprite image.bmp");
	//std::string response = connector.receiveMessage();
	//std::cout << "Response: " << response << std::endl;

	//BladeGraphics graphics;
	//DisplayImageTest(graphics);

	cout << 55 << "\n";

}