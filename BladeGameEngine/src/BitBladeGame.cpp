/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"

#include "ImageSource.h"
#include "GameObject.h"

namespace game {// Define the static members

	BitBladeGame::BitBladeGame( IGfxMessagePacker* messagePacker )
		: IGfxMessageProducer( messagePacker ), gameObjectCount( 0 )
	{
		/* ------------------------------------ _ ----------------------------------- */
	   //gameData.ReservePool<Sprite>( 200 );
	}

	BitBladeGame::~BitBladeGame() { }


	void BitBladeGame::QuitGame()
	{
		isGameRunning = false;

		// let the BladeGraphics know we are stopping
		AddPackedInstruction(
			gfxLink::GfxCode::StopGraphics,
			GetGameTitle(),
			strlen( GetGameTitle() ) + 1 );
	}

	ImageSource* BitBladeGame::LoadImageSource( const char* filename ) {

		ImageSource* image = gameData.Add<ImageSource>( this, filename );
		return image;
	}


	GameObject* BitBladeGame::Instantiate()
	{
		gameObjectCount++;
		auto* obj = gameData.Add<GameObject>( this );
		topLevelObjects.push_back( obj );
		return obj;
	}
	GameObject* BitBladeGame::Instantiate( const char* name )
	{
		gameObjectCount++;
		auto* obj = gameData.Add<GameObject>( this, name );
		topLevelObjects.push_back( obj );
		return obj;
	}
	GameObject* BitBladeGame::Instantiate( GameObject* parent )
	{
		gameObjectCount++;
		auto* obj = gameData.Add<GameObject>( this, parent );
		if (!parent) {
			topLevelObjects.push_back( obj );
		}
		return obj;
	}
	GameObject* BitBladeGame::Instantiate( GameObject* parent, const char* name )
	{
		gameObjectCount++;
		auto* obj = gameData.Add<GameObject>( this, parent, name );
		if (!parent) {
			topLevelObjects.push_back( obj );
		}
		return obj;
	}


	bool BitBladeGame::internalUpdate()
	{
		Update(); // updates user's top level update / global update.

		std::vector<GameObject*> stack;
		stack.reserve( gameObjectCount ); // Reserve space to minimize reallocations

		// Push all top-level objects onto the stack in reverse order
		// This ensures that the first object in topLevelObjects is processed first
		for (auto it = topLevelObjects.rbegin(); it != topLevelObjects.rend(); ++it) {
			stack.push_back( *it );
		}

		// Perform iterative DFS
		while (!stack.empty()) {
			GameObject* current = stack.back();
			stack.pop_back();

			// Update the current GameObject
			current->internalUpdate();

			// Push children onto the stack in reverse order to maintain original order
			const auto& children = current->GetChildren();
			for (auto it = children.rbegin(); it != children.rend(); ++it) {
				stack.push_back( *it );
			}
		}

		return isGameRunning;
	}
}