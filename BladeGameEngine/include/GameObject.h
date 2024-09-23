/* GameObject.h
*/

#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "IBladeGame.h"
#include "DataPoolMember.h"
#include "Vector2.h"

#include <vector>

namespace game {
	template<typename T>
	class DataPool;
	class Component;
	class ImageSource;
	class Sprite;

	class GameObject : public DataPoolMember {

	public:
		friend struct DataPool<GameObject>;			// calls constructor

		~GameObject();

		Vector2 Position() const { return position; }
		void SetPosition( const Vector2& Position ) { position = Position; }

		template<typename T, typename... Args>
		T* AddComponent( Args&&... args )
		{
			if constexpr (std::is_same<T, Sprite>::value)
			{
				// Special case for Sprite with ImageSource as an argument
				auto* sprite = game->AddComponent<Sprite>( std::forward<Args>( args )... );
				components.push_back( sprite );
				return sprite;
			}
			else
			{
				// General case for all other components
				auto* comp = game->AddComponent<T>( std::forward<Args>( args )... );
				components.push_back( comp );
				return comp;
			}
		}

	private:
		IBladeGame* game;
		GameObject* parent;
		Sprite* sprite;
		Vector2 position;

		std::vector<GameObject*> children;
		std::vector<Component*> components;

		GameObject( IBladeGame* game, GameObject* parent );
		void internalUpdate();
	};
}

#endif // GAME_OBJECT_H