/* TestGame.cpp
*/

#include "TestGame.h"

#include <iostream>

TestGame::TestGame()
{
	backgroundImage = LoadImageSource( "background.bmp" );
	heroImage = LoadImageSource( "hero.bmp" );

	background = CreateInstance( backgroundImage );
	hero = CreateInstance( heroImage );
}

TestGame::~TestGame() {}

void TestGame::GameStart()
{

}
