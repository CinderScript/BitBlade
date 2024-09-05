/* GraphicsLink.h
*/

#ifndef BLADE_LINK_H
#define BLADE_LINK_H

#include "IGraphicsLink.h"
#include <Windows.h>

class GraphicsLink : public IGraphicsLink {
public:
	GraphicsLink(bool isServer);
	~GraphicsLink() override;

	void WaitForConnectedThreadFinish() override;
	const char* GetBladeMessage() override;

	void PackInstruction(char functionCode, const char* data, size_t length) override;
	void SendBladeMessage() override;

	void SignalGraphicsUpdateSent() override;

private:
	static constexpr LPCSTR consoleOutputFileName = "BitBladeConsoleOutputBuffer";
	static constexpr LPCSTR graphicsOutputFileName = "BitBladeGraphicsOutputBuffer";

	bool isServer;

	char* packedInstructions; /////////////!!!!!////////////////

	char* outputMessageBuffer;
	char* inputMessageBuffer;
	size_t currentPosition = 0; // Position tracker for writing to the buffer

	HANDLE hOutputBufferHandle;
	HANDLE hInputBufferHandle;

	HANDLE hThisThreadReady;
	HANDLE hConnectingThreadReady;

	HANDLE CreateOrConnectEvent(const char* eventName);
	void CreateOrOpenMemoryMap(const LPCSTR& test, HANDLE& handleOut, char* bufferOut);
};

#endif // BLADE_LINK_H

