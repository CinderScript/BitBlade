/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"

// Define the static members
vector<Prefab> BitBladeGame::prefabs;
vector<GameObject> BitBladeGame::gameObjects;

BitBladeGame::~BitBladeGame() { }

Prefab& BitBladeGame::LoadPrefab( const char* filename ) {
	prefabs.emplace_back( filename );
	return prefabs.back();
}

GameObject& BitBladeGame::CreateInstance( const Prefab* prefab ) {
	// Create a GameObject and add it to the vector
	gameObjects.emplace_back( *prefab );
	return gameObjects.back();
}


void BitBladeGame::Tick() {
	for (auto& gameObject : gameObjects) {
		gameObject.Tick();
	}
}