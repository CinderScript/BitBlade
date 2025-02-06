/* TestGame.h
*/

#ifndef TEST_GAME_H 
#define TEST_GAME_H

#include "BitBladeGame.h"
#include "Transform.h"

#include "IGfxMessagePacker.h"
#include "ImageSource.h"

class TestGame : public game::BitBladeGame {
public:

	TestGame( IGfxMessagePacker* link );
	~TestGame() override;

	const char* GetGameTitle() override;
	void Initialize() override;
	void GlobalUpdate() override;

private:
	const game::ImageSource* backgroundImage;
	const game::ImageSource* heroImage;

	game::GameObject* background;
	game::GameObject* tree;
	game::GameObject* hero;
	game::GameObject* arm;
	game::GameObject* sword;

	game::Transform* heroTransform = nullptr;
	game::Transform* armTransform = nullptr;
	game::Transform* swordTransform = nullptr;

	size_t updateCount = 0;
};

#endif // TEST_GAME_H