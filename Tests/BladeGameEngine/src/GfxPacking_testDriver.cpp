/* GfxPacking_testDriver.cpp*/


#include <gtest/gtest.h>
#include <iostream>

#include "GfxTestPacker_test.h"
#include "GameObject.h"
#include "GfxLinkCommon.h"


using game::GameObject;
using gfxLink::GfxCode;
using gfxLink::readMessageBuffer;
using gfxLink::readMessageBufferString;
using gfxLink::toGfxCommand;

class GfxPackingGameTest : public game::BitBladeGame
{
public:
	using BitBladeGame::BitBladeGame;  // Inherit the base constructor
	~GfxPackingGameTest() {}

	const char* GetGameTitle() override { return "GfxPackingGameTest"; }
	void Initialize() override {
		heroImg = LoadImageSource( "Hero.bmp" );
		swordImg = LoadImageSource( "Sword.bmp" );
	}
	void Update() override {

	}

	size_t totalUpdates = 3;
	size_t updateCount = 0;

	game::ImageSource* heroImg;
	game::ImageSource* swordImg;
	game::GameObject* hero;
	game::GameObject* sword;
};


class GfxPackingTest : public ::testing::Test {
protected:
	GfxPackingGameTest* game;
	GfxTestPacker* gfxPacker;

	void SetUp() override {
		// Initialize the graphics packer and game instance
		gfxPacker = new GfxTestPacker();
		game = new GfxPackingGameTest( gfxPacker );
		game->totalUpdates = 3;
	}

	void TearDown() override {
		delete game;
		delete gfxPacker;
	}
};


TEST_F( GfxPackingTest, LoadImageSource ) {

	game->Initialize();

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