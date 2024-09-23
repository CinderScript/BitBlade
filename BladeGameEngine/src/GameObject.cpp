/* GameObject.cpp
*/

#include "GameObject.h"

namespace game {
	GameObject::GameObject( IBladeGame* game, GameObject* parent )
		: game( game ), parent( parent ), sprite( nullptr ), position() {}

	GameObject::~GameObject() {}


	void GameObject::internalUpdate()
	{
	}
}