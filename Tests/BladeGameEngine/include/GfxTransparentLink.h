#ifndef GFX_TRANSPARENT_PACKER_H
#define GFX_TRANSPARENT_PACKER_H

#include "IGfxMessageLink.h"
#include "GfxLinkCommon.h"
#include "BladeConfig.h"


/// @brief Provides a way for objects that need to AddPackedInstructions from BitBladeGame
/// can simulate the method call and be tested by viewing the packed instructions and position
class GfxTransparentLink : public gfxLink::IGfxMessageLink
{

public:
	inline GfxTransparentLink()
	{
		packedInstructions = new char[gfxLinkConfig::MESSAGE_BUFFER_LENGTH]();
	}

	uint16_t CurrentPosition() const { return currentPosition; }
	char* PackedInstructions() const { return packedInstructions; }
};

#endif // GFX_TRANSPARENT_PACKER_H