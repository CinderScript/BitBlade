/* GameObject.h
*/

#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "BitBladeGame.h"
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
		friend class BitBladeGame;					// calls internalUpdate

		~GameObject();

		Vector2 Position() const { return position; }
		void SetPosition( const Vector2& Position ) { position = Position; }
		GameObject* Parent() const { return parent; }
		void SetParent( GameObject* parent );


		template<typename T, typename... Args>
		T* AddComponent()
		{
			// General case for all other components
			auto* comp = game->AddComponent<T>( this );
			components.push_back( comp );
			startComponents.push_back( comp );
			comp->Awake();

			if constexpr (std::is_same<T, Sprite>::value)
				sprite = comp;

			return comp;
		}

		template <typename T>
		T* GetComponent() const {
			for (auto* comp : components) {
				if (auto* desired = dynamic_cast<T*>(comp)) {
					return desired;
				}
			}
			return nullptr;
		}

		// template <typename T>
		// std::vector<T*> GetComponents() const {
		// 	std::vector<T*> desired;
		// 	for (auto* comp : components) {
		// 		if (auto* desired = dynamic_cast<T*>(comp)) {
		// 			desired.push_back( desired );
		// 		}
		// 	}
		// 	return desired;
		// }


		const std::vector<GameObject*>& GetChildren() const { return children; }
		const std::vector<Component*>& GetComponents() const { return components; }

		const char* Name() const { return name; }

	private:
		BitBladeGame* game;

		GameObject* parent;
		const char* name;
		Sprite* sprite;
		Vector2 position;

		std::vector<GameObject*> children;
		std::vector<Component*> components;
		std::vector<Component*> startComponents;

		GameObject( BitBladeGame* game );
		GameObject( BitBladeGame* game, const char* name );
		GameObject( BitBladeGame* game, GameObject* parent );
		GameObject( BitBladeGame* game, GameObject* parent, const char* name );
		void internalUpdate();
	};
}

#endif // GAME_OBJECT_H