/* GameObject.cpp
*/

#include "GameObject.h"

namespace game {
	GameObject::GameObject( BitBladeGame* game, const ImageSource* imageSource )
		: game( game ), position()
	{
		sprite = nullptr;
		game = nullptr;
	}

	GameObject::~GameObject()
	{
	}


	void GameObject::update()
	{
	}
}