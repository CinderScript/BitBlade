#ifndef I_GFX_MESSAGE_LINK_H
#define I_GFX_MESSAGE_LINK_H

#include "GfxLinkCommon.h"

#include <cstdint>	// uint*_t

namespace gfxLink {

	/// @brief Interface that provides a AddPackedInstruction method call. Inherited by
	/// ConsoleLink so that anything with a reference can pack messages into the link, which
	/// are sent through the link to BladeGraphics.
	class IGfxMessageLink {
	public:

		IGfxMessageLink() :
			packedInstructions( new char[gfxLinkConfig::MESSAGE_BUFFER_LENGTH]() ),
			inputBuffer( nullptr ),
			outputBuffer( nullptr ),
			currentPosition( 0 ) {}

		~IGfxMessageLink() {
			delete[] packedInstructions;
		}

		void AddPackedInstruction(
			gfxLink::GfxCode functionCode,
			const char appendData[],
			uint16_t length )
		{
			gfxLink::packGfxInstruction(
				packedInstructions, functionCode, appendData, length, currentPosition );
		}

		template<typename T>
		void Read( T& valueOut, uint16_t& pos )
		{
			memcpy( &valueOut, inputBuffer + pos, sizeof( T ) );
			pos += sizeof( T );
		}

		void ReadCmdCode( GfxCode cmdCode, uint16_t& pos ) {
			cmdCode = toGfxCommand( inputBuffer[pos++] );
		}

		void ReadString( char* valueOut, uint16_t& pos )
		{
			size_t strLen = strlen( inputBuffer + pos );
			strcpy( valueOut, inputBuffer + pos );

			pos += strLen + 1; // Update position to point after the null terminator
		}

	protected:
		char* packedInstructions;	// double buffer for next outgoing message
		char* outputBuffer;			// buffer written to
		char* inputBuffer;			// buffer read from

		uint16_t currentPosition;     // Position tracker for writing to the buffer
	};

}
#endif // I_GFX_MESSAGE_LINK_H
