#ifndef BIT_BLADE_GAMES_TEST_H
#define BIT_BLADE_GAMES_TEST_H


#include "BitBladeGame.h"
#include "GameObject.h"
#include "ImageSource.h"
#include "GameComponents_test.h"

using std::cout;
using std::endl;

/* ------------------------------- GAME ALPHA ------------------------------- */

class GameAlpha : public game::BitBladeGame
{
public:

	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~GameAlpha() {}

	const char* GetGameTitle() override;
	void Initialize() override;
	void Update() override;

	size_t totalUpdates = 3;
	size_t updateCount = 0;

};

const char* GameAlpha::GetGameTitle() {
	return "TestGame-Alpha\n";
}

void GameAlpha::Initialize()
{

}

void GameAlpha::Update()
{
	updateCount++;

	if (updateCount >= totalUpdates) {

		QuitGame();
	}

}

/* -------------------------------- GAME ZULU ------------------------------- */

class GameZulu : public game::BitBladeGame
{
public:

	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~GameZulu() {}

	const char* GetGameTitle() override;
	void Initialize() override;
	void Update() override;

	size_t totalUpdates = 3;
	size_t updateCount = 0;

	game::ImageSource* backgroundImage;
	game::ImageSource* treeImage;
	game::GameObject* background;
	game::GameObject* tree;
	game::GameObject* hero;
	game::GameObject* larm;
	game::GameObject* rarm;
	game::GameObject* sword;
	game::GameObject* shield;
};

const char* GameZulu::GetGameTitle() {
	return "TestGame-Zulu\n";
}

void GameZulu::Initialize()
{
	backgroundImage = LoadImageSource( "forrest_background.bmp" );
	treeImage = LoadImageSource( "pine_tree.bmp" );

	background = Instantiate( "Background" );
	tree = Instantiate( background, "tree" );
	hero = Instantiate( "Hero" );
	larm = Instantiate( hero, "leftarm" );
	rarm = Instantiate( hero, "rightarm" );
	sword = Instantiate( "Sword" );
	sword->SetParent( larm );

	background->AddComponent<DebugPrintComponent>(); 		//objID = 0
	tree->AddComponent<DebugPrintComponent>();      		//objID = 1

	hero->AddComponent<DebugPrintComponent>();      		//objID = 2
	larm->AddComponent<DebugPrintComponent>();      		//objID = 3
	rarm->AddComponent<DebugPrintComponent>();      		//objID = 4
	sword->AddComponent<DebugPrintComponent>();      		//objID = 5
}

void GameZulu::Update()
{
	if (updateCount == 1) {
		shield = Instantiate( rarm, "Shield" );
		shield->AddComponent<DebugPrintComponent>();      		//objID = 6
	}

	if (updateCount > totalUpdates) {

		QuitGame();
	}

	updateCount++;
}


/* -------------------------------- GAME BETA ------------------------------- */

class GameBeta : public game::BitBladeGame
{
public:

	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~GameBeta() {}

	const char* GetGameTitle() override;
	void Initialize() override;
	void Update() override;

	size_t totalUpdates = 3;
	size_t updateCount = 0;

	game::GameObject* hero;
	game::GameObject* larm;
	game::GameObject* rarm;
	game::GameObject* sword;
};

const char* GameBeta::GetGameTitle() {
	return "TestGame-Zulu\n";
}

void GameBeta::Initialize()
{
	hero = Instantiate( "Hero" );
	larm = Instantiate( hero, "leftarm" );
	rarm = Instantiate( hero, "rightarm" );
	sword = Instantiate( "Sword" );
	sword->SetParent( larm );

	hero->AddComponent<DebugOnTransformChanged>();
}

void GameBeta::Update()
{
	if (updateCount == 1) {
		hero->GetTransform()->SetPosition( 2, 3 );
	}

	if (updateCount > totalUpdates) {

		QuitGame();
	}

	updateCount++;
}

#endif // BIT_BLADE_GAMES_TEST_H