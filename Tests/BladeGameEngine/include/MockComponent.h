// MockComponent.h

#ifndef MOCK_COMPONENT_H
#define MOCK_COMPONENT_H

#include "Component.h"
#include "gmock/gmock.h"

namespace game {

	class MockComponent : public Component {
	public:
		MockComponent( GameObject* owner ) : Component( owner ) {}
		MOCK_METHOD( void, Awake, (), (override) );
		MOCK_METHOD( void, Start, (), (override) );
		MOCK_METHOD( void, Update, (), (override) );
	};

} // namespace game

#endif // MOCK_COMPONENT_H
