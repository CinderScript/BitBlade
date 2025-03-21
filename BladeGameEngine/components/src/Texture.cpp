#include "Texture.h"
/* Texture.cpp
*/

namespace game {

	Texture::Texture( GameObject* owner, const ImageSource* imageSource )
		: Component( owner ),
		image( imageSource ),
		pivot( imageSource->Pivot() ),
		scale( imageSource->Scale() ) {}

}