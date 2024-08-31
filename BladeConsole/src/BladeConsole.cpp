/* BladeConsole.cpp
*
* Updates the game loop
* Keeps track of each object
* Updates objects positions and rotations
* Sets objects height visibility
*/

#include "BladeConsole.h"
#include "BladeLink.h"

BladeConsole::BladeConsole() : bladeLink(std::make_unique<BladeLink>(false))
{
	// GraphicsConsole sends ready message when bladeLink is setup and listening for our ready signal
	bladeLink->WaitForConnectedThreadReady();
}

BladeConsole::~BladeConsole()
{
	// dispose of the bladeLink
}

void BladeConsole::UpdateConsole()
{
	// resolve GameObjects or Prefabs with BladeGraphics if necessary
	processGraphicsMessage(bladeLink->GetBladeMessage());

	// process game engine and build instruction message
	// wait 1 second to simulate work
	// gameEngine.Update();

	bladeLink->SendBladeMessage(testMessage);
	bladeLink->SignalThisThreadReady();

	bladeLink->WaitForConnectedThreadReady();
}

void BladeConsole::processGraphicsMessage(const char* message)
{
	//std::cout << "BladeGraphics: ";
	//for (size_t i = 0; i < message.size() && i < 5; ++i) {
	//	if (i > 0) std::cout << ", ";
	//	std::cout << message[i];
	//}
	//std::cout << "\n";
}
