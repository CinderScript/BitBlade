/* TestGame.h
*/

#ifndef TEST_GAME_H 
#define TEST_GAME_H

#include "BitBladeGame.h"
#include "ImageSource.h"

class TestGame : public game::BitBladeGame {
public:
	friend class console::BladeConsole;

	TestGame();
	~TestGame() override;

	const char* GetGameTitle() override;
	void GameStart() override;

private:
	const game::ImageSource* backgroundImage;
	const game::ImageSource* heroImage;

	game::GameObject* background;
	game::GameObject* hero;
};

#endif // TEST_GAME_H