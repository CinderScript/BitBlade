/* GameObject.h
*/

#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H


class BitBladeGame;

class GameObject {
public:
	friend class BitBladeGame;

	GameObject();
	~GameObject();

private:
	void Tick();
};

#endif // GAME_OBJECT_H