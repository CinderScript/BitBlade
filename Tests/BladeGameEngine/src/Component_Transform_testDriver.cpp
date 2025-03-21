// /* TransformComponent_testDriver.cpp*/


#include <gtest/gtest.h>
#include <iostream>

#include "GfxTestPacker_test.h"
#include "GameObject.h"
#include "Vector2.h"

#include "DebugComponents.h"

using game::Vector2;

// /* -------------------------------- TransformTestGame ------------------------------- */

class TransformTestGame : public game::BitBladeGame
{
public:

	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~TransformTestGame() {}

	const char* GetGameTitle() override;
	void Initialize() override;

	game::GameObject* hero;
	game::GameObject* arm;
	game::GameObject* sword;
};

const char* TransformTestGame::GetGameTitle() {
	return "TestGame-TransformTests\n";
}

void TransformTestGame::Initialize()
{
	hero = Instantiate( "Hero" );
	arm = Instantiate( hero, "leftarm" );
	sword = Instantiate( "Sword" );
	sword->SetParent( arm );

	if (sword->GetTransform()->Parent() == nullptr) {
		std::cout << "Error: Sword transform parent is null!\n";
	}
}


// Tests for the Transform component
class GameEngine_Components_Transform : public ::testing::Test {
protected:
	TransformTestGame* game;
	GfxTestPacker* gfxPacker;

	game::GameObject* hero;
	game::Transform* heroTransform;

	game::GameObject* arm;
	game::Transform* armTransform;

	game::GameObject* sword;
	game::Transform* swordTransform;

	void SetUp() override {
		// Initialize the graphics packer and game instance
		gfxPacker = new GfxTestPacker();
		game = new TransformTestGame( gfxPacker );
		game->Initialize();
		game->Start();

		hero = game->hero;
		heroTransform = game->hero->GetTransform();

		arm = game->arm;
		armTransform = game->arm->GetTransform();

		sword = game->sword;
		swordTransform = game->sword->GetTransform();
	}

	void TearDown() override {
		delete game;
		delete gfxPacker;
	}
};

// Test setting and getting position
TEST_F( GameEngine_Components_Transform, SetGetPosition ) {
	heroTransform->SetPosition( 5.0f, 10.0f );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 5.0f, 10.0f ) );

	heroTransform->SetPosition( game::Vector2( 15.0f, 20.0f ) );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 15.0f, 20.0f ) );
}

// Test setting and getting rotation
TEST_F( GameEngine_Components_Transform, SetGetRotation ) {
	heroTransform->SetRotation( 45.0f );
	float rotation = heroTransform->Rotation();
	EXPECT_FLOAT_EQ( rotation, 45.0f );

	heroTransform->Rotate( 30.0f );
	rotation = heroTransform->Rotation();
	EXPECT_FLOAT_EQ( rotation, 75.0f );
}

// Test setting and getting scale
TEST_F( GameEngine_Components_Transform, SetGetScale ) {
	heroTransform->SetGlobalScale( game::Vector2( 1.0f, 1.0f ) );
	EXPECT_EQ( heroTransform->Scale(), game::Vector2( 1.0f, 1.0f ) );

	heroTransform->SetGlobalScale( game::Vector2( 2.0f, 3.0f ) );
	EXPECT_EQ( heroTransform->Scale(), game::Vector2( 2.0f, 3.0f ) );
}

// Test moving the herotransform by delta
TEST_F( GameEngine_Components_Transform, MoveByDelta ) {
	heroTransform->SetPosition( 0.0f, 0.0f );
	heroTransform->Move( 5.0f, 5.0f );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 5.0f, 5.0f ) );

	heroTransform->Move( game::Vector2( -2.0f, -3.0f ) );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 3.0f, 2.0f ) );
}

// Test moving in a specific direction
TEST_F( GameEngine_Components_Transform, MoveInDirection ) {
	heroTransform->SetPosition( 0.0f, 0.0f );
	game::Vector2 direction( 1.0f, 0.0f ); // Move right
	heroTransform->Move( 10.0f, direction );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 10.0f, 0.0f ) );

	direction = game::Vector2( 0.0f, 1.0f ); // Move down
	heroTransform->Move( 5.0f, direction );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 10.0f, 5.0f ) );
}

