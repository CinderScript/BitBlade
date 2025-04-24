#ifndef I_GFX_MESSAGE_LINK_H
#define I_GFX_MESSAGE_LINK_H

#include "GfxLinkCommon.h"

#include <cstdint>	// uint*_t

namespace gfxLink {

	/// @brief Interface that provides a AddPackedInstruction method call. Inherited by
	/// ConsoleLink so that anything with a reference can pack messages into the link, which
	/// are sent through the link to BladeGraphics.
	class IGfxMessagePacker {
	public:
		virtual ~IGfxMessagePacker() = default;

		virtual void AddPackedInstruction(
			gfxLink::GfxCode functionCode,
			const char appendData[],
			uint16_t length ) = 0;
	};

}
#endif // I_GFX_MESSAGE_LINK_H
