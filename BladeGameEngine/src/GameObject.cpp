/* GameObject.cpp
*/

#include "GameObject.h"
#include "BladeConfig.h"
#include "BitBladeGame.h"
#include "Component.h"

#include <cassert>  // Make sure to include this

namespace game {
	GameObject::GameObject( BitBladeGame* game )
		: game( game ), name( "unnamed" ), parent( nullptr )
	{
		initialize();
	}
	GameObject::GameObject( BitBladeGame* game, const char* name )
		: game( game ), name( name ? name : "unnamed" ), parent( nullptr )
	{
		initialize();
	}
	GameObject::GameObject( BitBladeGame* game, GameObject* parent )
		: game( game ), name( "unnamed" ), parent( parent )
	{
		initialize();
		assert( parent != nullptr );
		parent->children.push_back( this );
	}
	GameObject::GameObject( BitBladeGame* game, GameObject* parent, const char* name )
		: game( game ), name( name ? name : "unnamed" ), parent( parent )
	{
		initialize();
		assert( parent != nullptr );
		parent->children.push_back( this );
	}

	GameObject::~GameObject() {}

	void GameObject::initialize() {
		children.reserve( gameConfig::GAME_OBJECT_CHILD_RESERVE );
		components.reserve( gameConfig::GAME_OBJECT_COMPONENT_RESERVE );
		startComponents.reserve( gameConfig::GAME_OBJECT_COMPONENT_RESERVE );

		transform = AddComponent<Transform>();
	}

	void GameObject::SetParent( GameObject* newParent ) {

		// 1 this was not a top level object, parent exists
		//		A. new parent is not null
		//				- remove this child from old parent
		//				- set this parent to the new parent
		//				- add this child to the new parent
		//		B. new parent is null
		//				- remove this child from old parent
		//				- set this parent to nullptr
		//				- add to top level objects

		// 2 this was a top level object, parent was null
		//		A. new parent is not null
		//				- remove from top level
		//				- add new parent
		//				- make child of new parent
		//		B. new parent is null
		//				- do nothing

	// If the new parent is the same as the current parent, do nothing.
		if (this->parent == newParent) {
			return;
		}

		// 1. If the current parent exists, remove this object from its children.
		if (this->parent != nullptr) {
			auto& currentParentChildren = this->parent->children;
			currentParentChildren.erase(
				std::remove( currentParentChildren.begin(), currentParentChildren.end(), this ),
				currentParentChildren.end()
			);
		}
		// 2. If the current parent is null, this is a top-level object.
		//    Remove it from the topLevelObjects in the game.
		else {
			auto& topObjects = game->topLevelObjects;
			topObjects.erase(
				std::remove( topObjects.begin(), topObjects.end(), this ),
				topObjects.end()
			);
		}

		this->parent = newParent;

		// A. If the new parent exists, add this object to the new parent's children.
		if (newParent != nullptr) {
			newParent->children.push_back( this );
		}
		// B. If the new parent is null, add this object to the topLevelObjects.
		else {
			game->topLevelObjects.push_back( this );
		}
	}

	void GameObject::internalUpdate()
	{
		for (auto* components : startComponents) {
			components->Start();
		}

		startComponents.clear();

		for (auto* component : components) {
			component->Update();
		}
	}
}