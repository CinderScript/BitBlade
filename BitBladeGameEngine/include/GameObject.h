/* GameObject.h
*/

#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "Prefab.h"

class BitBladeGame;

class GameObject {
public:
	friend class BitBladeGame;

	GameObject( const Prefab& prefab );
	~GameObject();

private:
	void Tick();

	const Prefab& prefab;
};

#endif // GAME_OBJECT_H