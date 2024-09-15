/* GameObject.cpp
*/

#include "GameObject.h"

namespace game {
	GameObject::GameObject( const ImageSource* imageSource )
		: sprite( imageSource ), position()
	{
	}

	GameObject::~GameObject()
	{
	}


	void GameObject::update()
	{
	}
}