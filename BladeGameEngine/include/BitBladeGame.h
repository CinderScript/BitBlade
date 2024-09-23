/* BitBladeGame.h
*/

#ifndef BIT_BLADE_GAME_H 
#define BIT_BLADE_GAME_H

#include "BladeConfig.h"
#include "IBladeGame.h"
#include "IGfxMessageLink.h"
#include "IGfxMessageProducer.h"
#include "DataCluster.h"
#include "DataPool.h"
#include "ImageSource.h"
#include "GameObject.h"
#include "Component.h"
#include "Sprite.h"

#include <vector>



namespace game {

	class BitBladeGame : public IGfxMessageProducer, public IBladeGame {
	public:
		explicit BitBladeGame( IGfxMessageLink* link );
		virtual ~BitBladeGame();

		void Initialize() override = 0;
		void LoadNewLevel() {}

		virtual const char* GetGameTitle() = 0;
		virtual void Update() {}

		bool internalUpdate() final override;

	protected:
		void QuitGame();

		ImageSource* LoadImageSource( const char* filename );
		GameObject* CreateInstance();
		GameObject* CreateInstance( GameObject* parent );

		template<typename T, typename... Args>
		T* AddComponent( Args&&... args ) {
			return componentPool.Add<T>( std::forward<Args>( args )... );
		}


	private:
		std::vector<GameObject*> topLevelObjects;
		DataPool<GameObject> objPool;
		DataPool<ImageSource> imgPool;
		DataCluster componentPool;


		bool isGameRunning;
		char nextPackedCommandTemp[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];

		void packPrefab();
	};
}

#endif // BIT_BLADE_GAME_H