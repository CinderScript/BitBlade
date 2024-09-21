/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"
#include "BitBladeCommon.h"

#include "ImageSource.h"
#include "GameObject.h"

#include "BladeConsole.h"

namespace game {// Define the static members

	console::ConsoleLink* BitBladeGame::consoleLink;
	char BitBladeGame::nextPackedCommandTemp[gfxLink::PACKED_COMMAND_MAX_SIZE];
	bool BitBladeGame::isGameRunning = true;

	BitBladeGame::BitBladeGame() : imgPool( 50 ), objPool( 300 ) {

		componentPool.ReservePool<Sprite>( 200 );
	}

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

		ImageSource* image = imgPool.Add( filename );
		uint16_t length = image->Pack_CreateImageData( nextPackedCommandTemp );

		// pack into the ConsoleLink buffer
		consoleLink->PackInstruction(
			gfxLink::GfxCode::CreateImageData,
			nextPackedCommandTemp,
			length );

		return image;
	}

	GameObject* BitBladeGame::CreateInstance( const ImageSource* image ) {

		GameObject* obj = objPool.Add( this, image );

		return obj;
	}


	bool BitBladeGame::update() {
		// for (auto& gameObject : gameObjects) {
		// 	gameObject.update();
		// }

		return isGameRunning;
	}

	void BitBladeGame::setConsoleLink( console::ConsoleLink& link )
	{
		BitBladeGame::consoleLink = &link;
	}
}