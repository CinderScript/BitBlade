/* TestGame.cpp
*/

#include "TestGame.h"

#include <iostream>

TestGame::TestGame() {}

TestGame::~TestGame() {}

void TestGame::GameAwake()
{
	backgroundPrefab = LoadPrefab( "background.bmp" );
	heroPrefab = LoadPrefab( "hero.bmp" );
}

void TestGame::GameStart()
{
	background = CreateInstance( backgroundPrefab );
	hero = CreateInstance( heroPrefab );
}