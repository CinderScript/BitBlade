/* ImageSource.cpp
*/

#include "ImageSource.h"

#include <cstring>

/* --------------------------------- PUBLIC --------------------------------- */

ImageSource::~ImageSource() {}


/* --------------------------------- PRIVATE -------------------------------- */

ImageSource::ImageSource( const char* filename, uint16_t index )
	: filename( filename ), pivot(), scale(), isResolved( 0 ),
	masterIndex( index ), bladeGraphicsAddress( nullptr ) {}

uint16_t ImageSource::Pack_CreateImageData( char* dataOut )
{
	uint16_t pos = 0;

	memcpy( dataOut + pos, &masterIndex, sizeof( uint16_t ) );
	pos += sizeof( uint16_t );

	// Copy the filename ensuring it is null-terminated
	strcpy( dataOut + pos, filename );
	pos += std::strlen( filename ) + 1; // +1 to include the null terminator

	return pos; // This returns the total number of bytes written to dataOut
}