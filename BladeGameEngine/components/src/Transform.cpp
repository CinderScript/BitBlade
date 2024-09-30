/* Transform.cpp
*/

#include "Transform.h"

namespace game
{
	Transform::Transform( GameObject* owner )
		: Component( owner ), position( 0.0f, 0.0f ), rotation( 0.0f ), scale( 1.0f, 1.0f ) {
		updateForwardVector();
	}

	Transform::~Transform() {}
} // namespace game