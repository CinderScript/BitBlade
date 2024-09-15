/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"
#include "BitBladeCommon.h"

// Define the static members
vector<ImageSource> BitBladeGame::imageSources;
vector<GameObject> BitBladeGame::gameObjects;
char BitBladeGame::nextPackedCommandTemp[gameConfig::PACKED_COMMAND_MAX_SIZE];
ConsoleLink* BitBladeGame::consoleLink;


BitBladeGame::~BitBladeGame() { }

ImageSource* BitBladeGame::LoadImageSource( const char* filename ) {

	ImageSource image( filename, imageSources.size() );
	imageSources.push_back( std::move( image ) );

	uint16_t length = image.Pack_CreateImageData( nextPackedCommandTemp );

	// pack into the ConsoleLink buffer
	consoleLink->PackInstruction(
		gfxLink::GfxCommand::CreateImageData,
		nextPackedCommandTemp,
		length );

	return &imageSources.back();
}

GameObject* BitBladeGame::CreateInstance( const ImageSource* image ) {
	return &gameObjects.emplace_back( image );
}


void BitBladeGame::update() {
	for (auto& gameObject : gameObjects) {
		gameObject.update();
	}
}

void BitBladeGame::setConsoleLink( ConsoleLink& link )
{
	BitBladeGame::consoleLink = &link;
}
