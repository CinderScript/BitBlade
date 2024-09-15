/* TestGame.h
*/

#ifndef TEST_GAME_H 
#define TEST_GAME_H

#include "BitBladeGame.h"
#include "ImageSource.h"

class TestGame : public BitBladeGame {
public:
	friend class console::BladeConsole;

	TestGame();
	~TestGame() override;

	void GameStart() override;

private:
	const ImageSource* backgroundImage;
	const ImageSource* heroImage;

	GameObject* background;
	GameObject* hero;
};

#endif // TEST_GAME_H