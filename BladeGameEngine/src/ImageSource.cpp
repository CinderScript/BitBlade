/* ImageSource.cpp
*/

#include "ImageSource.h"

#include <cstring>
namespace game {
	/* --------------------------------- PUBLIC --------------------------------- */

	ImageSource::~ImageSource() {}


	/* --------------------------------- PRIVATE -------------------------------- */

	ImageSource::ImageSource( const char* filename )
		: filename( filename ),
		pivot(),
		scale(),
		isResolved( 0 ),
		bladeGraphicsAddress( nullptr ) {}

	uint16_t ImageSource::Pack_CreateImageData( char* dataOut )
	{
		uint16_t pos = 0;

		// copy the ID to the instruction message
		memcpy( dataOut + pos, &objectID, sizeof( uint16_t ) );
		pos += sizeof( uint16_t );

		// Copy the filename ensuring it is null-terminated
		strcpy( dataOut + pos, filename );
		pos += std::strlen( filename ) + 1; // +1 to include the null terminator

		return pos; // This returns the total number of bytes written to dataOut
	}
}