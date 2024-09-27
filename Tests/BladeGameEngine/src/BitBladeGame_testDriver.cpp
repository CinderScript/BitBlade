// BitBladeGame_test.cpp


#include <gtest/gtest.h>
#include <iostream>

#include "BitBladeGame_test.h"
#include "GfxTestPacker_test.h"
#include "GameObject.h"

using game::GameObject;

/* -------------------------------- FIXTURES -------------------------------- */

class GameAlphaTest : public ::testing::Test {
protected:
	GameAlpha* game;
	GfxTestPacker* packer;

	GameAlphaTest() {
		packer = new GfxTestPacker();
		game = new GameAlpha( packer );
	}

	void SetUp() override {
		game->totalUpdates = 3;
		game->Initialize();
		game->Start();
	}

	void TearDown() override {
		delete game;
		delete packer;
	}
};


class BitBladeGameTest : public ::testing::Test {
protected:
	GameAlpha* game;
	GfxTestPacker* gfxPacker;

	void SetUp() override {
		// Initialize the graphics packer and game instance
		gfxPacker = new GfxTestPacker();
		game = new GameAlpha( gfxPacker );
		game->totalUpdates = 3;
		game->Initialize();
		game->Start();
	}

	void TearDown() override {
		delete game;
		delete gfxPacker;
	}
};

/* ---------------------------------- TESTS --------------------------------- */

TEST_F( BitBladeGameTest, QuitGame ) {
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

TEST_F( BitBladeGameTest, InstantiateTopLevel ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	ASSERT_NE( obj, nullptr );
	EXPECT_STREQ( obj->Name(), "TestObject" );
	EXPECT_EQ( obj->Parent(), nullptr );
	EXPECT_EQ( obj->GetChildren().size(), 0 );
	EXPECT_EQ( game->GetTopLevelObjects().size(), 1 );
	EXPECT_EQ( game->GetTopLevelObjects()[0], obj );
}

TEST_F( BitBladeGameTest, InstantiateChild ) {
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
TEST_F( BitBladeGameTest, SetParent ) {
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
}

class TestComponent : public game::Component {
public:
	TestComponent( GameObject* owner ) : Component( owner ), updateCalled( false ) {}
	void Awake() override {}
	void Start() override {}
	void Update() override { updateCalled = true; }

	bool updateCalled;
};


TEST_F( BitBladeGameTest, AddComponent ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	TestComponent* comp = obj->AddComponent<TestComponent>();
	ASSERT_NE( comp, nullptr );
	EXPECT_EQ( comp->Owner(), obj );
	EXPECT_EQ( obj->GetComponents().size(), 1 );
	EXPECT_EQ( obj->GetComponents()[0], comp );
}

// Test retrieving a component from a GameObject
TEST_F( BitBladeGameTest, GetComponent ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	TestComponent* comp = obj->AddComponent<TestComponent>();
	ASSERT_NE( comp, nullptr );
	TestComponent* retrievedComp = obj->GetComponent<TestComponent>();
	EXPECT_EQ( comp, retrievedComp );
}

// Test the internalUpdate method of GameObject
TEST_F( BitBladeGameTest, InternalUpdate ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	TestComponent* comp = obj->AddComponent<TestComponent>();
	comp->updateCalled = false;

	game->internalUpdate();

	EXPECT_TRUE( comp->updateCalled );
}

/* ---------------------------------- ZULU ---------------------------------- */


// Utility function to capture std::cout output
std::string captureCoutOutput( std::function<void()> func ) {
	std::stringstream buffer;
	std::streambuf* prevCoutBuf = std::cout.rdbuf( buffer.rdbuf() );  // Redirect cout

	func();  // Execute the function that will produce output to std::cout

	std::cout.rdbuf( prevCoutBuf );  // Restore cout
	return buffer.str();
}

class GameZuluTest : public ::testing::Test {
protected:
	GameZulu* gameZulu;
	GfxTestPacker* gfxPacker;
	std::string output;

	void SetUp() override {
		gfxPacker = new GfxTestPacker();
		gameZulu = new GameZulu( gfxPacker );
		std::string output = captureCoutOutput( [this]() {
			gameZulu->Initialize();
			} );

	}

	void TearDown() override {
		delete gameZulu;
		delete gfxPacker;
	}
};

// Test that Initialize creates the correct GameObjects and hierarchy
TEST_F( GameZuluTest, GameObjectHierarchy ) {
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


TEST_F( GameZuluTest, UpdatePrecedence ) {
	// Capture the cout output while performing internalUpdate
	output = captureCoutOutput( [this]() {
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

	output = captureCoutOutput( [this]() {
		gameZulu->internalUpdate();
		} );
}


TEST_F( GameZuluTest, GameLogicTest ) {

	// Expected output sequence for DFS
	std::string expectedOutput =
		"Component-0-Awake-Obj-Background\n"
		"Component-1-Awake-Obj-tree\n"

		"Component-2-Awake-Obj-Hero\n"			//hero
		"Component-3-Awake-Obj-leftarm\n"		//left arm
		"Component-5-Awake-Obj-Sword\n"			//sword
		"Component-4-Awake-Obj-rightarm\n";		//right arm

	// UPDATE 0
	output = captureCoutOutput( [this]()
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
	output = captureCoutOutput( [this]() {
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

TEST_F( GameZuluTest, ImageSourcePacking ) {
	// Capture the cout output while performing internalUpdate
	output = captureCoutOutput( [this]() {
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

	output = captureCoutOutput( [this]() {
		gameZulu->internalUpdate();
		} );
}