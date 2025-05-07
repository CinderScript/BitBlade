#include "Component.h"
/* Component.cpp
*/

#include "Component.h"
#include "BitBladeGame.h"
#include "GameObject.h"

namespace game {

	Component::Component( uint16_t poolID, uint16_t objectID, GameObject* owner )
		: DataPoolMember( poolID, objectID ), owner( owner ), game( owner->Game() ) {}

	void Component::Quit()
	{}

}
