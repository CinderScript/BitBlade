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
#include "BitBladeCommon.h"

#include <iostream>

using std::string;

using gfxLink::GfxCommand;
using gfxLink::readMessageBuffer;
using gfxLink::readMessageBufferString;
using gfxLink::toGfxCommand;

BladeGraphics::BladeGraphics()
{
	// STARTUP SEQUENCE
	link.SendGraphicsStartupEvent();
}

BladeGraphics::~BladeGraphics() {}


void BladeGraphics::StartGraphics()
{
	link.AwaitConsoleInstructionsReceivedSignal();

	ProcessGraphics();

	// Console has no resolved objects yet (don't wait for signal)

	link.SignalGraphicsFinishedProcessing();

	link.SendResolvedGraphicsObjects(); // triggers irq on finish
	// dma irq sends finish sending event
}

void BladeGraphics::UpdateGraphics()
{
	// this dma interrupt signals console after resolved objects send finishes

	link.AwaitConsoleInstructionsReceivedSignal();

	ProcessGraphics();

	link.AwaitConsoleFinishedResolvingObjectsSignal();

	link.SignalGraphicsFinishedProcessing();

	link.SendResolvedGraphicsObjects(); // triggers irq on finish
	// dma irq sends finish sending event	
}


void BladeGraphics::ProcessGraphics()
{
	uint16_t pos = 0;
	const char* buffer = link.GetGraphicsInstructions();

	GfxCommand cmd;

	while (buffer[pos] != +GfxCommand::End)
	{
		cmd = toGfxCommand( buffer[pos++] );

		switch (cmd) {

		case GfxCommand::CreateImageData:
		{
			uint16_t imageDataID;
			char filename[gfxLink::PACKED_COMMAND_MAX_SIZE];

			readMessageBuffer( buffer, imageDataID, pos );
			readMessageBufferString( buffer, filename, pos );

			std::cout << "Create Image Data: " << imageDataID << ",  \'" << filename << "\'\n";

			break;
		}
		case GfxCommand::CreateSpriteInstance:
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
		default:
			break;
		}
	}
}