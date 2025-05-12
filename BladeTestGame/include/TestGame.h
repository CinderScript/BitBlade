/* TestGame.h
*/

#ifndef TEST_GAME_H 
#define TEST_GAME_H

#include "BitBladeGame.h"
#include "Transform.h"

#include "IGfxMessageLink.h"
#include "ImageSource.h"

class TestGame : public game::BitBladeGame {
public:

	TestGame( gfxLink::IGfxMessageLink* link );
	~TestGame() override;

	const char* GetGameTitle() override;
	void GlobalStart() override;
	void GlobalUpdate() override;

private:
	const game::ImageSource* backgroundImage;
	const game::ImageSource* heroImage;

	game::GameObject* background = nullptr;
	game::GameObject* tree = nullptr;
	game::GameObject* hero = nullptr;
	game::GameObject* arm = nullptr;
	game::GameObject* sword = nullptr;

	game::Transform* heroTransform = nullptr;
	game::Transform* armTransform = nullptr;
	game::Transform* swordTransform = nullptr;

	size_t updateCount = 0;
};

#endif // TEST_GAME_H