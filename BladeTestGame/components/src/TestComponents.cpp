
#include "TestComponents.h"
#include "GameObject.h"


/* -------------------------------------------------------------------------- */
/*                             ObjectUpdatePrinter                            */
/* -------------------------------------------------------------------------- */

void ObjectUpdatePrinter::Awake() {

	std::cout << "*COMPONENT* Object Awake: " << Owner()->Name() << std::endl;
}

void ObjectUpdatePrinter::Start() {

	std::cout << "*COMPONENT* Object Start: " << Owner()->Name() << std::endl;
}

void ObjectUpdatePrinter::Update() {

	std::cout << "*COMPONENT* Object Updated: " << Owner()->Name() << std::endl;
}

/* -------------------------------------------------------------------------- */
/*                           TransformChangedPrinter                          */
/* -------------------------------------------------------------------------- */

void TransformChangedPrinter::handleEvent() {
	std::cout << Owner()->Name() << "'s Transform Changed. Pos: "
		<< Owner()->GetTransform()->Position().ToString() << "\n";
}

void TransformChangedPrinter::Awake() {
	auto* transform = Owner()->GetComponent<game::Transform>();
	transform->OnChanged.Subscribe( this, &TransformChangedPrinter::handleEvent );
}