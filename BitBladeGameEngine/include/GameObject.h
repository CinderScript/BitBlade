/* GameObject.h
*/

#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "ImageSource.h"
#include "Sprite.h"
#include "Vector2.h"

class BitBladeGame;

class GameObject {
public:
	// required for BitBladeGame create a new GameObject
	friend class BitBladeGame;

	GameObject( const ImageSource* sprite );
	~GameObject();

	Vector2 Position() const { return position; }
	void SetPosition( const Vector2& Position ) { position = Position; }

private:
	Sprite sprite;
	Vector2 position;

	void update();
};

#endif // GAME_OBJECT_H