// UniqueComponentTrait.h

#ifndef UNIQUE_COMPONENT_TRAIT_H
#define UNIQUE_COMPONENT_TRAIT_H

#include <type_traits>

namespace game {

	// Primary template: By default, components are not unique
	template<typename T>
	struct IsUniqueComponent {
		static_assert(std::is_base_of<Component, T>::value,
			"IsUniqueComponent can only be specialized for Component-derived types.");
		static constexpr bool value = false;
	};

	// Macro for making a component unique so only one can be added to a GameObject
#define MAKE_COMPONENT_UNIQUE(T) \
    template<> \
    struct game::IsUniqueComponent<T> { \
        static_assert(std::is_base_of<game::Component, T>::value, \
                      "T must inherit from Component to be marked as unique."); \
        static constexpr bool value = true; \
    };
} // namespace game


//MAKE_COMPONENT_UNIQUE( game::Sprite );

#endif // UNIQUE_COMPONENT_TRAIT_H
