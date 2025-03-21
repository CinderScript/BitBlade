/* Texture.h
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "ImageSource.h"
#include "Vector2.h"
#include "UniqueComponentTrait.h"

#include <cstdint>  // uint16_t

namespace game {
	template<typename T>
	class DataPool;
	class BitBladeGame;

	class Texture : public Component {

	public:
		friend struct DataPool<Texture>;
		friend class BitBladeGame;

		Vector2 Pivot() const { return pivot; }
		void SetPivot( const Vector2& Pivot ) { pivot = Pivot; }

		Vector2 Scale() const { return scale; }
		void SetScale( const Vector2& Scale ) { scale = Scale; }

	private:
		const ImageSource* image;
		Vector2 pivot;
		Vector2 scale;

		Texture( GameObject* owner, const ImageSource* imageSource );
	};
}

MAKE_COMPONENT_UNIQUE( game::Texture );

#endif // SPRITE_H
