/* BitBladeGame.h
*/

#ifndef BIT_BLADE_GAME_H 
#define BIT_BLADE_GAME_H

#include "BladeConfig.h"
#include "IGfxMessagePacker.h"
#include "IGfxMessageProducer.h"
#include "DataCluster.h"
#include "DataPool.h"

#include <vector>

namespace game {

	class GameObject;
	class ImageSource;

	class BitBladeGame : public IGfxMessageProducer {
	public:
		// removing parent requires topLevelObjects
		friend class GameObject;

		explicit BitBladeGame( IGfxMessagePacker* messagePacker );
		virtual ~BitBladeGame();

		void Initialize() override = 0;
		void LoadNewLevel() {}

		virtual const char* GetGameTitle() = 0;
		virtual void Update() {}
		bool internalUpdate() final override;

		ImageSource* LoadImageSource( const char* filename );

		GameObject* CreateInstance();
		GameObject* CreateInstance( const char* name );
		GameObject* CreateInstance( GameObject* parent );
		GameObject* CreateInstance( GameObject* parent, const char* name );

		template<typename T, typename... Args>
		T* AddComponent( Args&&... args ) {
			return gameData.Add<T>( std::forward<Args>( args )... );
		}

		void QuitGame();


	private:
		std::vector<GameObject*> topLevelObjects;
		DataCluster gameData;
		uint16_t gameObjectCount;


		bool isGameRunning;
		char nextPackedCommandTemp[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];
	};
}

#endif // BIT_BLADE_GAME_H