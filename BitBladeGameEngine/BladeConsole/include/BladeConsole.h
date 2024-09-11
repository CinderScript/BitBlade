/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include "ConsoleLink.h"
#include "BitBladeGame.h"

#include <memory>  // for smart pointers

using std::unique_ptr;

namespace console {

	class BladeConsole {
	public:
		BladeConsole( BitBladeGame& myGame );
		~BladeConsole();

		void StartConsole();
		void UpdateConsole();

	private:
		ConsoleLink link;
		BitBladeGame& game;

		void ResolveGraphicsObjects();
	};

}

#endif // BLADE_CONSOLE_H
