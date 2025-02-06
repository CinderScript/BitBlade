/* TestGame.cpp
*/

#include "TestGame.h"
#include "GameObject.h"
#include "Transform.h"
#include "TestComponents.h"

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
	// backgroundImage = LoadImageSource( "background.bmp" );
	heroImage = LoadImageSource( "hero.bmp" );
	arm = Instantiate( hero, "arm" );
	sword = Instantiate( "Sword" );
	sword->SetParent( arm );

	hero = Instantiate( "Hero" );
	heroTransform = hero->GetTransform();
	armTransform = arm->GetTransform();
	swordTransform = sword->GetTransform();

	std::cout << "Test Game - Initialize." << "\n";
}

void TestGame::GlobalUpdate() {
	std::cout << "\nTest Game - Update:" << updateCount << "\n";

	if (updateCount == 0) {
		heroTransform->SetLocalPosition( 0, 1.0 );
		heroTransform->SetLocalRotation( 90.0 );
	}

	if (updateCount == 1) {
		heroTransform->SetPosition( 0, 1.0 );
		heroTransform->SetRotation( 180.0 );
	}

	if (updateCount == 2) {
		QuitGame();
	}

	// std::cout << "\nHero local position = " << heroTransform->LocalPosition() << "\n";
	std::cout << "\nHero local rotation = " << heroTransform->LocalRotation() << "\n";
	updateCount++;
}

// void TestGame::Initialize()
// {
// 	// backgroundImage = LoadImageSource( "background.bmp" );
// 	heroImage = LoadImageSource( "hero.bmp" );

// 	// background = Instantiate( "Background" );
// 	// tree = Instantiate( background, "tree" );
// 	hero = Instantiate( "Hero" );
// 	arm = Instantiate( hero, "arm" );
// 	sword = Instantiate( "Sword" );
// 	sword->SetParent( arm );

// 	armTransform = arm->GetTransform();
// 	swordTransform = sword->GetTransform();

// 	hero->AddComponent<ObjectUpdatePrinter>();
// 	arm->AddComponent<ObjectUpdatePrinter>();
// 	sword->AddComponent<ObjectUpdatePrinter>();

// 	std::cout << "Test Game - Initialize." << "\n";
// }

// void TestGame::GlobalUpdate() {
// 	std::cout << "\nTest Game - Update:" << updateCount << "\n";

// 	if (updateCount == 0) {
// 		arm->AddComponent<TransformChangedPrinter>();

// 		armTransform->SetPosition( 3.0, 3.0 );
// 	}

// 	if (updateCount > 0) {
// 		QuitGame();
// 	}

// 	updateCount++;
// }
