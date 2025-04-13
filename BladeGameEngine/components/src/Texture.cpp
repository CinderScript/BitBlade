#include "Texture.h"
/* Texture.cpp
*/

#include "IGfxMessageProducer.h"
#include "ImageSource.h"
#include "GameObject.h"

namespace game {

	Texture::Texture( GameObject* owner, const ImageSource* imageSource )
		: Component( owner ),
		image( imageSource ),
		pivot( imageSource->Pivot() ),
		scale( imageSource->Scale() )
	{
		messageProducer = owner->GetMessageProducer();
	}

}