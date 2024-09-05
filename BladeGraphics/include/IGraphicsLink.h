/* IGraphicsLink.h
*/

#ifndef IGRAPHICS_LINK_H
#define IGRAPHICS_LINK_H

class IGraphicsLink {
public:
	static constexpr int GRAPHICS_BUFFER_LENGTH = 1000;

	virtual ~IGraphicsLink() {}

	virtual void WaitForConnectedThreadFinish() = 0;
	virtual const char* GetBladeMessage() = 0;

	virtual void PackInstruction(char functionCode, const char* data, size_t length) = 0;
	virtual void SendBladeMessage() = 0;

	virtual void SignalGraphicsUpdateSent() = 0;
};

#endif // IGRAPHICS_LINK_H
