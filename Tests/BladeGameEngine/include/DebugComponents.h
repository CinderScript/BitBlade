#ifndef DEBUGCOMPONENTS_H
#define DEBUGCOMPONENTS_H

#include "Component.h"
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


#endif /*DEBUGCOMPONENTS_H*/
