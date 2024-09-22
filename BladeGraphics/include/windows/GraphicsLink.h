/* GraphicsLink.h
*/

#ifndef GRAPHICS_LINK_H
#define GRAPHICS_LINK_H

#include "GfxLinkCommon.h"

#include <Windows.h>
#include <mutex>
#include <atomic>
#include <future>

class GraphicsLink {
public:
	GraphicsLink();
	~GraphicsLink();

	void PackInstruction( gfxLink::GfxCode functionCode, const char* data, uint16_t length );
	const char* GetGraphicsInstructions();

	void SendGraphicsStartupEvent(); // reuses resolve objects received irq (blocking)
	void SendResolvedGraphicsObjects();
	void AwaitConsoleInstructionsReceivedSignal();
	void AwaitConsoleFinishedResolvingObjectsSignal();
	void SignalGraphicsFinishedProcessing();

private:

	static constexpr LPCSTR graphicsOutputFileName = "BitBladeGraphicsOutputBuffer";
	static constexpr LPCSTR consoleOutputFileName = "BitBladeConsoleOutputBuffer";

	// buffers
	char* packedInstructions;	// double buffer for sending graphics update
	char* graphicsOutputBuffer;	// given packed instructions when finished
	char* consoleOutputBuffer;	// resolved objects received
	uint16_t currentPosition;		// Position tracker for writing to the buffer

	// memory mapped files
	HANDLE hGraphicsOutputBuffer;
	HANDLE hConsoleOutputBuffer;

	// events
	HANDLE hfinishedConsoleInstructionTransferSignal;	// sent by console gpio (catch)
	HANDLE hfinishedConsoleResolvedObjectsFinishSignal;	// sent by console gpio (catch)
	HANDLE hGraphicsFinishedProcessingSignal;				// sent by graphics gpio  (send)
	HANDLE hGraphicsResolvedObjectSendFinishSignal;			// sent by graphics gpio  (send)

	bool isInstructionsReceived;
	std::mutex mtxInstructionsReceived;
	std::future<void> futureInstructionsReceivedListener;

	bool isConsoleObjectResolveComplete;
	std::mutex mtxConsoleObjectResolve;
	std::future<void> futureConsoleObjectsResolvedListener;

	std::atomic<bool> linkStopSignal;

	HANDLE CreateOrConnectEvent( const char* eventName );
	void CreateOrOpenMemoryMap( const LPCSTR& test, HANDLE& handleOut, char*& bufferOut );

	void irqHandlerOnObjectsTransferFinish();
	void irqHandlerOnInstructionsReceivedSignal();
	void irqHandlerOnConsoleResolvedFinishedSignal();

	// simulate gpio pins with events
	void gpioSignalFinishedProcessingGraphics();	  // triggered by main loop
	void gpioSignalFinishedResolvedObjectsTransfer(); // triggered by DMA

	// simulated irq triggering
	// listen or start in another thread to simulate non blocking irq signalling
	void triggerResolvedObjectsTransferFinishDmaIrqAsync();						// simulate triggering irq handler
	std::future<void> triggerListenerInstructionsReceivedGpioIrqAsync();
	std::future<void> triggerListenerConsoleResolvedObjectsGpioIrqAsync();
};

#endif // GRAPHICS_LINK_H

