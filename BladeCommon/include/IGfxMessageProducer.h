#ifndef I_GFX_MESSAGE_PRODUCER_H
#define I_GFX_MESSAGE_PRODUCER_H

#include "IGfxMessagePacker.h"

class IGfxMessageProducer {
public:
	IGfxMessageProducer( IGfxMessagePacker* link ) : gfxMessagePacker( link ) {}

	virtual ~IGfxMessageProducer() = default;
	virtual void Initialize() = 0;
	virtual bool internalUpdate() = 0;

	void AddPackedInstruction(
		gfxLink::GfxCode functionCode,
		const char* data,
		uint16_t length )
	{
		gfxMessagePacker->AddPackedInstruction(
			functionCode,
			data,
			length );
	}

private:
	IGfxMessagePacker* gfxMessagePacker;
};

#endif // I_GFX_MESSAGE_PRODUCER_H
