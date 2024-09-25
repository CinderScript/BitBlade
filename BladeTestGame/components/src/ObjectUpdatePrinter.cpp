
#include "ObjectUpdatePrinter.h"
#include "GameObject.h"


void ObjectUpdatePrinter::Awake() {

	std::cout << "*COMPONENT* Object Awake: " << Owner()->Name() << std::endl;
}

void ObjectUpdatePrinter::Start() {

	std::cout << "*COMPONENT* Object Start: " << Owner()->Name() << std::endl;
}

void ObjectUpdatePrinter::Update() {

	std::cout << "*COMPONENT* Object Updated: " << Owner()->Name() << std::endl;
}