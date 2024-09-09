/* BitBladeGame.h
*/

#ifndef BIT_BLADE_GAME_H 
#define BIT_BLADE_GAME_H

#include "GameObject.h"
#include "Prefab.h"

#include <vector>
using std::vector;

// Forward declare BladeConsole to make it a friend of BitBladeGame
class BladeConsole;

class BitBladeGame {
public:
	friend class BladeConsole;

	virtual ~BitBladeGame();

protected:

	Prefab* LoadPrefab(const char* filename);
	GameObject* CreateInstance(const Prefab* prefab);

	virtual void GameAwake() {}
	virtual void GameStart() {}

private:
	vector<Prefab*> prefabs;
	vector<GameObject*> gameObjects;

	void Tick();
};

#endif // BIT_BLADE_GAME_H