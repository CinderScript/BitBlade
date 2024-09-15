/* ImageSource.h
*/

#ifndef IMAGE_SOURCE_H 
#define IMAGE_SOURCE_H

#include "ConsoleLink.h"
#include "Vector2.h"

namespace game {
	class BitBladeGame;

	class ImageSource {
	public:
		// required for BitBladeGame to create a new ImageSource
		friend class BitBladeGame;

		~ImageSource();

		Vector2 Pivot() const { return pivot; }
		void SetPivot( const Vector2& Pivot ) { pivot = Pivot; }

		Vector2 Scale() const { return scale; }
		void SetScale( const Vector2& Scale ) { scale = Scale; }


	private:
		const char* filename;
		Vector2 pivot;
		Vector2 scale;

		// BladeGraphics will use the index given by BitBladeGame to the object pool
		// as an ID until it can be resolved by saving BladeGraphic's address for this resource.
		uint8_t isResolved;
		uint16_t masterIndex;
		void* bladeGraphicsAddress;

		ImageSource( const char* filename, uint16_t index );

		uint16_t Pack_CreateImageData( char* dataOut );
	};
}

#endif // IMAGE_SOURCE_H