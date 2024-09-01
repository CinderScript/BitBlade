/* BladeLink.h
*/

#ifndef BLADE_LINK_H
#define BLADE_LINK_H

#include "IBladeLink.h"
#include <Windows.h>

using std::vector;

class BladeLink : public IBladeLink {
public:
	BladeLink(bool isServer);
	~BladeLink() override;

	void WaitForConnectedThreadReady() override;
	const char* GetBladeMessage() override;

	void PackInstruction(uint8_t functionCode, const char* data, size_t length) override;
	void SendBladeMessage() override;

	void SignalThisThreadReady() override;

private:
	static constexpr LPCSTR consoleOutputFileName = "BitBladeConsoleOutputBuffer";
	static constexpr LPCSTR graphicsOutputFileName = "BitBladeGraphicsOutputBuffer";
	static constexpr int mapLength = 1000;
	static constexpr int mapSize = sizeof(char) * mapLength;

	bool isServer;

	char* outputMessageBuffer;
	char* inputMessageBuffer;
	size_t currentPosition = 0; // Position tracker for writing to the buffer

	HANDLE hOutputBufferHandle;
	HANDLE hInputBufferHandle;

	HANDLE hThisThreadReady;
	HANDLE hConnectingThreadReady;

	HANDLE ConnectEvent(const char* eventName);
	void CreateOrOpenMemoryMap(const LPCSTR& test, HANDLE& handleOut, char* bufferOut);
};

#endif // BLADE_LINK_H

