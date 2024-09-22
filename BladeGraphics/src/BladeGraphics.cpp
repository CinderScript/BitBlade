/* BladeGraphics.cpp
*/

//			CONSOLE									GRAPHICS
// START:
// TICK graphics 1
// (nothing to wait for)													
// send update: 1								

// (interrupt, event) send 1 finished	-------> // (await signal) console send 1 finish
// TICK graphics 2								// graphics PROCSSES tick 1
// (await signal) for graphics finish	<------- // (signal) graphics process finished: 1
// send update: 2								// send result: A

// (interrupt, resolve, event) result A	<-------	// (interrupt, event) send A finish
// (interrupt, event) send 2 finished	-------> // (await signal) console send 2 finish
// TICK graphics 3								// graphics PROCSSES tick 2	
// 		cont. resolve A processing		------->	// (await signal) result A resolved (will usually resolve before tick finishes)
// (await signal) for graphics finish	<------- // (signal) graphics process finished: 2
//		cont. resolve A processing				// send result: B
// send update: 3																	

// (interrupt, resolve, event) result B	<-------	// (interrupt, event) send B finish
// (interrupt, event) send 3 finished	-------> // (await signal) console send 3 finish
// TICK graphics 4								// graphics PROCSSES tick 3	
//		cont. resolve B processing		------->	// (await signal) result B resolved
// (await signal) for graphics finish	<------- // (signal) graphics process finished: 3
//		cont. resolve B processing				// send result: C	
// send update: 4								
// continue ...

#include "BladeGraphics.h"
#include "GraphicsLink.h"
#include "GfxLinkCommon.h"

#include <iostream>

using std::string;

using gfxLink::GfxCode;
using gfxLink::readMessageBuffer;
using gfxLink::readMessageBufferString;
using gfxLink::toGfxCommand;

BladeGraphics::BladeGraphics()
{
	// STARTUP SEQUENCE
	link.SendGraphicsStartupEvent();
}

BladeGraphics::~BladeGraphics() {}


bool BladeGraphics::FirstUpdate()
{
	link.AwaitConsoleInstructionsReceivedSignal();

	if (!ProcessGfxInstructions()) {
		return false;
	}
	// Console has no resolved objects yet (don't wait for signal)

	link.SignalGraphicsFinishedProcessing();

	link.SendResolvedGraphicsObjects(); // triggers irq on finish
	// dma irq sends finish sending event

	return true;
}

bool BladeGraphics::Update()
{
	// this dma interrupt signals console after resolved objects send finishes

	link.AwaitConsoleInstructionsReceivedSignal();

	if (!ProcessGfxInstructions()) {
		return false;
	}

	link.AwaitConsoleFinishedResolvingObjectsSignal();

	link.SignalGraphicsFinishedProcessing();

	link.SendResolvedGraphicsObjects(); // triggers irq on finish
	// dma irq sends finish sending event	

	return true;
}


bool BladeGraphics::ProcessGfxInstructions()
{
	uint16_t pos = 0;
	const char* buffer = link.GetGraphicsInstructions();

	GfxCode cmd;

	while (buffer[pos] != +GfxCode::EndMessage)
	{
		cmd = toGfxCommand( buffer[pos++] );

		switch (cmd) {

		case GfxCode::CreateImageData:
		{
			uint16_t imageDataID;
			char filename[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];
			readMessageBuffer( buffer, imageDataID, pos );
			readMessageBufferString( buffer, filename, pos );

			std::cout << "Create Image Data: " << imageDataID << ",  '" << filename << "'\n";
			break;
		}
		case GfxCode::CreateSpriteInstance:
		{
			uint8_t isImageDataResolved;
			uint16_t imageDataID;
			uint16_t spriteID;
			uint32_t spriteAddress;
			uint16_t x, y;
			readMessageBuffer( buffer, isImageDataResolved, pos );
			readMessageBuffer( buffer, imageDataID, pos );
			readMessageBuffer( buffer, spriteID, pos );
			readMessageBuffer( buffer, spriteAddress, pos );
			readMessageBuffer( buffer, x, pos );
			readMessageBuffer( buffer, y, pos );

			std::cout << "Sprite Instanced: " << spriteID << "(" << x << "," << y << ")" << "\n";
			break;
		}
		case GfxCode::StopGraphics:
		{
			char gameTitle[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];
			readMessageBufferString( buffer, gameTitle, pos );
			std::cout << "Game Stopped: " << gameTitle << "\n";

			return false;
		}
		default:
			std::cout << "Did not receive a valid GfxCode\n";
			break;
		}

	}
	return true;
}