/* ConsoleLink.h
*/

#ifndef CONSOLE_LINK_H
#define CONSOLE_LINK_H

#include "BladeLink.h"
#include <Windows.h>
#include <mutex>

class ConsoleLink {
public:
	ConsoleLink();
	~ConsoleLink();

	void PackInstruction(char functionCode, const char* data, size_t length);
	void SendGraphicsInstructions();
	void WaitForGraphicsReadySignal();
	void SetOnResolvedObjectsReceivedHandler(OnMessageReceivedEvent handleResolvedObjects);

private:
	static constexpr LPCSTR consoleOutputFileName = "BitBladeConsoleOutputBuffer";
	static constexpr LPCSTR graphicsOutputFileName = "BitBladeGraphicsOutputBuffer";

	// buffers
	char* packedInstructions;	// double buffer for sending graphics update
	char* outputMessageBuffer;	// given packed instructions when finished
	char* inputMessageBuffer;	// resolved objects received
	size_t currentPosition;		// Position tracker for writing to the buffer

	// memory mapped files
	HANDLE hOutputBufferHandle;
	HANDLE hInputBufferHandle;

	// events
	HANDLE hfinishedConsoleInstructionTransferSignal;	// sent by console gpio
	HANDLE hfinishedProcessingResolvedObjectsSignal;	// sent by console gpio

	HANDLE hGraphicsFinishedProcessingSignal;				// sent by graphics gpio (catch)
	HANDLE hGraphicsResolvedObjectSendFinishSignal;			// sent by graphics gpio (catch)

	std::mutex mtx;
	bool isGraphicsReady = false;

	OnMessageReceivedEvent handleResolvedObjects_;			// set by BladeConsole

	HANDLE CreateOrConnectEvent(const char* eventName);
	void CreateOrOpenMemoryMap(const LPCSTR& test, HANDLE& handleOut, char* bufferOut);

	void irqHandlerOnConsoleTransferFinish();
	void irqHandlerOnResolvedObjectsReceived();
	void irqHandlerOnGraphicsReady();

	// simulated irq triggering
	// listen or start in another thread to simulate non blocking irq signalling
	void triggerConsoleTransferFinishDmaIrqAsync();						// simulate triggering irq handler
	void listenForResolvedObjectsReceivedGpioIrqBlocking();				// simulate triggering irq handler
	void listenForGraphicsReadyGpioIrqAsync();								// simulate triggering irq handler

	void gpioSignalFinishedProcessingResolvedObjects();				// Graphics result resolved
	void gpioSignalFinishedConsoleTransferSignal();
public:

};

#endif // CONSOLE_LINK_H

