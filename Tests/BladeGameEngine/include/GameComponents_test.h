#ifndef DEBUG_PRINT_COMPONENT_H
#define DEBUG_PRINT_COMPONENT_H

#include "Component.h"
#include "GameObject.h"

#include <iostream>


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


#endif // DEBUG_PRINT_COMPONENT_H
