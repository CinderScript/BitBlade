/* Component.h
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include "DataPoolMember.h"

#include <cstdint>

namespace game {

	template<typename T>
	class DataPool;
	class GameObject; // forward declaration


	class Component : public DataPoolMember
	{
	public:
		friend struct DataPool<Component>;
		friend class BitBladeGame;					// calls constructor

		Component( GameObject* owner ) : owner( owner ) {}
		virtual ~Component() {}

		/// @brief Called right after Component is added to the GameObject.
		virtual void Awake() {}

		/// @brief Called before this component is Updated for the first time.
		virtual void Start() {}

		/// @brief Called right after Component is added to the GameObject.
		virtual void Update() {}

		GameObject* Owner() { return owner; }


	private:
		GameObject* owner;

	};
}

#endif // COMPONENT_H
