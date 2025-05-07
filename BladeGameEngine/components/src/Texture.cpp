#include "Texture.h"
/* Texture.cpp
*/

#include "IGfxMessageProducer.h"
#include "ImageSource.h"
#include "GameObject.h"

namespace game {

	Texture::Texture( uint16_t poolID, uint16_t objectID, GameObject* owner, const ImageSource* imageSource )
		: Component( poolID, objectID, owner ),
		image( imageSource ),
		pivot( imageSource->Pivot() ),
		scale( imageSource->Scale() )
	{
		messageProducer = owner->GetMessageProducer();
	}

}