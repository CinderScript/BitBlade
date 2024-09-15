/* Sprite.h
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "ImageSource.h"
#include "Vector2.h"

#include <cstdint>  // uint16_t

namespace game {
	class Sprite {

	public:
		Sprite( const ImageSource* imageSource );

		Vector2 Pivot() const { return pivot; }
		void SetPivot( const Vector2& Pivot ) { pivot = Pivot; }

		Vector2 Scale() const { return scale; }
		void SetScale( const Vector2& Scale ) { scale = Scale; }

	private:
		const ImageSource* image;
		Vector2 pivot;
		Vector2 scale;
	};
}

#endif // SPRITE_H
