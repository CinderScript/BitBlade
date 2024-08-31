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

	void SendBladeMessage(const char* message, const size_t size) override;

	void SignalThisThreadReady() override;

private:
	static constexpr LPCSTR memoryFileName = "BitBladeConnectionBuffer98801";
	static constexpr int mapLength = 1000;
	static constexpr int mapSize = sizeof(char) * mapLength;
	HANDLE fileHandle;
	char* messageBuffer;

	HANDLE hThisThreadReady;
	HANDLE hConnectingThreadReady;

	HANDLE ConnectEvent(const char* eventName);
	HANDLE CreateOrOpenMemoryMap();
	char* MapView();
};

#endif // BLADE_LINK_H

