/* BladeConsole.h
*/

#ifndef BLADE_CONSOLE_H
#define BLADE_CONSOLE_H

#include "ConsoleLink.h"
#include "BitBladeGame.h"


namespace console {

	class BladeConsole {

	public:
		BladeConsole();

		~BladeConsole();

		template<typename T>
		void StartGame()
		{
			static_assert(std::is_base_of<game::BitBladeGame, T>::value, "T must derive from BitBladeGame");
			game = std::make_unique<T>();
			game->GameStart();
		}

		bool FirstUpdate();
		bool Update();

	private:
		ConsoleLink link;
		std::unique_ptr<game::BitBladeGame> game;

		void resolveGraphicsObjects();
	};




}

#endif // BLADE_CONSOLE_H
