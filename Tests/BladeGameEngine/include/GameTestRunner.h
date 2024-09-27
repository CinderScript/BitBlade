

#include "BitBladeGame.h"

template<typename T>
void StartGame() {

	static_assert(std::is_base_of<IGfxMessageProducer, T>::value,
		"T must derive from BitBladeGame, or IGfxMessageProducer");

	// Initialize the game pointer with an instance of T
	auto* game = std::make_unique<T>( link.get() );

	// Start the game
	game->Initialize();

	game->Start();

	bool shouldContinue = game.internalUpdate();

	while (shouldContinue)
	{
		shouldContinue = game.internalUpdate();
	}
}