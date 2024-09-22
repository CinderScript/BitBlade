/* BitBladeGame.h
*/

#ifndef BIT_BLADE_GAME_H 
#define BIT_BLADE_GAME_H

#include "BladeConfig.h"
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

	class BitBladeGame : public IGfxMessageProducer {
	public:

		explicit BitBladeGame( IGfxMessageLink* link );

		virtual ~BitBladeGame();

		virtual const char* GetGameTitle() = 0;
		void InitializeGame() override {};
		void LoadNewLevel() {}


		bool update() override;

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


		bool isGameRunning;
		char nextPackedCommandTemp[gfxLinkConfig::PACKED_INSTRUCTION_MAX_LENGTH];

		void packPrefab();
	};
}

#endif // BIT_BLADE_GAME_H