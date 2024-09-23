/* TestGame.h
*/

#ifndef TEST_GAME_H 
#define TEST_GAME_H

#include "BitBladeGame.h"
#include "IGfxMessageLink.h"
#include "ImageSource.h"

class TestGame : public game::BitBladeGame {
public:

	TestGame( IGfxMessageLink* link );
	~TestGame() override;

	const char* GetGameTitle() override;
	void Initialize() override;

private:
	const game::ImageSource* backgroundImage;
	const game::ImageSource* heroImage;

	game::GameObject* background;
	game::GameObject* hero;
};

#endif // TEST_GAME_H