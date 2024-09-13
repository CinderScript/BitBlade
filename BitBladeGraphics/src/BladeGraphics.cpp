/* BladeGraphics.cpp
*/

#include "BladeGraphics.h"
#include "GraphicsLink.h"
#include "BitBladeCommon.h"

#include <iostream>

using std::string;

using bladeLinkCommon::GfxCommand;
using bladeLinkCommon::readMessageBuffer;
using bladeLinkCommon::toGfxCommand;

BladeGraphics::BladeGraphics()
{
	// STARTUP SEQUENCE
	link.SendGraphicsStartupEvent();
}

BladeGraphics::~BladeGraphics() {}


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


	system( "pause" );
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
		case GfxCommand::CreateMasterSprite:
		{
			uint16_t x, y;
			uint32_t spriteAddress;
			readMessageBuffer( buffer, x, pos );
			readMessageBuffer( buffer, y, pos );
			readMessageBuffer( buffer, spriteAddress, pos );

			std::cout << "x: " << x << "y: " << y << "address: " << spriteAddress << "\n";

			break;
		}

		default:
			break;
		}
	}
}