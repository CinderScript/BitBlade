/* GfxPacking_testDriver.cpp*/


#include <gtest/gtest.h>
#include <iostream>

#include "BitBladeGame_test.h"
#include "GfxTestPacker_test.h"
#include "GameObject.h"

using game::GameObject;

// class GameBetaTest : public ::testing::Test {
// protected:
// 	GameBeta* game;
// 	GfxTestPacker* gfxPacker;

// 	void SetUp() override {
// 		// Initialize the graphics packer and game instance
// 		gfxPacker = new GfxTestPacker();
// 		game = new GameBeta( gfxPacker );
// 		game->totalUpdates = 3;
// 		game->Initialize();
// 		game->Start();
// 	}

// 	void TearDown() override {
// 		delete game;
// 		delete gfxPacker;
// 	}
// };


// Utility function to capture std::cout output
std::string captureCoutOutput( std::function<void()> func ) {
	std::stringstream buffer;
	std::streambuf* prevCoutBuf = std::cout.rdbuf( buffer.rdbuf() );  // Redirect cout

	func();  // Execute the function that will produce output to std::cout

	std::cout.rdbuf( prevCoutBuf );  // Restore cout
	return buffer.str();
}


// TEST_F( GameBetaTest, ImageSourcePacking ) {


// }