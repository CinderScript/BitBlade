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

	private:
		Vector2 position;
		Vector2 rotation;
		Vector2 scale;
	};

} // namespace game


#endif /*POSITION_H*/
