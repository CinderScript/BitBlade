/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include "ConsoleLink.h"
#include "BitBladeGame.h"


namespace console {

	class BladeConsole {
	public:
		BladeConsole( BitBladeGame& myGame );
		~BladeConsole();

		void Start();
		void Update();

	private:
		ConsoleLink link;
		BitBladeGame& game;

		void resolveGraphicsObjects();
	};

}

#endif // BLADE_CONSOLE_H
