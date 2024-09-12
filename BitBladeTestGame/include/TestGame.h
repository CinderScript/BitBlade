/* TestGame.h
*/

#ifndef TEST_GAME_H 
#define TEST_GAME_H

#include "BitBladeGame.h"
#include "Prefab.h"

class TestGame : public BitBladeGame {
public:
	TestGame();
	~TestGame() override;

	void GameStart() override;

private:
	const Prefab* backgroundPrefab;
	const Prefab* heroPrefab;

	GameObject* background;
	GameObject* hero;
};

#endif // TEST_GAME_H