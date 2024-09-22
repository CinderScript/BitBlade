/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"


namespace game {// Define the static members

	BitBladeGame::BitBladeGame( IGfxMessageLink* link )
		: IGfxMessageProducer( link ),
		imgPool( 50 ),
		objPool( 300 ) {

		componentPool.ReservePool<Sprite>( 200 );
	}

	BitBladeGame::~BitBladeGame() { }

	void BitBladeGame::QuitGame()
	{
		isGameRunning = false;

		strcpy( nextPackedCommandTemp, GetGameTitle() );
		uint16_t length = strlen( GetGameTitle() ) + 1;

		// let the BladeGraphics know we are stopping
		gfxMessageHandler->PackInstruction(
			gfxLink::GfxCode::StopGraphics,
			nextPackedCommandTemp,
			length );
	}

	ImageSource* BitBladeGame::LoadImageSource( const char* filename ) {

		ImageSource* image = imgPool.Add( filename );
		uint16_t length = image->Pack_CreateImageData( nextPackedCommandTemp );

		// pack into the ConsoleLink buffer
		gfxMessageHandler->PackInstruction(
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
}