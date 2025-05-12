/* ConsoleLink.h
 */

#ifndef CONSOLE_LINK_H
#define CONSOLE_LINK_H

#include "BladeConfig.h"
#include "IGfxMessageLink.h"

#include <Windows.h>
#include <atomic>
#include <future>
#include <mutex>

namespace console {

  class ConsoleLink : public gfxLink::IGfxMessageLink
  {
  public:
    ConsoleLink();
    ~ConsoleLink();

    void AddPackedInstruction( gfxLink::GfxCode functionCode, const char appendData[], uint16_t length );
    const char* GetReceivedResolvedObjectsInstructions();
    bool HasReceivedResolvedObjects();

    void SendGraphicsInstructions();
    void SignalObjectsResolvedComplete();
    void AwaitGraphicsReadySignal();
    void WaitForResolvedObjectsReceived();
    void WaitForGraphicsStartupEvent(); // reuses resolve objects received irq (blocking)


  private:
    static constexpr LPCSTR graphicsOutputFileName = "BitBladeGraphicsOutputBuffer";
    static constexpr LPCSTR consoleOutputFileName = "BitBladeConsoleOutputBuffer";

    // memory mapped files
    HANDLE hConsoleOutputBuffer;
    HANDLE hGraphicsOutputBuffer;

    // events
    HANDLE hfinishedConsoleInstructionTransferSignal; // sent by console gpio
    HANDLE hfinishedProcessingResolvedObjectsSignal;  // sent by console gpio
    HANDLE hGraphicsFinishedProcessingSignal;         // sent by graphics gpio (catch)
    HANDLE hGraphicsResolvedObjectSendFinishSignal;   // sent by graphics gpio (catch)

    bool isGraphicsReady;
    std::mutex mutexIsGraphicsReady;
    std::future<void> futureGraphicsReadyListener;

    bool isResolvedObjectsReceived;
    std::mutex mutexResolvedObjectsReceived;
    std::future<void> futureResolvedObjectsListener;

    std::atomic<bool> linkStopSignal;

    HANDLE CreateOrConnectEvent( LPCSTR eventName );
    void CreateOrOpenMemoryMap( const LPCSTR& test, HANDLE& handleOut, char*& bufferOut );

    void irqHandlerOnConsoleTransferFinish();
    void irqHandlerOnResolvedObjectsReceived();
    void irqHandlerOnGraphicsReady();

    // simulated irq triggering
    // listen or start in another thread to simulate non blocking irq signalling
    void triggerConsoleTransferFinishDmaIrqAsync();                         // simulate triggering irq handler
    std::future<void> triggerListenerResolvedObjectsReceivedGpioIrqAsync(); // simulate triggering irq handler
    std::future<void> triggerListenerGraphicsReadyGpioIrqAsync();           // simulate triggering irq handler

    void gpioSignalFinishedProcessingResolvedObjects(); // Graphics result resolved
    void gpioSignalFinishedConsoleTransfer();
  };
}

#endif // CONSOLE_LINK_H
