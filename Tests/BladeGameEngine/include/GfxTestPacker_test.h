#ifndef GFXTESTPACKER_TEST_H
#define GFXTESTPACKER_TEST_H




#include "IGfxMessagePacker.h"
#include "GfxLinkCommon.h"
#include "BladeConfig.h"

class GfxTestPacker : public IGfxMessagePacker
{

public:
	inline GfxTestPacker() : currentPosition( 0 )
	{
		packedInstructions = new char[gfxLinkConfig::MESSAGE_BUFFER_LENGTH]();
	}
	inline ~GfxTestPacker() {}

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

#endif // GFXTESTPACKER_TEST_H