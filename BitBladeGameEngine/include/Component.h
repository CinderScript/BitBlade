#ifndef COMPONENT_H
#define COMPONENT_H

#include "GameObject.h"

namespace game {
	class Component {
	public:
		virtual ~Component() {}

		virtual void Update();

	private:
		GameObject* gameObject;
	};
}

#endif // COMPONENT_H
