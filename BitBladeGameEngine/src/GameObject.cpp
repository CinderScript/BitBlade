/* GameObject.cpp
*/

#include "GameObject.h"

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