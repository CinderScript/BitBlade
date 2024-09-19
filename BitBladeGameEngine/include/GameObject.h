/* GameObject.h
*/

#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "ImageSource.h"
#include "Vector2.h"

#include <vector>

namespace game {
	template<typename T>
	class DataPool;
	class BitBladeGame;
	class Component;
	class Sprite;

	class GameObject : public PoolObject {

	public:
		friend struct DataPool<GameObject>;
		friend class BitBladeGame;				// needs Pack functions

		~GameObject();

		Vector2 Position() const { return position; }
		void SetPosition( const Vector2& Position ) { position = Position; }

		template<typename T>
		bool AddComponent() {}

	private:
		Vector2 position;
		Sprite* sprite;
		BitBladeGame* game;
		std::vector<Component*> components;

		GameObject( BitBladeGame* game, const ImageSource* imageSource );
		void update();
	};
}

#endif // GAME_OBJECT_H