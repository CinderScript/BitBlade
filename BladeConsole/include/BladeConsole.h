/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include "ConsoleLink.h"
#include "IGfxMessageProducer.h"


namespace console {

	class BladeConsole {

	public:
		BladeConsole();

		~BladeConsole();

		template<typename T>
		void StartGame()
		{
			static_assert(std::is_base_of<IGfxMessageProducer, T>::value,
				"T must derive from BitBladeGame, or IGfxMessageProducer");

			// Initialize the game pointer with an instance of T
			game = std::make_unique<T>( link.get() );

			// Start the game
			game->InitializeGame();
		}

		bool FirstUpdate();
		bool Update();

	private:
		std::unique_ptr<ConsoleLink> link;
		std::unique_ptr<IGfxMessageProducer> game;

		void resolveGraphicsObjects();
	};




}

#endif // BLADE_CONSOLE_H
