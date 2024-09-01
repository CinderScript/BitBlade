/* IBladeLink.h
*/

#ifndef IBLADE_LINK_H
#define IBLADE_LINK_H

class IBladeLink {
public:
	static constexpr int GRAPHICS_BUFFER_LENGTH = 1000;

	virtual ~IBladeLink() {}

	virtual void WaitForConnectedThreadReady() = 0;
	virtual const char* GetBladeMessage() = 0;

	virtual void PackInstruction(char functionCode, const char* data, size_t length) = 0;
	virtual void SendBladeMessage() = 0;

	virtual void SignalThisThreadReady() = 0;
};

#endif // IBLADE_LINK_H
