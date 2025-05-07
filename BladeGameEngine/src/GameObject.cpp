/* GameObject.cpp
*/

#include "GameObject.h"
#include "BladeConfig.h"
#include "BitBladeGame.h"
#include "Component.h"

#include <cassert>  // Make sure to include this

namespace game {
	GameObject::GameObject( uint16_t poolID, uint16_t objectID, BitBladeGame* game )
		: DataPoolMember( poolID, objectID ),
		game( game ), name( "unnamed" ), parent( nullptr )
	{
		initialize();
	}
	GameObject::GameObject( uint16_t poolID, uint16_t objectID, BitBladeGame* game, const char* name )
		: DataPoolMember( poolID, objectID ),
		game( game ), name( name ? name : "unnamed" ), parent( nullptr )
	{
		initialize();
	}
	GameObject::GameObject( uint16_t poolID, uint16_t objectID, BitBladeGame* game, GameObject* parent )
		: DataPoolMember( poolID, objectID ),
		game( game ), name( "unnamed" ), parent( parent )
	{
		initialize();
		assert( parent != nullptr );
		parent->children.push_back( this );
	}
	GameObject::GameObject( uint16_t poolID, uint16_t objectID, BitBladeGame* game, GameObject* parent, const char* name )
		: DataPoolMember( poolID, objectID ),
		game( game ), name( name ? name : "unnamed" ), parent( parent )
	{
		initialize();
		assert( parent != nullptr );
		parent->children.push_back( this );
	}

	GameObject::~GameObject() {}

	/// @brief Sets up the basic GameObject with a parent, children, and transform. Invoked by the Constructor
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
		//
		// Set new parent
		// set object's transform's parent

		// If the new parent is the same as the current parent, do nothing.
		if (this->parent == newParent) {
			return;
		}

		// 1. If a parent existed, remove this object from old parent's children.
		if (this->parent != nullptr) {
			auto& currentParentChildren = this->parent->children;
			currentParentChildren.erase(
				std::remove( currentParentChildren.begin(), currentParentChildren.end(), this ),
				currentParentChildren.end()
			);
		}
		// 2. If the old parent was null, this was a top-level object.
		//    Remove it from the topLevelObjects in the game.
		else {
			auto& topObjects = game->topLevelObjects;
			topObjects.erase(
				std::remove( topObjects.begin(), topObjects.end(), this ),
				topObjects.end()
			);
		}

		// A. If the new parent exists, add this object to the new parent's children.
		if (newParent != nullptr) {
			newParent->children.push_back( this );
		}
		// B. If the new parent is null, add this object to the topLevelObjects.
		else {
			game->topLevelObjects.push_back( this );
		}

		// Set the new parent GameObject and this GO's Transform's parent
		this->parent = newParent;
		transform->parent = newParent->transform; // a GO should always have a transform
	}
}