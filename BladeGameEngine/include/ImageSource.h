/* ImageSource.h
*/

#ifndef IMAGE_SOURCE_H 
#define IMAGE_SOURCE_H

#include "DataPoolMember.h"
#include "IGfxMessageProducer.h"
#include "Vector2.h"

namespace game {
	template<typename T>
	class DataPool;

	class ImageSource : public DataPoolMember {
	public:
		friend struct DataPool<ImageSource>;
		friend class BitBladeGame;					// calls Pack_CreateImageData

		~ImageSource();

		Vector2 Pivot() const { return pivot; }
		void SetPivot( const Vector2& Pivot ) { pivot = Pivot; }

		Vector2 Scale() const { return scale; }
		void SetScale( const Vector2& Scale ) { scale = Scale; }


	private:
		IGfxMessageProducer* messageSource;
		const char* filename;
		Vector2 pivot;
		Vector2 scale;

		// BladeGraphics will use the index given by BitBladeGame to the object pool
		// as an ID until it can be resolved by saving BladeGraphic's address for this resource.
		uint8_t isResolved;
		void* bladeGraphicsAddress;

		ImageSource( IGfxMessageProducer* messageSource, const char* filename );
		uint16_t Pack_CreateImageData();
	};
}

#endif // IMAGE_SOURCE_H