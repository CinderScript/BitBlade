// /* TransformComponent_testDriver.cpp*/


#include <gtest/gtest.h>
#include <iostream>

#include "GameObject.h"
#include "GfxTransparentLink.h"
#include "DebugComponents.h"

using game::GameObject;


/* ----------------------------- BIT BLADE GAMES ---------------------------- */

class TestGame : public game::BitBladeGame
{
public:

	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~TestGame() {}

	const char* GetGameTitle() override;
	void GlobalStart() override;
	void GlobalUpdate() override;

	size_t totalUpdates = 3;
	size_t updateCount = 0;

};

const char* TestGame::GetGameTitle() {
	return "TestGame-Alpha\n";
}

void TestGame::GlobalStart()
{

}

void TestGame::GlobalUpdate()
{
	updateCount++;

	if (updateCount >= totalUpdates) {

		QuitGame();
	}

}

/* -------------------------------- FIXTURES -------------------------------- */

class TestComponent : public game::Component {
public:
	TestComponent( uint16_t pID, uint16_t oID, GameObject* owner ) : Component( pID, oID, owner ), updateCalled( false ) {}
	void Awake() override {}
	void Start() override {}
	void Update() override { updateCalled = true; }

	bool updateCalled;
	int testValue = 7;
};


class GameEngine_Components_General : public ::testing::Test {
protected:
	TestGame* game;
	GfxTransparentLink* gfxPacker;

	void SetUp() override {
		// Initialize the graphics packer and game instance
		gfxPacker = new GfxTransparentLink();
		game = new TestGame( gfxPacker );
		game->totalUpdates = 3;
		game->GlobalStart();
	}

	void TearDown() override {
		delete game;
		delete gfxPacker;
	}
};

TEST_F( GameEngine_Components_General, AddComponent ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	TestComponent* comp = obj->AddComponent<TestComponent>();
	ASSERT_NE( comp, nullptr );
	EXPECT_EQ( comp->Owner(), obj );
	EXPECT_EQ( obj->GetComponents().size(), 2 );	// all GameObjects have Transform comp
	EXPECT_EQ( obj->GetComponents()[1], comp );		// Transform has index  0
}

// Test retrieving a component from a GameObject
TEST_F( GameEngine_Components_General, GetComponent ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	TestComponent* comp = obj->AddComponent<TestComponent>();
	ASSERT_NE( comp, nullptr );
	TestComponent* retrievedComp = obj->GetComponent<TestComponent>();
	EXPECT_EQ( comp, retrievedComp );

	EXPECT_EQ( comp->testValue, retrievedComp->testValue );
	comp->testValue = 99;
	EXPECT_EQ( comp->testValue, retrievedComp->testValue );
}

// Test retrieving multiple components from a GameObject
TEST_F( GameEngine_Components_General, GetComponents ) {
	GameObject* obj = game->Instantiate( "TestObject" );

	// Add multiple components of the same type
	TestComponent* comp1 = obj->AddComponent<TestComponent>();
	TestComponent* comp2 = obj->AddComponent<TestComponent>();

	// Retrieve the components of type TestComponent
	std::vector<TestComponent*> components = obj->GetComponents<TestComponent>();

	// Assert that we retrieved two components
	ASSERT_EQ( components.size(), 2 );

	// Check that the retrieved components are the ones we added
	EXPECT_EQ( components[0], comp1 );
	EXPECT_EQ( components[1], comp2 );

	// Modify and check that both retrieved components reflect the changes
	comp1->testValue = 42;
	EXPECT_EQ( components[0]->testValue, 42 );
	EXPECT_EQ( comp1->testValue, 42 );

	comp2->testValue = 99;
	EXPECT_EQ( components[1]->testValue, 99 );
	EXPECT_EQ( comp2->testValue, 99 );
}


// Test the internalUpdate method of GameObject
TEST_F( GameEngine_Components_General, ComponentUpdate ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	TestComponent* comp = obj->AddComponent<TestComponent>();
	comp->updateCalled = false;

	game->internalUpdate();

	EXPECT_TRUE( comp->updateCalled );
}

class UniqueComponent : public game::Component {
public:
	UniqueComponent( uint16_t pID, uint16_t oID, GameObject* owner ) : Component( pID, oID, owner ), updateCalled( false ) {}
	void Awake() override {}
	void Start() override {}
	void Update() override { updateCalled = true; }

	bool updateCalled;
	int testValue = 7;
};

MAKE_COMPONENT_UNIQUE( UniqueComponent );

// Test the internalUpdate method of GameObject
TEST_F( GameEngine_Components_General, MakeUnique ) {
	GameObject* obj = game->Instantiate( "TestObject" );
	UniqueComponent* uniqueComp = obj->AddComponent<UniqueComponent>();
	//const std::vector<UniqueComponent*>* components = &obj->GetComponents<UniqueComponent>());
	const std::vector<UniqueComponent*> components = obj->GetComponents<UniqueComponent>();

	ASSERT_EQ( components.size(), 1 );
	EXPECT_DEATH( obj->AddComponent<UniqueComponent>(), ".*Attempted to add a duplicate unique component.*" );
}