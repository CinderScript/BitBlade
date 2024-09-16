/* BitBlade.h
*/

#ifndef BIT_BLADE_H
#define BIT_BLADE_H

#include "BladeConsole.h"
#include "BitBladeGame.h"


namespace game {

	void RunGame( BitBladeGame& myGame ) {

		console::BladeConsole console( myGame );

		console.Start();

		while (true)
		{
			console.Update();
		}
	}

}

#endif // BIT_BLADE_H
