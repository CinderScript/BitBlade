/* BladeConsole.cpp
*
* Ticks the game loop
* Keeps track of each object
* Ticks objects positions and rotations
* Sets objects height visibility
*/

#include "BladeConsole.h"
#include "BladeLink.h"

BladeConsole::BladeConsole() : bladeLink(std::make_unique<ConsoleLink>())
{
	// STARTUP SEQUENCE
	//bladeLink->WaitForGraphicsProcessingFinish();
	//bladeLink->SetOnResolvedObjectsReceivedHandler(resolvedObjectsReceivedHandler);
}

BladeConsole::~BladeConsole() {}

//			CONSOLE									GRAPHICS
//	 START:
// TICK graphics 1
// (nothing to wait for)													
// send update: 1								
											//		START:
// (interrupt, signal) send 1 finished	-------> // (await signal) console send 1 finish
// TICK graphics 2								// graphics PROCSSES tick 1
// (block) for graphics finish			<------- // (signal) graphics process finished: 1
// send update: 2								// send result: A
//	UPDATE:									//		UPDATE:
// (interrupt, resolve, signal) result A	<-------	// (interrupt, signal) send A finish
// (interrupt, signal) send 2 finished	-------> // (await signal) console send 2 finish
// TICK graphics 3								// graphics PROCSSES tick 2	
// 		cont. resolve A processing		------->	// (await signal) result A resolved
// (block) for graphics finish			<------- // (signal) graphics process finished: 2
//		cont. resolve A processing				// send result: B
// send update: 3																	

// (interrupt, resolve, signal) result B	<-------	// (interrupt, signal) send B finish
// (interrupt, signal) send 3 finished	-------> // (await signal) console send 3 finish
// TICK graphics 4								// graphics PROCSSES tick 3	
//		cont. resolve B processing		------->	// (await signal) result B resolved
// (block) for graphics finish			<------- // (signal) graphics process finished: 3
//		cont. resolve B processing				// send result: C
// send update: 4								
// continue ...


void BladeConsole::StartConsole() {
	// perform first game tick
	// gameEngine.Tick();							// tick

	bladeLink->SendGraphicsInstructions();		// irq sends event when finished
}

void BladeConsole::UpdateConsole()
{
	// gameEngine.Tick();							// tick

	// blocks until: graphics gets frame before last -> graphics finishes, console resolves objects -> sends resolved event
	bladeLink->WaitForGraphicsReadySignal();

	bladeLink->SendGraphicsInstructions();		// irq sends event when finished
}

void BladeConsole::resolvedObjectsReceivedHandler(const char* message)
{
	// resolve addresses
}
