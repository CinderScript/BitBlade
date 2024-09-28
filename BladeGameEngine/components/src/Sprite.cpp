#include "Sprite.h"
/* Sprite.cpp
*/

namespace game {

	Sprite::Sprite( GameObject* owner, const ImageSource* imageSource )
		: Component( owner ),
		image( imageSource ),
		pivot( imageSource->Pivot() ),
		scale( imageSource->Scale() ) {}

}