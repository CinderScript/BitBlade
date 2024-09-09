/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"

BitBladeGame::~BitBladeGame()
{
	for (auto* prefab : prefabs) {
		delete prefab;
	}
	for (auto* gameObject : gameObjects) {
		delete gameObject;
	}
}


Prefab* BitBladeGame::LoadPrefab(const char* filename) {
	Prefab* prefab = new Prefab();
	prefabs.push_back(prefab);
	return prefab;
}

GameObject* BitBladeGame::CreateInstance(const Prefab* prefab) {
	GameObject* gameObject = new GameObject();
	gameObjects.push_back(gameObject);
	return gameObject;
}


void BitBladeGame::Tick() {
	for (auto* gameObject : gameObjects) {
		gameObject->Tick();
	}
}