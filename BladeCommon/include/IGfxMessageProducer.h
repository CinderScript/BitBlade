#ifndef I_GFX_MESSAGE_PRODUCER_H
#define I_GFX_MESSAGE_PRODUCER_H

#include "IGfxMessageLink.h"

class IGfxMessageProducer {
public:
	IGfxMessageProducer( IGfxMessageLink* link ) : gfxMessageHandler( link ) {}
	virtual ~IGfxMessageProducer() = default;
	virtual void InitializeGame() = 0;
	virtual bool update() = 0;

protected:
	IGfxMessageLink* gfxMessageHandler;
};

#endif // I_GFX_MESSAGE_PRODUCER_H
