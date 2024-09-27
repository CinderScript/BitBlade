#ifndef GFXTESTPACKER_TEST_H
#define GFXTESTPACKER_TEST_H




#include "IGfxMessagePacker.h"
#include "GfxLinkCommon.h"
#include "BladeConfig.h"

class GfxTestPacker : public IGfxMessagePacker
{

public:
	GfxTestPacker();
	~GfxTestPacker();

	void AddPackedInstruction(
		gfxLink::GfxCode functionCode,
		const char* data,
		uint16_t length ) override;

	uint16_t currentPosition;    // Position tracker for writing to the buffer
	char* packedInstructions;  // double buffer for sending graphics update

};

GfxTestPacker::GfxTestPacker() : currentPosition( 0 )
{
	packedInstructions = new char[gfxLinkConfig::MESSAGE_BUFFER_LENGTH]();
}

GfxTestPacker::~GfxTestPacker()
{
}

void GfxTestPacker::AddPackedInstruction(
	gfxLink::GfxCode functionCode, const char appendData[], uint16_t length )
{
	gfxLink::packGfxInstruction(
		packedInstructions, functionCode, appendData, length, currentPosition );
}


#endif // GFXTESTPACKER_TEST_H