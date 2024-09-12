/* BitBladeGame.h
*/

#ifndef BIT_BLADE_GAME_H 
#define BIT_BLADE_GAME_H

#include "GameObject.h"
#include "Prefab.h"

#include <vector>
using std::vector;

// Forward declare BladeConsole to make it a friend of BitBladeGame
namespace console {
	class BladeConsole;  // Forward declaration within the namespace
}

class BitBladeGame {
public:
	friend class console::BladeConsole;

	virtual ~BitBladeGame();

protected:

	static Prefab& LoadPrefab( const char* filename );
	static GameObject& CreateInstance( const Prefab* prefab );

	virtual void GameStart() {}
	void LoadNewLevel() {}

private:
	static vector<Prefab> prefabs;
	static vector<GameObject> gameObjects;


	static void Tick();
};

#endif // BIT_BLADE_GAME_H