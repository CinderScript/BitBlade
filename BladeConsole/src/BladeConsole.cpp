/* BladeConsole.cpp
*
* Ticks the game loop
* Keeps track of each object
* Ticks objects positions and rotations
* Sets objects height visibility
*/

#include "BladeConsole.h"
#include "BladeLinkCommon.h"

BladeConsole::BladeConsole() : bladeLink(std::make_unique<ConsoleLink>())
{
	//// STARTUP SEQUENCE
	bladeLink->SetOnResolvedObjectsReceivedHandler(this, &BladeConsole::resolvedObjectsReceivedHandler);
	bladeLink->WaitForGraphicsStartupConnection();
}

BladeConsole::~BladeConsole() {}


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
