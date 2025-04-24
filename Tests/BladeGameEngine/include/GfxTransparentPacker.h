#ifndef GFX_TRANSPARENT_PACKER_H
#define GFX_TRANSPARENT_PACKER_H

#include "IGfxMessagePacker.h"
#include "GfxLinkCommon.h"
#include "BladeConfig.h"


/// @brief Provides a way for objects that need to AddPackedInstructions from BitBladeGame
/// can simulate the method call and be tested by viewing the packed instructions and position
class GfxTransparentPacker : public gfxLink::IGfxMessagePacker
{

public:
	inline GfxTransparentPacker() : currentPosition( 0 )
	{
		packedInstructions = new char[gfxLinkConfig::MESSAGE_BUFFER_LENGTH]();
	}
	inline ~GfxTransparentPacker() {}

	inline void AddPackedInstruction(
		gfxLink::GfxCode functionCode,
		const char* appendData,
		uint16_t length ) override {
		gfxLink::packGfxInstruction(
			packedInstructions, functionCode, appendData, length, currentPosition );
	}

	uint16_t currentPosition;    // Position tracker for writing to the buffer
	char* packedInstructions;  // double buffer for sending graphics update

};

#endif // GFX_TRANSPARENT_PACKER_H