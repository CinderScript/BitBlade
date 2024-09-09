/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include "ConsoleLink.h"
#include "BitBladeGame.h"

#include <memory>  // for smart pointers

using std::unique_ptr;

class BladeConsole {
public:
	BladeConsole();
	~BladeConsole();

	void LoadGame(unique_ptr<BitBladeGame> loadedGame);
	void StartConsole();
	void UpdateConsole();

private:
	ConsoleLink link;
	unique_ptr<BitBladeGame> game;

	void ResolveGraphicsObjects();
};

#endif // BLADE_CONSOLE_H
