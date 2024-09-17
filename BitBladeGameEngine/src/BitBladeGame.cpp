/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"
#include "BitBladeCommon.h"

namespace game {// Define the static members
	vector<ImageSource> BitBladeGame::imageSources;
	vector<GameObject> BitBladeGame::gameObjects;
	ConsoleLink* BitBladeGame::consoleLink;
	char BitBladeGame::nextPackedCommandTemp[gfxLink::PACKED_COMMAND_MAX_SIZE];
	bool BitBladeGame::isGameRunning = true;


	BitBladeGame::~BitBladeGame() { }

	void BitBladeGame::QuitGame()
	{
		BitBladeGame::isGameRunning = false;

		strcpy( nextPackedCommandTemp, GetGameTitle() );
		uint16_t length = strlen( GetGameTitle() ) + 1;

		// let the BladeGraphics know we are stopping
		consoleLink->PackInstruction(
			gfxLink::GfxCode::StopGraphics,
			nextPackedCommandTemp,
			length );
	}

	ImageSource* BitBladeGame::LoadImageSource( const char* filename ) {

		ImageSource image( filename, imageSources.size() );
		imageSources.push_back( std::move( image ) );

		uint16_t length = image.Pack_CreateImageData( nextPackedCommandTemp );

		// pack into the ConsoleLink buffer
		consoleLink->PackInstruction(
			gfxLink::GfxCode::CreateImageData,
			nextPackedCommandTemp,
			length );

		return &imageSources.back();
	}

	GameObject* BitBladeGame::CreateInstance( const ImageSource* image ) {
		return &gameObjects.emplace_back( image );
	}


	bool BitBladeGame::update() {
		for (auto& gameObject : gameObjects) {
			gameObject.update();
		}

		return isGameRunning;
	}

	void BitBladeGame::setConsoleLink( ConsoleLink& link )
	{
		BitBladeGame::consoleLink = &link;
	}
}