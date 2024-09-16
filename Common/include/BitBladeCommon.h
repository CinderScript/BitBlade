/* BitBladeCommon.h
*/

#ifndef BIT_BLADE_COMMON_H
#define BIT_BLADE_COMMON_H

#include <iostream>
#include <cstdint>	// uint*_t
#include <cstddef>  // For size_t
#include <cstring>  // For memcpy


namespace gameConfig
{
	static constexpr int MAX_IMAGE_RESOURCES = 100;
	static constexpr int MAX_GAME_OBJECTS = 300;

	// Max number of bytes any single packed command can be
}

namespace gfxLink
{
	static constexpr int MESSAGE_BUFFER_LENGTH = 1000;
	static constexpr int PACKED_COMMAND_MAX_SIZE = 200;

	enum class GfxCommand : uint8_t {

		End = 0,						// end of the graphics instructions
		CreateImageData = 1,
		CreateSpriteInstance = 2,

		DeleteMasterSprite = 3,
		DeleteSpriteInstance = 4,

		MovePosition = 3
	};

	// Operator to automatically convert GfxCommand to uint8_t
	constexpr uint8_t operator+( GfxCommand cmd ) {
		return static_cast<uint8_t>(cmd);
	}

	// Operator to automatically convert uint8_t to GfxCommand
	constexpr GfxCommand toGfxCommand( uint8_t cmd ) {
		return static_cast<GfxCommand>(cmd);
	}

	template<typename T>
	inline void readMessageBuffer( const char* buffer, T& valueOut, uint16_t& posOut )
	{
		memcpy( &valueOut, buffer + posOut, sizeof( T ) );
		posOut += sizeof( T );
	}

	inline void readMessageBufferString( const char* buffer, char* valueOut, uint16_t& posOut )
	{
		size_t strLen = strlen( buffer + posOut );
		strcpy( valueOut, buffer + posOut );

		posOut += strLen + 1; // Update position to point after the null terminator
	}

	inline void packGfxInstruction(
		char* buffer, GfxCommand functionCode, const char* data, size_t length, uint16_t& posOut )
	{
		if (posOut + length + 2 > MESSAGE_BUFFER_LENGTH)
		{
			// Check buffer overflow (+1 for function code, +1 for EOF code)
			std::cerr << "Buffer overflow prevented." << std::endl;
			return;
		}

		// Write the function code to the buffer
		buffer[posOut++] = +functionCode;

		// Write the data to the buffer
		memcpy( buffer + posOut, data, length );
		posOut += length;
	}
}

#endif // BIT_BLADE_COMMON_H