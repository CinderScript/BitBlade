/* GfxCommand.h
*/

#ifndef GFX_COMMAND_H
#define GFX_COMMAND_H


enum class GfxCommand : uint8_t {

	End = 0,						// end of the graphics instructions
	CreateMasterSprite = 1,			// followed by 
	CreateSpriteInstance = 2,

	DeleteMasterSprite = 3,
	DeleteSpriteInstance = 4,

	MovePosition = 3
};

// Operator to automatically convert GfxCommand to uint8_t
constexpr uint8_t operator+(GfxCommand cmd) {
	return static_cast<uint8_t>(cmd);
}

// Operator to automatically convert uint8_t to GfxCommand
constexpr GfxCommand toGfxCommand(uint8_t cmd) {
	return static_cast<GfxCommand>(cmd);
}

#endif // GFX_COMMAND_H