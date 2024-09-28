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

		void LoadNewLevel() {}

		virtual const char* GetGameTitle() = 0;

		void Initialize() override = 0;
		void Start() override {};
		virtual void Update() {}

		bool internalUpdate() final override;

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

		const std::vector<GameObject*>& GetTopLevelObjects() const { return topLevelObjects; }

	private:
		std::vector<GameObject*> topLevelObjects;
		DataCluster gameData;
		uint16_t gameObjectCount;


		bool shouldContinue;
	};
}

#endif // BIT_BLADE_GAME_H