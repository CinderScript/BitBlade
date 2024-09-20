/* Component.h
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include "DataPoolMember.h"

#include <cstdint>

namespace game {

	class GameObject; // forward declaration

	class Component : public DataPoolMember
	{
	public:
		virtual ~Component() {}

		virtual void Update() {}

	protected:
		Component( GameObject* gameObject ) : gameObject( gameObject ) {}

	private:
		GameObject* gameObject;

	};
}

#endif // COMPONENT_H
