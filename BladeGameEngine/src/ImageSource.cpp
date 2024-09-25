/* ImageSource.cpp
*/

#include "ImageSource.h"
#include "BladeConfig.h"

#include <cstring>
namespace game {
	/* --------------------------------- PUBLIC --------------------------------- */

	ImageSource::~ImageSource() {}


	/* --------------------------------- PRIVATE -------------------------------- */

	ImageSource::ImageSource( IGfxMessageProducer* messageSource, const char* filename )
		: filename( filename ),
		messageSource( messageSource ),
		pivot(),
		scale(),
		isResolved( 0 ),
		bladeGraphicsAddress( nullptr )
	{
		Pack_CreateImageData();
	}

	uint16_t ImageSource::Pack_CreateImageData()
	{
		uint16_t pos = 0;
		char message[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];

		// copy the ID to the instruction message
		memcpy( message + pos, &objectID, sizeof( uint16_t ) );
		pos += sizeof( uint16_t );

		// Copy the filename ensuring it is null-terminated
		strcpy( message + pos, filename );
		pos += std::strlen( filename ) + 1; // +1 to include the null terminator

		// pack into the ConsoleLink buffer
		messageSource->AddPackedInstruction(
			gfxLink::GfxCode::CreateImageData,
			message,
			pos );

		return pos; // This returns the total number of bytes written to message
	}
}