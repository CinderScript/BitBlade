#ifndef I_GFX_MESSAGE_PRODUCER_H
#define I_GFX_MESSAGE_PRODUCER_H

#include "IGfxMessageLink.h"

namespace gfxLink {

	class IGfxMessageProducer {
	public:
		IGfxMessageProducer( IGfxMessageLink* link ) : gfxMessageLink( link ) {}

		virtual ~IGfxMessageProducer() = default;
		virtual void internalInitialize() = 0;
		virtual bool internalUpdate() = 0;

		void AddPackedInstruction(
			gfxLink::GfxCode functionCode,
			const char* data,
			uint16_t length )
		{
			gfxMessageLink->AddPackedInstruction(
				functionCode,
				data,
				length );
		}



	private:
		IGfxMessageLink* gfxMessageLink;
	};
}

#endif // I_GFX_MESSAGE_PRODUCER_H
