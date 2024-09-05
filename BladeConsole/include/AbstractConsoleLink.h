/* AbstractConsoleLink.h
*/

#ifndef ABSTRACT_CONSOLE_LINK_H
#define ABSTRACT_CONSOLE_LINK_H

#include "BladeLink.h"

class AbstractConsoleLink {
public:
	virtual ~AbstractConsoleLink() {}

	void SetOnResolvedObjectsReceivedHandler(OnMessageReceivedEvent handleResolvedObjects);

	virtual void WaitForGraphicsReadySignal() = 0;
	virtual void PackInstruction(char functionCode, const char* data, size_t length) = 0;
	virtual void SendGraphicsInstructions() = 0;

protected:
	OnMessageReceivedEvent handleResolvedObjects_;
	void handleResolvedObjects(const char* message);


	virtual void gpioSignalFinishedProcessingResolvedObjects() = 0;
	virtual void finishedConsoleInstructionTransferSignal() = 0;
};

#endif // ABSTRACT_CONSOLE_LINK_H