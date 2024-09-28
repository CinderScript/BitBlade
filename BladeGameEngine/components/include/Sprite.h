/* Sprite.h
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "Component.h"
#include "ImageSource.h"
#include "Vector2.h"

#include <cstdint>  // uint16_t

namespace game {
	template<typename T>
	class DataPool;
	class BitBladeGame;

	class Sprite : public Component {

	public:
		friend struct DataPool<Sprite>;
		friend class BitBladeGame;

		Vector2 Pivot() const { return pivot; }
		void SetPivot( const Vector2& Pivot ) { pivot = Pivot; }

		Vector2 Scale() const { return scale; }
		void SetScale( const Vector2& Scale ) { scale = Scale; }

	private:
		const ImageSource* image;
		Vector2 pivot;
		Vector2 scale;

		Sprite( GameObject* owner, const ImageSource* imageSource );
	};
}

#endif // SPRITE_H
