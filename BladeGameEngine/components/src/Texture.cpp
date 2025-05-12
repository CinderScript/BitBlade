/* Texture.cpp
*/

#include "Texture.h"
#include "BladeConfig.h"
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

	uint16_t Texture::gfx_CreateSprite()
	{
		uint16_t pos = 0;
		char message[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];

		// copy ID

		return 0;
	}

}