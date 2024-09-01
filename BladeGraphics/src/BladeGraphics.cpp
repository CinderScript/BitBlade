/* BladeGraphics.cpp
*/

#include "BladeGraphics.h"
#include "BladeLink.h"


BladeGraphics::BladeGraphics() : bladeLink(std::make_unique<BladeLink>(true))
{
	// the link has been set up and now we need to let Console know that it can begin sending
	bladeLink->SignalThisThreadReady();
}

BladeGraphics::~BladeGraphics()
{
	// dispose of the bladeLink
}

void BladeGraphics::UpdateGraphics()
{
	// wait until Console sends a Ready signal
	bladeLink->WaitForConnectedThreadReady();

	// resolve GameObjects or Prefabs with BladeGraphics if necessary
	processConsoleMessage(bladeLink->GetBladeMessage());

	// perform all Loading and instancing of sprites
	// send back resolved metadata for MasterSpriteData and SpriteInstance

	// signal ready before performing graphics update so Console can begin 
	// processing the new frame while BladeGraphics works concurrently
	bladeLink->SignalThisThreadReady();

	// Process Graphical Updates
	// graphics.Update();

	system("pause");
}


void BladeGraphics::sendGraphicsMetadata()
{
	//bladeLink->SendBladeMessage(testMessage);
}

void BladeGraphics::processConsoleMessage(const char* message)
{

}