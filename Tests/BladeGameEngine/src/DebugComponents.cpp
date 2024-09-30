

#include "DebugComponents.h"
#include "GameObject.h"
#include "Transform.h"

/* --------------------------- Debug Print Component -------------------------- */

DebugPrintComponent::~DebugPrintComponent()
{
}

void DebugPrintComponent::Awake()
{
	std::cout << "Component-" << ObjectID() << "-Awake-Obj-" << Owner()->Name() << "\n";
}

void DebugPrintComponent::Start()
{
	std::cout << "Component-" << ObjectID() << "-Start-Obj-" << Owner()->Name() << "\n";
}

void DebugPrintComponent::Update()
{
	std::cout << "Component-" << ObjectID() << "-Update-Obj-" << Owner()->Name() << "\n";
}

/* ------------------------- Debug On Position Changed ------------------------- */

DebugOnTransformChanged::~DebugOnTransformChanged() {}

void DebugOnTransformChanged::handleEvent()
{
	std::cout << Owner()->Name() << "'s Transform Changed. Pos: "
		<< Owner()->GetTransform()->Position().ToString() << "\n";
}

void DebugOnTransformChanged::Awake()
{
	auto* transform = Owner()->GetComponent<game::Transform>();
	transform->OnChanged.Subscribe( this, &DebugOnTransformChanged::handleEvent );
}