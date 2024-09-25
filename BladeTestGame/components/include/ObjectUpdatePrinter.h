#ifndef OBJECT_UPDATE_PRINTER_H
#define OBJECT_UPDATE_PRINTER_H

#include "Component.h"
#include "GameObject.h"

#include <iostream>


class ObjectUpdatePrinter : public game::Component
{
private:
	/* data */
public:
	using Component::Component;  // Inherit the base constructor

	~ObjectUpdatePrinter() override = default;  // Use default destructor

	void Awake() override;
	void Start() override;
	void Update() override;
};


#endif // OBJECT_UPDATE_PRINTER_H
