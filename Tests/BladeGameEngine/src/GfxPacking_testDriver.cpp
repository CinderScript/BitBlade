/* GfxPacking_testDriver.cpp*/


#include <gtest/gtest.h>
#include <iostream>

#include "GfxTransparentPacker.h"
#include "GameObject.h"
#include "GfxLinkCommon.h"


using game::GameObject;
using gfxLink::GfxCode;
using gfxLink::readMessageBuffer;
using gfxLink::readMessageBufferString;
using gfxLink::toGfxCommand;

/* ----------------------------- BIT BLADE GAMES ---------------------------- */

class GfxPackingGameTest : public game::BitBladeGame
{
public:
	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~GfxPackingGameTest() {}

	const char* GetGameTitle() override { return "GfxPackingGameTest"; }
	void GlobalStart() override {

	}
	void GlobalUpdate() override {}

	void CreateImageSource() {
		heroImg = LoadImageSource( "Hero.bmp" );
		swordImg = LoadImageSource( "Sword.bmp" );
	}
	void CreateSprite() {
		heroImg = LoadImageSource( "Hero.bmp" );
		swordImg = LoadImageSource( "Sword.bmp" );
	}

	size_t totalUpdates = 3;
	size_t updateCount = 0;

	game::ImageSource* heroImg;
	game::ImageSource* swordImg;
	game::GameObject* hero;
	game::GameObject* sword;
};


/* -------------------------------- FIXTURES -------------------------------- */

class GameEngine_GfxPacking_Message : public ::testing::Test {
protected:
	GfxPackingGameTest* game;
	GfxTransparentPacker* gfxPacker;

	void SetUp() override {
		// Initialize the graphics packer and game instance
		gfxPacker = new GfxTransparentPacker();
		game = new GfxPackingGameTest( gfxPacker );
		game->totalUpdates = 3;
	}

	void TearDown() override {
		delete game;
		delete gfxPacker;
	}
};

/* ---------------------------------- TESTS --------------------------------- */

TEST_F( GameEngine_GfxPacking_Message, CreateImageData ) {

	game->CreateImageSource();

	char* buffer = gfxPacker->packedInstructions;
	uint16_t pos;
	GfxCode cmd;

	cmd = toGfxCommand( buffer[pos++] );
	uint16_t imageDataID;
	char filename[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];
	readMessageBuffer( buffer, imageDataID, pos );
	readMessageBufferString( buffer, filename, pos );

	EXPECT_EQ( cmd, GfxCode::CreateImageData );
	EXPECT_EQ( imageDataID, 0 );
	EXPECT_STREQ( filename, "Hero.bmp" );

	cmd = toGfxCommand( buffer[pos++] );
	readMessageBuffer( buffer, imageDataID, pos );
	readMessageBufferString( buffer, filename, pos );

	EXPECT_EQ( cmd, GfxCode::CreateImageData );
	EXPECT_EQ( imageDataID, 1 );
	EXPECT_STREQ( filename, "Sword.bmp" );
}

TEST_F( GameEngine_GfxPacking_Message, CreateSpriteInstance ) {

	game->GlobalStart();

	char* buffer = gfxPacker->packedInstructions;
	uint16_t pos;
	GfxCode cmd;

	cmd = toGfxCommand( buffer[pos++] );
	uint16_t imageDataID;
	char filename[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];
	readMessageBuffer( buffer, imageDataID, pos );
	readMessageBufferString( buffer, filename, pos );

	EXPECT_EQ( cmd, GfxCode::CreateImageData );
	EXPECT_EQ( imageDataID, 0 );
	EXPECT_STREQ( filename, "Hero.bmp" );
}