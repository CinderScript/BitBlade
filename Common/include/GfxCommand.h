/* GfxCommand.h
*/

#ifndef GFX_COMMAND_H
#define GFX_COMMAND_H


enum class GfxCommand : uint8_t {

	EOF = 0
	CreateMasterSprite = 1,
	CreateSpriteInstance = 2,

	DeleteMasterSprite = 3,
	DeleteSpriteInstance = 4,

	MovePosition = 3
};

#endif // GFX_COMMAND_H