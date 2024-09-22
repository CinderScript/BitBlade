/* TestGame.cpp
*/

#include "TestGame.h"
#include "GameObject.h"

#include <iostream>

TestGame::TestGame( IGfxMessageLink* link ) : BitBladeGame( link )
{

}

TestGame::~TestGame() {}

const char* TestGame::GetGameTitle()
{
	return "Test Game";
}

void TestGame::InitializeGame()
{
	backgroundImage = LoadImageSource( "background.bmp" );
	heroImage = LoadImageSource( "hero.bmp" );

	background = CreateInstance( backgroundImage );
	hero = CreateInstance( heroImage );

	QuitGame();
}
