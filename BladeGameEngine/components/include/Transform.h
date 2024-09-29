#ifndef POSITION_H
#define POSITION_H

#include "Vector2.h"
#include "Component.h"

namespace game
{

	struct Transform : public Component
	{
	public:
		Transform( GameObject* owner );
		~Transform();

		Vector2& Position() { return position; }
		const Vector2& Position() const { return position; }

		Vector2& Rotation() { return rotation; }
		const Vector2& Rotation() const { return rotation; }

		Vector2& Scale() { return scale; }
		const Vector2& Scale() const { return scale; }

	private:
		Vector2 position;
		Vector2 rotation;
		Vector2 scale;
	};

} // namespace game


#endif /*POSITION_H*/
