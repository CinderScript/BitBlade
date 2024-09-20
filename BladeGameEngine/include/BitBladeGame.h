/* BitBladeGame.h
*/

#ifndef BIT_BLADE_GAME_H 
#define BIT_BLADE_GAME_H

#include "BitBladeCommon.h"
#include "DataCluster.h"
#include "DataPool.h"
#include "ImageSource.h"
#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"

#include <vector>

namespace console {
	class BladeConsole;
	class ConsoleLink;
}

namespace game {

	class ImageSource;

	class BitBladeGame {
	public:
		// BladeConsole calls the update() function
		// BladeConsole calls the setConsoleLink() function
		friend class console::BladeConsole;

		BitBladeGame();
		virtual ~BitBladeGame();

		virtual const char* GetGameTitle() = 0;
		virtual void GameStart() {};
		void LoadNewLevel() {}

	protected:
		void QuitGame();

		ImageSource* LoadImageSource( const char* filename );
		GameObject* CreateInstance( const ImageSource* imageSource );

		template<typename T>
		GameObject* AddComponent( const ImageSource* imageSource );


	private:
		std::vector<GameObject> topLevelObjects;
		DataPool<ImageSource> imgPool;
		DataPool<GameObject> objPool;
		DataCluster componentPool;


		static bool isGameRunning;
		static char nextPackedCommandTemp[gfxLink::PACKED_COMMAND_MAX_SIZE];

		static bool update();

		static console::ConsoleLink* consoleLink;
		static void setConsoleLink( console::ConsoleLink& link );
		void packPrefab();
	};
}

#endif // BIT_BLADE_GAME_H