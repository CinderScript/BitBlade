// BitBladeGame_test.cpp


#include <gtest/gtest.h>
#include <iostream>

#include "GfxTransparentPacker.h"
#include "GameObject.h"
#include "DebugComponents.h"
#include "Logging.h"

using game::GameObject;

/* ----------------------------- BIT BLADE GAMES ---------------------------- */

class EmptyGame : public game::BitBladeGame
{
public:

	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~EmptyGame() {}

	const char* GetGameTitle() override;
	void GlobalStart() override;
	void GlobalUpdate() override;

	size_t totalUpdates = 3;
	size_t updateCount = 0;

};

const char* EmptyGame::GetGameTitle() {
	return "TestGame-Alpha\n";
}

void EmptyGame::GlobalStart()
{

}

void EmptyGame::GlobalUpdate()
{
	updateCount++;

	if (updateCount >= totalUpdates) {

		QuitGame();
	}

}

class GameWithObjects : public game::BitBladeGame
{
public:

	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~GameWithObjects() {}

	const char* GetGameTitle() override;
	void GlobalStart() override;
	void GlobalUpdate() override;

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

const char* GameWithObjects::GetGameTitle() {
	return "TestGame-ObjectsAndComponents\n";
}

void GameWithObjects::GlobalStart()
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

void GameWithObjects::GlobalUpdate()
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


/* -------------------------------- FIXTURES -------------------------------- */


class GameEngine_BitBladeGame_Functions : public ::testing::Test {
protected:
	EmptyGame* game;
	GfxTransparentPacker* gfxPacker;

	void SetUp() override {
		// Initialize the graphics packer and game instance
		gfxPacker = new GfxTransparentPacker();
		game = new EmptyGame( gfxPacker );
		game->totalUpdates = 3;
		game->GlobalStart();
	}

	void TearDown() override {
		delete game;
		delete gfxPacker;
	}
};

class GameEngine_BitBladeGame_ObjectTests : public ::testing::Test {
protected:
	GameWithObjects* gameZulu;
	GfxTransparentPacker* gfxPacker;
	std::string output;

	void SetUp() override {
		gfxPacker = new GfxTransparentPacker();
		gameZulu = new GameWithObjects( gfxPacker );
		std::string output = logging::captureCoutOutput( [this]() {
			gameZulu->GlobalStart();
			} );

	}

	void TearDown() override {
		delete gameZulu;
		delete gfxPacker;
	}
};

/* ---------------------------------- TESTS --------------------------------- */

TEST_F( GameEngine_BitBladeGame_Functions, QuitGame ) {
	bool shouldContinue = true;

	for (size_t i = 0; i < game->totalUpdates - 1; i++)
	{
		shouldContinue = game->internalUpdate();
		EXPECT_TRUE( shouldContinue );
	}

	// last update
	shouldContinue = game->internalUpdate();
	EXPECT_FALSE( shouldContinue );
}

TEST_F( GameEngine_BitBladeGame_Functions, InstantiateTopLevel ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	ASSERT_NE( obj, nullptr );
	EXPECT_STREQ( obj->Name(), "TestObject" );
	EXPECT_EQ( obj->Parent(), nullptr );
	EXPECT_EQ( obj->GetChildren().size(), 0 );
	EXPECT_EQ( game->GetTopLevelObjects().size(), 1 );
	EXPECT_EQ( game->GetTopLevelObjects()[0], obj );
}

TEST_F( GameEngine_BitBladeGame_Functions, InstantiateChild ) {
	GameObject* parent = game->Instantiate( "ParentObject" );
	GameObject* child = game->Instantiate( parent, "ChildObject" );

	ASSERT_NE( child, nullptr );
	EXPECT_STREQ( child->Name(), "ChildObject" );
	EXPECT_EQ( child->Parent(), parent );
	EXPECT_EQ( child->GetChildren().size(), 0 );
	EXPECT_EQ( parent->GetChildren().size(), 1 );
	EXPECT_EQ( parent->GetChildren()[0], child );
	EXPECT_EQ( game->GetTopLevelObjects().size(), 1 );
	EXPECT_EQ( game->GetTopLevelObjects()[0], parent );
}

// Test setting a new parent for a GameObject
TEST_F( GameEngine_BitBladeGame_Functions, SetParent ) {
	GameObject* parent1 = game->Instantiate( "Parent1" );
	GameObject* parent2 = game->Instantiate( "Parent2" );
	GameObject* child = game->Instantiate( parent1, "Child" );

	EXPECT_EQ( child->Parent(), parent1 );
	EXPECT_EQ( parent1->GetChildren().size(), 1 );
	EXPECT_EQ( parent2->GetChildren().size(), 0 );

	child->SetParent( parent2 );

	EXPECT_EQ( child->Parent(), parent2 );
	EXPECT_EQ( parent1->GetChildren().size(), 0 );
	EXPECT_EQ( parent2->GetChildren().size(), 1 );
	EXPECT_EQ( parent2->GetChildren()[0], child );

	// check child's transform's parent and see if it was also reparented
	EXPECT_TRUE( parent2->GetTransform() == child->GetTransform()->Parent() );
}

class TestComponent : public game::Component {
public:
	TestComponent( GameObject* owner ) : Component( owner ), updateCalled( false ) {}
	void Awake() override {}
	void Start() override {}
	void Update() override { updateCalled = true; }

