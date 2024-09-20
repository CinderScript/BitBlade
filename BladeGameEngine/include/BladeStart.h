/* BladeStart.h
*/


#ifndef BLADE_START_H
#define BLADE_START_H

#include "BladeConsole.h"
#include "BitBladeGame.h"


namespace game {

	template<typename T>
	void StartGame() {

		static_assert(std::is_base_of<BitBladeGame, T>::value, "T must derive from BitBladeGame");

		console::BladeConsole console;

		console.StartGame<T>();

		bool shouldContinue = console.FirstUpdate();

		while (shouldContinue)
		{
			shouldContinue = console.Update();
		}
	}
}



#endif // BLADE_START_H
