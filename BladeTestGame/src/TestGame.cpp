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

void TestGame::Initialize()
{
	backgroundImage = LoadImageSource( "background.bmp" );
	heroImage = LoadImageSource( "hero.bmp" );

	background = CreateInstance();
	hero = CreateInstance();

	QuitGame();
}
