/* GameObject.h
*/

#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "BitBladeGame.h"
#include "Transform.h"

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

		Transform* GetTransform() const { return transform; }

		GameObject* Parent() const { return parent; }
		void SetParent( GameObject* parent );


		template<typename T, typename... Args>
		T* AddComponent( Args&&... args )
		{
			// General case for all other components
			auto* comp = game->AddComponent<T>( this, std::forward<Args>( args )... );
			components.push_back( comp );
			startComponents.push_back( comp );
			comp->Awake();

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

		template <typename T>
		std::vector<T*> GetComponents() const {
			std::vector<T*> desired;
			for (auto* comp : components) {
				if (T* castedComp = dynamic_cast<T*>(comp)) {
					desired.push_back( castedComp );
				}
			}
			return desired;
		}


		const std::vector<GameObject*>& GetChildren() const { return children; }
		const std::vector<Component*>& GetComponents() const { return components; }

		const char* Name() const { return name; }

	private:
		BitBladeGame* game;
		GameObject* parent;
		const char* name;

		Transform* transform;

		std::vector<GameObject*> children;
		std::vector<Component*> components;
		std::vector<Component*> startComponents;

		GameObject( BitBladeGame* game );
		GameObject( BitBladeGame* game, const char* name );
		GameObject( BitBladeGame* game, GameObject* parent );
		GameObject( BitBladeGame* game, GameObject* parent, const char* name );

		// Disable copy constructor and copy assignment operator
		GameObject( const GameObject& ) = delete;
		GameObject& operator=( const GameObject& ) = delete;

		// Disable move constructor and move assignment operator
		GameObject( GameObject&& ) = delete;
		GameObject& operator=( GameObject&& ) = delete;

		void internalUpdate();
		void initialize();
	};
}

#endif // GAME_OBJECT_H