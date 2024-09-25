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
			// BitBladeGame stores GameObject directly in DataPool, so it needs
			// the header file. GameObject needs to use AddComponent template, so
			// it also needs header access, so we give it access to IBladeGame
			return nullptr; // Placeholder
		}
	};


} // namespace game


#endif // I_BLADE_GAME_H
