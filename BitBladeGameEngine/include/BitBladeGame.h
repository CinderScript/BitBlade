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

namespace game {

	class BitBladeGame {
	public:
		// BladeConsole calls the update() function
		// BladeConsole calls the setConsoleLink() function
		friend class console::BladeConsole;

		virtual ~BitBladeGame();

		virtual const char* GetGameTitle() = 0;
		virtual void GameStart() {};
		void LoadNewLevel() {}

	protected:
		void QuitGame();

		ImageSource* LoadImageSource( const char* filename );
		GameObject* CreateInstance( const ImageSource* imageSource );


	private:
		static vector<ImageSource> imageSources;
		static vector<GameObject> gameObjects;
		static bool isGameRunning;
		static char nextPackedCommandTemp[gfxLink::PACKED_COMMAND_MAX_SIZE];

		static bool update();

		static ConsoleLink* consoleLink;
		static void setConsoleLink( ConsoleLink& link );
		void packPrefab();
	};
}

#endif // BIT_BLADE_GAME_H