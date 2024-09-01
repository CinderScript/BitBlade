/* IBladeLink.h
*/

#ifndef IBLADE_LINK
#define IBLADE_LINK

#include <vector>

using std::vector;

class IBladeLink {
public:
	virtual ~IBladeLink() {}

	virtual void WaitForConnectedThreadReady() = 0;
	virtual const char* GetBladeMessage() = 0;

	virtual void PackInstruction(uint8_t functionCode, const char* data, size_t length) = 0;
	virtual void SendBladeMessage() = 0;

	virtual void SignalThisThreadReady() = 0;
};

#endif // IBLADE_LINK
