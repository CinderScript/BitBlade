/* TestGame.cpp
*/

#include "TestGame.h"
#include "GameObject.h"
#include "ObjectUpdatePrinter.h"

#include <iostream>

TestGame::TestGame( IGfxMessagePacker* link ) : BitBladeGame( link )
{
	std::cout << "Test Game - Constructor." << "\n";
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

	background = Instantiate( "Background" );
	tree = Instantiate( background, "tree" );
	hero = Instantiate( "Hero" );
	arm = Instantiate( hero, "arm" );
	sword = Instantiate( "Sword" );
	sword->SetParent( arm );

	background->AddComponent<ObjectUpdatePrinter>();
	tree->AddComponent<ObjectUpdatePrinter>();

	hero->AddComponent<ObjectUpdatePrinter>();
	arm->AddComponent<ObjectUpdatePrinter>();
	sword->AddComponent<ObjectUpdatePrinter>();

	std::cout << "Test Game - Initialize." << "\n";
}

void TestGame::Update() {
	std::cout << "\nTest Game - Update:" << updateCount << "\n";


	if (updateCount > 2) {

		QuitGame();
	}

	updateCount++;

}
