/* BitBladeCommon.h
*/

#ifndef GFX_LINK_COMMON_H
#define GFX_LINK_COMMON_H

#include "BladeConfig.h"

#include <iostream>
#include <cstdint>	// uint*_t
#include <cstddef>  // For size_t
#include <cstring>  // For memcpy


namespace gfxLink
{
	enum class GfxCode : uint8_t {

		EndMessage = 0,						// end of the graphics instructions buffer
		CreateImageData = 1,
		CreateSpriteInstance = 2,

		DeleteMasterSprite = 3,
		DeleteSpriteInstance = 4,

		MovePosition = 3,

		StopGraphics = 255					// program end
	};

	// Operator to automatically convert GfxCode to uint8_t
	constexpr uint8_t operator+( GfxCode cmd ) {
		return static_cast<uint8_t>(cmd);
	}

	// Operator to automatically convert uint8_t to GfxCode
	constexpr GfxCode toGfxCommand( uint8_t cmd ) {
		return static_cast<GfxCode>(cmd);
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
		char* buffer, GfxCode functionCode, const char* data, size_t length, uint16_t& posOut )
	{
		if (posOut + length + 2 > gfxLinkConfig::MESSAGE_BUFFER_LENGTH)
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

#endif // GFX_LINK_COMMON_H