	bool updateCalled;
	int testValue = 7;
};


// Test that Initialize creates the correct GameObjects and hierarchy
TEST_F( GameEngine_BitBladeGame_ObjectTests, GameObjectHierarchy ) {
	EXPECT_NE( gameZulu->background, nullptr );
	EXPECT_NE( gameZulu->tree, nullptr );
	EXPECT_NE( gameZulu->hero, nullptr );
	EXPECT_NE( gameZulu->larm, nullptr );
	EXPECT_NE( gameZulu->rarm, nullptr );
	EXPECT_NE( gameZulu->sword, nullptr );

	// Check the hierarchy
	EXPECT_EQ( gameZulu->background->GetChildren().size(), 1 );
	EXPECT_EQ( gameZulu->background->GetChildren()[0], gameZulu->tree );

	EXPECT_EQ( gameZulu->hero->GetChildren().size(), 2 );
	EXPECT_EQ( gameZulu->hero->GetChildren()[0], gameZulu->larm );
	EXPECT_EQ( gameZulu->hero->GetChildren()[1], gameZulu->rarm );

	EXPECT_EQ( gameZulu->larm->GetChildren().size(), 1 );
	EXPECT_EQ( gameZulu->larm->GetChildren()[0], gameZulu->sword );

	EXPECT_EQ( gameZulu->sword->Parent(), gameZulu->larm );
}

// Parent objects' components should be updated before child components.
TEST_F( GameEngine_BitBladeGame_ObjectTests, ComponenentUpdatePrecedence ) {
	// Capture the cout output while performing internalUpdate
	output = logging::captureCoutOutput( [this]() {
		gameZulu->internalUpdate();
		} );

	// Expected output sequence for DFS
	std::string expectedOutput =
		"Component-0-Start-Obj-Background\n"
		"Component-0-Update-Obj-Background\n"
		"Component-1-Start-Obj-tree\n"
		"Component-1-Update-Obj-tree\n"

		"Component-2-Start-Obj-Hero\n"
		"Component-2-Update-Obj-Hero\n"			//hero
		"Component-3-Start-Obj-leftarm\n"
		"Component-3-Update-Obj-leftarm\n"		//left arm
		"Component-5-Start-Obj-Sword\n"
		"Component-5-Update-Obj-Sword\n"		//sword
		"Component-4-Start-Obj-rightarm\n"
		"Component-4-Update-Obj-rightarm\n";	//right arm

	// Check if the output matches the expected sequence
	EXPECT_EQ( output, expectedOutput );

	output = logging::captureCoutOutput( [this]() {
		gameZulu->internalUpdate();
		} );
}


TEST_F( GameEngine_BitBladeGame_ObjectTests, InternalUpdateCycle ) {

	// Expected output sequence for DFS
	std::string expectedOutput =
		"Component-0-Awake-Obj-Background\n"
		"Component-1-Awake-Obj-tree\n"

		"Component-2-Awake-Obj-Hero\n"			//hero
		"Component-3-Awake-Obj-leftarm\n"		//left arm
		"Component-5-Awake-Obj-Sword\n"			//sword
		"Component-4-Awake-Obj-rightarm\n";		//right arm

	// UPDATE 0
	output = logging::captureCoutOutput( [this]()
		{
			gameZulu->internalUpdate();
		} );
	expectedOutput =
		"Component-0-Start-Obj-Background\n"
		"Component-0-Update-Obj-Background\n"
		"Component-1-Start-Obj-tree\n"
		"Component-1-Update-Obj-tree\n"

		"Component-2-Start-Obj-Hero\n"
		"Component-2-Update-Obj-Hero\n"			//hero
		"Component-3-Start-Obj-leftarm\n"
		"Component-3-Update-Obj-leftarm\n"		//left arm
		"Component-5-Start-Obj-Sword\n"
		"Component-5-Update-Obj-Sword\n"		//sword
		"Component-4-Start-Obj-rightarm\n"
		"Component-4-Update-Obj-rightarm\n";	//right arm

	EXPECT_EQ( output, expectedOutput );


	// UPDATE 1
	output = logging::captureCoutOutput( [this]() {
		gameZulu->internalUpdate();
		} );
	expectedOutput =
		"Component-6-Awake-Obj-Shield\n"

		"Component-0-Update-Obj-Background\n"
		"Component-1-Update-Obj-tree\n"

		"Component-2-Update-Obj-Hero\n"			//hero
		"Component-3-Update-Obj-leftarm\n"		//left arm
		"Component-5-Update-Obj-Sword\n"		//sword
		"Component-4-Update-Obj-rightarm\n"		//right arm
		"Component-6-Start-Obj-Shield\n"
		"Component-6-Update-Obj-Shield\n";		//shield

	EXPECT_EQ( output, expectedOutput );
}