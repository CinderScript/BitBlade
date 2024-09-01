/* BladeGraphics.cpp
*/

#include "BladeGraphics.h"
#include "BladeLink.h"
#include "GfxCommand.h"
#include "BladeLinkBufferHelper.h"


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
	// send message containing resolved sprites
	bladeLink->SendBladeMessage();

	// wait until Console sends a Ready signal
	bladeLink->WaitForConnectedThreadReady();

	// signal ready before performing graphics update so Console can begin 
	// processing the new frame while BladeGraphics works concurrently
	bladeLink->SignalThisThreadReady();
	processConsoleMessage(bladeLink->GetBladeMessage());

	system("pause");
}


void BladeGraphics::sendGraphicsMetadata()
{
	//bladeLink->SendBladeMessage(testMessage);
}


void BladeGraphics::processConsoleMessage(const char* message)
{
	size_t pos = 0;
	const char* buffer = bladeLink->GetBladeMessage();

	GfxCommand cmd;

	while (buffer[pos] != +GfxCommand::End)
	{
		cmd = toGfxCommand(buffer[pos++]);

		switch (cmd) {
		case GfxCommand::CreateMasterSprite:
		{
			uint16_t x, y;
			uint32_t address;
			readFromBuffer(buffer, x, pos);
			readFromBuffer(buffer, y, pos);
			readFromBuffer(buffer, address, pos);

			std::cout << "x: " << x << "y: " << y << "address: " << address << "\n";

			break;
		}
		}
	}
}