/* Transform.cpp
*/

#include "Transform.h"
#include "GameObject.h"

#include <queue>


using std::queue;

namespace game
{
	Transform::Transform( GameObject* owner )
		: Component( owner ),
		localRotation( 0.0f ), localScale( 1.0f, 1.0f ),
		localMatrixDirty( false ), globalMatrixDirty( false ),
		owner( owner ),
		parent( nullptr )
	{
		// if the owning GameObject has a parent, assign the parent's transform
		if (owner->Parent()) {
			parent = owner->Parent()->GetTransform();
		}

		localMatrix.m02 = 0;
		localMatrix.m12 = 0;
		rebuildLocalMatrix();
		rebuildGlobalMatrix();
	}

	Transform::~Transform() {}



	/* -------------------------------------------------------------------------- */
	/*                                 MAKE DIRTY                                 */
	/* -------------------------------------------------------------------------- */


	/// @brief Marks all child positions as dirty
	void Transform::markGlobalMatrixDirtyBFS() {

		globalMatrixDirty = true;

		std::queue<GameObject*> queue;
		for (auto* child : owner->GetChildren()) {
			queue.push( child );
		}

		while (!queue.empty()) {
			GameObject* currentChild = queue.front();
			queue.pop();

			Transform* childTransform = currentChild->GetTransform();
			childTransform->globalMatrixDirty = true;

			for (auto* grandChild : currentChild->GetChildren()) {
				queue.push( grandChild );
			}
		}
	}

} // namespace game