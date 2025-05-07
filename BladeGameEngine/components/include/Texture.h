/** Texture.h
 * 	Texture is a type of GameObject component that links to an ImageSource
 *	resource. When this Texture, or Transform for this GameObject is
 *	modified,
 *
 *	Texture is a type of GameObject component that links to an
 *	ImageSource resource. When this Textue, or Transform for this GameObject
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "Vector2.h"
#include "UniqueComponentTrait.h"


#include <cstdint>  // uint16_t

namespace gfxLink {
	class IGfxMessageProducer;
}

namespace game {
	template<typename T>
	class DataPool;
	class ImageSource;

	class Texture : public Component {

	public:
		friend struct DataPool<Texture>;

		Vector2 Pivot() const { return pivot; }
		void SetPivot( const Vector2& Pivot ) { pivot = Pivot; }

		Vector2 Scale() const { return scale; }
		void SetScale( const Vector2& Scale ) { scale = Scale; }

	private:
		const ImageSource* image;
		const gfxLink::IGfxMessageProducer* messageProducer;
		Vector2 pivot;
		Vector2 scale;

		Texture( uint16_t poolID, uint16_t objectID, GameObject* owner, const ImageSource* imageSource );

		/// @brief Packs a Create Image Data message using the Message Producer
		/// @return the current position of the packed message buffer
		uint16_t Pack_CreateSprite();
	};
}

MAKE_COMPONENT_UNIQUE( game::Texture );

#endif // SPRITE_H
