/* BitBladeGame.h
*/

#ifndef BIT_BLADE_GAME_H 
#define BIT_BLADE_GAME_H

#include "GameObject.h"
#include "ImageSource.h"

#include <vector>
using std::vector;


namespace console {
	class BladeConsole;
}

class BitBladeGame {
public:
	// BladeConsole calls the update() function
	// BladeConsole calls the setConsoleLink() function
	friend class console::BladeConsole;

	virtual ~BitBladeGame();

protected:

	static ImageSource* LoadImageSource( const char* filename );
	static GameObject* CreateInstance( const ImageSource* imageSource );

	virtual void GameStart() {}
	void LoadNewLevel() {}

private:
	static vector<ImageSource> imageSources;
	static vector<GameObject> gameObjects;
	static char nextPackedCommandTemp[gameConfig::PACKED_COMMAND_MAX_SIZE];

	static void update();

	static ConsoleLink* consoleLink;
	static void setConsoleLink( ConsoleLink& link );
	void packPrefab();
};

#endif // BIT_BLADE_GAME_H