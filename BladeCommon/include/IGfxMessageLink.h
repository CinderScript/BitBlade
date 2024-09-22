#ifndef I_GFX_MESSAGE_LINK_H
#define I_GFX_MESSAGE_LINK_H

#include "GfxLinkCommon.h"

#include <cstdint>	// uint*_t


class IGfxMessageLink {
public:
	virtual ~IGfxMessageLink() = default;
	virtual void PackInstruction(
		gfxLink::GfxCode functionCode,
		const char* data,
		uint16_t length ) = 0;
};

#endif // I_GFX_MESSAGE_LINK_H
