/* TestGame.cpp
*/

#include "TestGame.h"

#include <iostream>

TestGame::TestGame()
{
	backgroundPrefab = &LoadPrefab( "background.bmp" );
	heroPrefab = &LoadPrefab( "hero.bmp" );

	background = &CreateInstance( backgroundPrefab );
	hero = &CreateInstance( heroPrefab );
}

TestGame::~TestGame() {}

void TestGame::GameStart()
{

}
