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
	class Texture;

	class BitBladeGame : public gfxLink::IGfxMessageProducer {
	public:
		// removing parent requires topLevelObjects
		friend class GameObject;

		explicit BitBladeGame( gfxLink::IGfxMessagePacker* messagePacker );
		virtual ~BitBladeGame();

		void LoadNewLevel() {}

		virtual const char* GetGameTitle() = 0;

		virtual void GlobalStart() {}
		virtual void GlobalUpdate() {}

		void internalInitialize() final override;
		bool internalUpdate() final override;

		const std::vector<GameObject*>& GetTopLevelObjects() const { return topLevelObjects; }

		ImageSource* LoadImageSource( const char* filename );

		GameObject* Instantiate();
		GameObject* Instantiate( const char* name );
		GameObject* Instantiate( GameObject* parent );
		GameObject* Instantiate( GameObject* parent, const char* name );

		template<typename T, typename... Args>
		T* AddComponent( Args&&... args ) {
			return gameData.Add<T>( std::forward<Args>( args )... );
		}

		void QuitGame();

		void PackImageSourceCreationMessage( ImageSource* newImageSource );
		void PackTextureCreationMessage( ImageSource* newImageSource );

	private:
		std::vector<GameObject*> topLevelObjects;
		DataCluster gameData;
		uint16_t gameObjectCount;

		std::vector<ImageSource*> pendingImageSourceCreationMessage;
		std::vector<ImageSource*> pendingTextureCreationMessage;

		bool shouldContinue;
	};
}

#endif // BIT_BLADE_GAME_H