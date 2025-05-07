/* Component.h
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include "DataPoolMember.h"

#include <cstdint>

namespace game {

	template<typename T>
	class DataPool;
	class BitBladeGame;
	class GameObject; // forward declaration

	class Component : public DataPoolMember
	{
	public:
		friend struct DataPool<Component>;
		friend class BitBladeGame;					// calls constructor

		/// @brief DataPoolMembers are assigned a PoolID and ObjectID after construction.
		/// @param owner  - the parent GameObject
		Component( uint16_t poolID, uint16_t objectID, GameObject* owner );
		virtual ~Component() {}

		/// @brief Called right after Component is added to the GameObject.
		virtual void Awake() {}

		/// @brief Called before this component is Updated for the first time.
		virtual void Start() {}

		/// @brief Called right after Component is added to the GameObject.
		virtual void Update() {}

		GameObject* Owner() { return owner; }


		/// ToDo:  Add functions usefull inside the game update loop, such as
		///  Quit, Instantiate, CreateImageSource, etc...

		void Quit();

	protected:
		GameObject* owner;
		BitBladeGame* game;
	};
}

#endif // COMPONENT_H