// Test forward movement
TEST_F( GameEngine_Components_Transform, ForwardMovement ) {
	heroTransform->SetPosition( 0.0f, 0.0f );
	heroTransform->SetRotation( 0.0f ); // Facing Up
	heroTransform->MoveForward( 10.0f );

	Vector2 pos = heroTransform->Position();
	EXPECT_TRUE( pos == Vector2( 0, 10 ) );
	// passes

	heroTransform->SetRotation( 90.0f ); // Facing Right
	heroTransform->MoveForward( 5.0f );
	pos = heroTransform->Position();
	EXPECT_TRUE( pos == Vector2( 5, 10 ) );
	// fails, pos == -5, 10

	heroTransform->SetRotation( 180.0f ); // Facing Down
	heroTransform->MoveForward( 3.0f );
	pos = heroTransform->Position();
	EXPECT_TRUE( pos == Vector2( 5, 7 ) );

	heroTransform->SetRotation( 270.0f ); // Facing Left
	heroTransform->MoveForward( 2.0f );
	pos = heroTransform->Position();
	EXPECT_TRUE( pos == Vector2( 3, 7 ) );
}

// Test combining movement and rotation
TEST_F( GameEngine_Components_Transform, MoveAndRotateCombination ) {
	heroTransform->SetPosition( 0.0f, 0.0f );
	heroTransform->SetRotation( 45.0f ); // Facing diagonal

	heroTransform->MoveForward( 10.0f );
	EXPECT_NEAR( heroTransform->Position().X(), 7.07107f, 1e-5 );
	EXPECT_NEAR( heroTransform->Position().Y(), 7.07107f, 1e-5 );

	heroTransform->Rotate( 45.0f ); // Now facing down
	heroTransform->MoveForward( 5.0f );
	EXPECT_NEAR( heroTransform->Position().X(), 7.07107f, 1e-5 );
	EXPECT_NEAR( heroTransform->Position().Y(), 12.07107f, 1e-5 );
}

// Test scaling
TEST_F( GameEngine_Components_Transform, Scaling ) {
	heroTransform->SetGlobalScale( game::Vector2( 1.0f, 1.0f ) );
	EXPECT_EQ( heroTransform->Scale(), game::Vector2( 1.0f, 1.0f ) );

	heroTransform->SetGlobalScale( game::Vector2( 2.0f, 0.5f ) );
	EXPECT_EQ( heroTransform->Scale(), game::Vector2( 2.0f, 0.5f ) );
}

// Test moving with zero length direction
TEST_F( GameEngine_Components_Transform, MoveWithZeroLengthDirection ) {
	heroTransform->SetPosition( 0.0f, 0.0f );
	game::Vector2 zeroDirection( 0.0f, 0.0f );
	heroTransform->Move( 10.0f, zeroDirection );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 0.0f, 0.0f ) ); // Position should not change
}

// Make sure only one Transform can be added to a GameObject
TEST_F( GameEngine_Components_Transform, IsUniqueComponent ) {
	EXPECT_DEATH( game->hero->AddComponent<game::Transform>(), ".*Attempted to add a duplicate unique component.*" );
}

// Test that setting the local position updates the local translation correctly.
TEST_F( GameEngine_Components_Transform, LocalPositionTest )
{
	// For a top-level object (hero), local and global positions are the same.
	heroTransform->SetLocalPosition( 10.0f, 20.0f );
	EXPECT_EQ( heroTransform->LocalPosition(), game::Vector2( 10.0f, 20.0f ) );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 10.0f, 20.0f ) );

	// Modify via the overload that takes a Vector2.
	heroTransform->SetLocalPosition( game::Vector2( 15.0f, 25.0f ) );
	EXPECT_EQ( heroTransform->LocalPosition(), game::Vector2( 15.0f, 25.0f ) );
	EXPECT_EQ( heroTransform->Position(), game::Vector2( 15.0f, 25.0f ) );
}

// Test that child transforms combine their own local translation with their parent's global transform.
TEST_F( GameEngine_Components_Transform, ParentChildTransformTest )
{
	heroTransform->SetPosition( 10.0f, 20.0f );
	armTransform->SetLocalPosition( 5.0f, 5.0f );

	// Debugging variables
	Vector2 armLocalPos = armTransform->LocalPosition();
	Vector2 armGlobalPos = armTransform->Position();

	// Check the expected local and global positions
	EXPECT_EQ( armLocalPos, game::Vector2( 5.0f, 5.0f ) );
	EXPECT_EQ( armGlobalPos, game::Vector2( 15.0f, 25.0f ) );

	heroTransform->SetRotation( 90.0f );
	armGlobalPos = armTransform->Position();
	EXPECT_EQ( armGlobalPos, game::Vector2( 15.0f, 15.0f ) );

	// Debugging variables for sword
	Vector2 swordLocalPos = swordTransform->LocalPosition();
	Vector2 swordGlobalPos = swordTransform->Position();

	swordTransform->SetLocalPosition( 2.0f, 3.0f );

	// Debugging variables for sword
	swordLocalPos = swordTransform->LocalPosition();
	swordGlobalPos = swordTransform->Position();

	// Check expected values
	EXPECT_EQ( swordLocalPos, game::Vector2( 2.0f, 3.0f ) );
	EXPECT_EQ( swordGlobalPos, game::Vector2( 18.0f, 13.0f ) );
}