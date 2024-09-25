/* BladeConsole.cpp
 */

 //			CONSOLE									GRAPHICS
 // START:
 // TICK graphics 1
 // (nothing to wait for)
 // send update: 1

 // (interrupt, event) send 1 finished	-------> 	// (await signal) console send 1 finish
 // TICK graphics 2									// graphics PROCSSES tick 1
 // (await signal) for graphics finish	<------- 	// (signal) graphics process finished: 1
 // send update: 2									// send result: A

 // UPDATE:											// UPDATE:
 // (interrupt, resolve, event) result A	<------ // (interrupt, event) send A finish
 // (interrupt, event) send 2 finished	-------> 	// (await signal) console send 2 finish
 // TICK graphics 3									// graphics PROCSSES tick 2
 // 		cont. resolve A processing	------> 		// (await signal) result A resolved (will usually resolve before tick finishes)
 // (await signal) for graphics finish	<------- 	// (signal) graphics process finished: 2
 //		cont. resolve A processing					// send result: B
 // send update: 3

 // (interrupt, resolve, event) result B <-------	// (interrupt, event) send B finish
 // (interrupt, event) send 3 finished	 -------> 	// (await signal) console send 3 finish
 // TICK graphics 4									// graphics PROCSSES tick 3
 //		cont. resolve B processing		------->	// (await signal) result B resolved
 // (await signal) for graphics finish	<------- 	// (signal) graphics process finished: 3
 //		cont. resolve B processing					// send result: C
 // send update: 4
 // continue ...

#include "BladeConsole.h"

#include <iostream>

namespace console {

	BladeConsole::BladeConsole()
		: link( std::make_unique<ConsoleLink>() ),
		game( nullptr )
	{
		std::cout << "Console - Constructor." << "\n";

		link->WaitForGraphicsStartupEvent();
	}

	BladeConsole::~BladeConsole() {}

	bool BladeConsole::FirstUpdate()
	{
		std::cout << "Console - Update 1 Start." << "\n";

		// perform first game tick
		bool shouldContinue = game->internalUpdate();

		std::cout << "Console - Update 1 Finish. Sending Update 1." << "\n";

		link->SendGraphicsInstructions(); // irq when finished
		// dma irq sends finish sending event


		// allow Graphics to receive last code if quitting
		if (!shouldContinue) {
			return false;								/* * * EARLY OUT * * */
		}

		std::cout << "Console - Update 2 Start." << "\n";

		// tick 2
		shouldContinue = game->internalUpdate();

		std::cout << "Console - Update 2 Finish. Waiting for Graphics" << "\n";

		link->AwaitGraphicsReadySignal(); // wait for graphics to finish reading tick 1

		std::cout << "Console - Sending update 2" << "\n";

		link->SendGraphicsInstructions(); // send update 2

		return shouldContinue;
	}

	bool BladeConsole::Update()
	{
		// try to resolve objects from previous tick before calculating graphics
		if (link->HasReceivedResolvedObjects())
		{
			resolveGraphicsObjects();
			link->SignalObjectsResolvedComplete();
		}

		bool shouldContinue = game->internalUpdate();

		// wait for objects to resolve from tick before last so graphics can stop waiting
		if (link->HasReceivedResolvedObjects())
		{
			link->WaitForResolvedObjectsReceived();
			resolveGraphicsObjects();
			link->SignalObjectsResolvedComplete();
		}

		// blocks until: graphics gets frame before last -> graphics finishes, console resolves objects -> sends resolved event
		link->AwaitGraphicsReadySignal();

		link->SendGraphicsInstructions(); // irq when finished
		// dma irq sends finish sending event

		// allow Graphics to receive last code if quitting
		return shouldContinue;
	}


	void BladeConsole::resolveGraphicsObjects()
	{
		const char* message = link->GetReceivedResolvedObjectsInstructions();
	}
}