/* ConsoleLink.h
*/

#ifndef CONSOLE_LINK_H
#define CONSOLE_LINK_H

#include "BladeLinkCommon.h"
class BladeConsole;  // Forward declaration

#include <Windows.h>
#include <mutex>

class ConsoleLink {
public:
	using ProcessMessageCallback = void (BladeConsole::*)(const char*);

	ConsoleLink();
	~ConsoleLink();

	void PackInstruction(char functionCode, const char* data, size_t length);
	void SendGraphicsInstructions();
	void WaitForGraphicsReadySignal();
	void WaitForGraphicsStartupEvent(); // reuses resolve objects received irq (blocking)

	void SetOnResolvedObjectsReceivedHandler(BladeConsole* console, ProcessMessageCallback callback);

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
	bool isGraphicsReady;

	// Pointers to store the object and the member function
	BladeConsole* bladeConsole;
	ProcessMessageCallback onMessageReceivedHandler;

	HANDLE CreateOrConnectEvent(const char* eventName);
	void CreateOrOpenMemoryMap(const LPCSTR& test, HANDLE& handleOut, char* bufferOut);

	void irqHandlerOnConsoleTransferFinish();
	void irqHandlerOnResolvedObjectsReceived();
	void irqHandlerOnGraphicsReady();

	// simulated irq triggering
	// listen or start in another thread to simulate non blocking irq signalling
	void triggerConsoleTransferFinishDmaIrqAsync();						// simulate triggering irq handler
	void triggerListenerResolvedObjectsReceivedGpioIrqBlocking();				// simulate triggering irq handler
	void triggerListenerGraphicsReadyGpioIrqAsync();							// simulate triggering irq handler

	void gpioSignalFinishedProcessingResolvedObjects();				// Graphics result resolved
	void gpioSignalFinishedConsoleTransfer();
};

#endif // CONSOLE_LINK_H

