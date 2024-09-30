/* TransformComponent_testDriver.cpp*/


#include <gtest/gtest.h>
#include <iostream>

#include "BitBladeGame_test.h"
#include "GfxTestPacker_test.h"
#include "GameObject.h"

// Tests for the Transform component
class TransformTest : public ::testing::Test {
protected:
	GameBeta* game;
	GfxTestPacker* gfxPacker;
	game::GameObject* gameObject;
	game::Transform* transform;

	void SetUp() override {
		// Initialize the graphics packer and game instance
		gfxPacker = new GfxTestPacker();
		game = new GameBeta( gfxPacker );
		game->totalUpdates = 3;
		game->Initialize();
		game->Start();

		gameObject = game->hero;
		transform = game->hero->GetTransform();
	}

	void TearDown() override {
		delete game;
		delete gfxPacker;
	}
};

// Test setting and getting position
TEST_F( TransformTest, SetGetPosition ) {
	transform->SetPosition( 5.0f, 10.0f );
	EXPECT_EQ( transform->Position(), game::Vector2( 5.0f, 10.0f ) );

	transform->SetPosition( game::Vector2( 15.0f, 20.0f ) );
	EXPECT_EQ( transform->Position(), game::Vector2( 15.0f, 20.0f ) );
}

// Test setting and getting rotation
TEST_F( TransformTest, SetGetRotation ) {
	transform->SetRotation( 45.0f );
	EXPECT_FLOAT_EQ( transform->Rotation(), 45.0f );

	transform->Rotate( 30.0f );
	EXPECT_FLOAT_EQ( transform->Rotation(), 75.0f );

	transform->Rotate( -90.0f );
	EXPECT_FLOAT_EQ( transform->Rotation(), 345.0f ); // Rotation wraps around 360
}

// Test setting and getting scale
TEST_F( TransformTest, SetGetScale ) {
	transform->SetScale( game::Vector2( 1.0f, 1.0f ) );
	EXPECT_EQ( transform->Scale(), game::Vector2( 1.0f, 1.0f ) );

	transform->SetScale( game::Vector2( 2.0f, 3.0f ) );
	EXPECT_EQ( transform->Scale(), game::Vector2( 2.0f, 3.0f ) );
}

// Test moving the transform by delta
TEST_F( TransformTest, MoveByDelta ) {
	transform->SetPosition( 0.0f, 0.0f );
	transform->Move( 5.0f, 5.0f );
	EXPECT_EQ( transform->Position(), game::Vector2( 5.0f, 5.0f ) );

	transform->Move( game::Vector2( -2.0f, -3.0f ) );
	EXPECT_EQ( transform->Position(), game::Vector2( 3.0f, 2.0f ) );
}

// Test moving in a specific direction
TEST_F( TransformTest, MoveInDirection ) {
	transform->SetPosition( 0.0f, 0.0f );
	game::Vector2 direction( 1.0f, 0.0f ); // Move right
	transform->Move( 10.0f, direction );
	EXPECT_EQ( transform->Position(), game::Vector2( 10.0f, 0.0f ) );

	direction = game::Vector2( 0.0f, 1.0f ); // Move down
	transform->Move( 5.0f, direction );
	EXPECT_EQ( transform->Position(), game::Vector2( 10.0f, 5.0f ) );
}

// Test forward movement
TEST_F( TransformTest, ForwardMovement ) {
	transform->SetPosition( 0.0f, 0.0f );
	transform->SetRotation( 0.0f ); // Facing right
	transform->Forward( 10.0f );
	EXPECT_NEAR( transform->Position().X(), 10.0f, 1e-5 );
	EXPECT_NEAR( transform->Position().Y(), 0.0f, 1e-5 );

	transform->SetRotation( 90.0f ); // Facing down
	transform->Forward( 5.0f );
	EXPECT_NEAR( transform->Position().X(), 10.0f, 1e-5 );
	EXPECT_NEAR( transform->Position().Y(), 5.0f, 1e-5 );

	transform->SetRotation( 180.0f ); // Facing left
	transform->Forward( 3.0f );
	EXPECT_NEAR( transform->Position().X(), 7.0f, 1e-5 );
	EXPECT_NEAR( transform->Position().Y(), 5.0f, 1e-5 );

	transform->SetRotation( 270.0f ); // Facing up
	transform->Forward( 2.0f );
	EXPECT_NEAR( transform->Position().X(), 7.0f, 1e-5 );
	EXPECT_NEAR( transform->Position().Y(), 3.0f, 1e-5 );
}

// Test NormalizeRotation helper function
TEST_F( TransformTest, NormalizeRotation ) {
	transform->SetRotation( 370.0f );
	EXPECT_FLOAT_EQ( transform->Rotation(), 10.0f );

	transform->SetRotation( -30.0f );
	EXPECT_FLOAT_EQ( transform->Rotation(), 330.0f );

	transform->Rotate( 90.0f );
	EXPECT_FLOAT_EQ( transform->Rotation(), 60.0f );

	transform->Rotate( -120.0f );
	EXPECT_FLOAT_EQ( transform->Rotation(), 300.0f );
}

// Test updateForwardVector correctness
TEST_F( TransformTest, UpdateForwardVector ) {
	transform->SetRotation( 0.0f );
	EXPECT_NEAR( transform->Forward().X(), 1.0f, 1e-5 );
	EXPECT_NEAR( transform->Forward().Y(), 0.0f, 1e-5 );

	transform->SetRotation( 90.0f );
	EXPECT_NEAR( transform->Forward().X(), 0.0f, 1e-5 );
	EXPECT_NEAR( transform->Forward().Y(), 1.0f, 1e-5 );

	transform->SetRotation( 180.0f );
	EXPECT_NEAR( transform->Forward().X(), -1.0f, 1e-5 );
	EXPECT_NEAR( transform->Forward().Y(), 0.0f, 1e-5 );

	transform->SetRotation( 270.0f );
	EXPECT_NEAR( transform->Forward().X(), 0.0f, 1e-5 );
	EXPECT_NEAR( transform->Forward().Y(), -1.0f, 1e-5 );
}

// Test combining movement and rotation
TEST_F( TransformTest, MoveAndRotateCombination ) {
	transform->SetPosition( 0.0f, 0.0f );
	transform->SetRotation( 45.0f ); // Facing diagonal

	transform->Forward( 10.0f );
	EXPECT_NEAR( transform->Position().X(), 7.07107f, 1e-5 );
	EXPECT_NEAR( transform->Position().Y(), 7.07107f, 1e-5 );

	transform->Rotate( 45.0f ); // Now facing down
	transform->Forward( 5.0f );
	EXPECT_NEAR( transform->Position().X(), 7.07107f, 1e-5 );
	EXPECT_NEAR( transform->Position().Y(), 12.07107f, 1e-5 );
}

// Test scaling
TEST_F( TransformTest, Scaling ) {
	transform->SetScale( game::Vector2( 1.0f, 1.0f ) );
	EXPECT_EQ( transform->Scale(), game::Vector2( 1.0f, 1.0f ) );

	transform->SetScale( game::Vector2( 2.0f, 0.5f ) );
	EXPECT_EQ( transform->Scale(), game::Vector2( 2.0f, 0.5f ) );
}

// Test moving with zero length direction
TEST_F( TransformTest, MoveWithZeroLengthDirection ) {
	transform->SetPosition( 0.0f, 0.0f );
	game::Vector2 zeroDirection( 0.0f, 0.0f );
	transform->Move( 10.0f, zeroDirection );
	EXPECT_EQ( transform->Position(), game::Vector2( 0.0f, 0.0f ) ); // Position should not change
}
