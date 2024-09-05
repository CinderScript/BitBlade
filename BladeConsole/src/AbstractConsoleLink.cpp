/* AbstractConsoleLink.cpp
*/

#include "AbstractConsoleLink.h"



void AbstractConsoleLink::SetOnResolvedObjectsReceivedHandler(OnMessageReceivedEvent handleResolvedObjects)
{
	handleResolvedObjects_ = handleResolvedObjects;
}


void AbstractConsoleLink::handleResolvedObjects(const char* message)
{
	handleResolvedObjects_(message);
	gpioSignalFinishedProcessingResolvedObjects();
}
