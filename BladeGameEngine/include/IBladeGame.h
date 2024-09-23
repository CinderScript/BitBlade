#ifndef I_BLADE_GAME_H
#define I_BLADE_GAME_H

namespace game
{

	class IBladeGame
	{
	public:
		virtual ~IBladeGame() = default;

		template<typename T, typename... Args>
		T* AddComponent( Args&&... args ) {
			// Implementation in BitBladeGame.cpp
			return nullptr; // Placeholder
		}
	};


} // namespace game


#endif // I_BLADE_GAME_H
