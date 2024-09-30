#ifndef DEBUG_PRINT_COMPONENT_H
#define DEBUG_PRINT_COMPONENT_H

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

#include <iostream>

/* --------------------------- Debug Print Component -------------------------- */
class DebugPrintComponent : public game::Component
{
public:
	using Component::Component;  // Inherit the base constructor
	~DebugPrintComponent();

	void Awake() override;
	void Start() override;
	void Update() override;
};

DebugPrintComponent::~DebugPrintComponent()
{
}

inline void DebugPrintComponent::Awake()
{
	std::cout << "Component-" << ObjectID() << "-Awake-Obj-" << Owner()->Name() << "\n";
}

inline void DebugPrintComponent::Start()
{
	std::cout << "Component-" << ObjectID() << "-Start-Obj-" << Owner()->Name() << "\n";
}

inline void DebugPrintComponent::Update()
{
	std::cout << "Component-" << ObjectID() << "-Update-Obj-" << Owner()->Name() << "\n";
}

/* ------------------------- Debug On Position Changed ------------------------- */
class DebugOnTransformChanged : public game::Component
{
private:
	/* data */
public:
	using Component::Component;  // Inherit the base constructor
	~DebugOnTransformChanged();

	void handleEvent();

	void Awake() override;
};

DebugOnTransformChanged::~DebugOnTransformChanged() {}

inline void DebugOnTransformChanged::handleEvent()
{
	std::cout << Owner()->Name() << "'s Transform Changed. Pos: "
		<< Owner()->GetTransform()->Position().ToString() << "\n";
}

inline void DebugOnTransformChanged::Awake()
{
	auto* transform = Owner()->GetComponent<game::Transform>();
	transform->OnChanged.Subscribe( this, &DebugOnTransformChanged::handleEvent );
}

#endif // DEBUG_PRINT_COMPONENT_H
