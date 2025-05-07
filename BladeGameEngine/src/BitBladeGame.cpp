/* BitBladeGame.cpp
*/

#include "BitBladeGame.h"

#include "ImageSource.h"
#include "GameObject.h"

namespace game {// Define the static members

	BitBladeGame::BitBladeGame( gfxLink::IGfxMessagePacker* messagePacker )
		: IGfxMessageProducer( messagePacker ), gameObjectCount( 0 ), shouldContinue( true )
	{
		/* ------------------------------------ _ ----------------------------------- */
	   //gameData.ReservePool<Texture>( 200 );
	}

	BitBladeGame::~BitBladeGame() {

		// TODO: DELETE EVERYTHING IN OBJECT POOL
	}


	void BitBladeGame::QuitGame()
	{
		shouldContinue = false;

		// let the BladeGraphics know we are stopping
		AddPackedInstruction(
			gfxLink::GfxCode::StopGraphics,
			GetGameTitle(),
			strlen( GetGameTitle() ) + 1 );
	}

	ImageSource* BitBladeGame::LoadImageSource( const char* filename ) {

		ImageSource* image = gameData.Add<ImageSource>( this, filename );
		image->Pack_CreateImageData();
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

	void BitBladeGame::internalInitialize()
	{
		GlobalStart();
	}

	bool BitBladeGame::internalUpdate()
	{
		GlobalUpdate(); // updates the user's game's global update

		if (!shouldContinue) {
			return false;			// QuitGame() called in the GlobalUpdate instead of a componennt.
		}

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

			/* * Update the current GameObject * */
			current->updateComponents();

			// Push children onto the stack in reverse order to maintain original order
			const auto& children = current->GetChildren();
			for (auto it = children.rbegin(); it != children.rend(); ++it) {
				stack.push_back( *it );
			}
		}

		// check amount of frame time used:
		// Sort Insertion Order? Does it matter? 

		return shouldContinue;
	}
}