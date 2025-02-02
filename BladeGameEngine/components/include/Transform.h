#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Event.h"
#include "UniqueComponentTrait.h"
#include "Vector2.h"
#include "Mat2D.h"
#include "MathConstants.h"

namespace game
{

	class Transform : public Component
	{
	public:
		Transform( GameObject* owner );
		~Transform();

		Transform* Parent() { return parent; }

		// used by Sprite so that it is notified of a position change, that way
		// every game object doesn't need to check if it has a sprite each time
		// its position changes.
		Event<> OnChanged;

		/* -------------------------------------------------------------------------- */
		/*                  Local TRS( Position / Rotation / Scale )                  */
		/* -------------------------------------------------------------------------- */

		// The "authoritative" data global data is based on

		inline const Vector2 LocalPosition() const { return localMatrix.GetPosition(); }
		inline float LocalRotation() const { return localRotation; }       // in degrees
		inline const Vector2& LocalScale() const { return localScale; }


		// Set local position
		inline void SetLocalPosition( const Vector2& pos ) {

			// update local matrix so we don't have to do a full update
			localMatrix.m02 = pos.X();
			localMatrix.m12 = pos.Y();

			markChildrenDirty(); // child global's are dirty
		}
		inline void SetLocalPosition( float x, float y ) {

			localMatrix.m02 = x;
			localMatrix.m12 = y;

			markChildrenDirty();
		}

		// Set local rotation (in degrees)
		inline void SetLocalRotation( float rotDeg ) {
			localRotation = rotDeg;
			localMatrixDirty = true;
			markChildrenDirty();
		}

		// Set local scale
		inline void SetLocalScale( const Vector2& scl ) {
			localScale = scl;
			localMatrixDirty = true;
			markChildrenDirty();
		}
		inline void SetLocalScale( float sx, float sy ) {
			localScale.Set( sx, sy );
			localMatrixDirty = true;
			markChildrenDirty();
		}

		/* -------------------------------------------------------------------------- */
		/*                             GLOBAL TRS GETTERS                             */
		/* -------------------------------------------------------------------------- */

		inline Vector2 Position()
		{
			return GetGlobalMatrix().GetPosition();
		}

		/// @brief rotation of this transform
		/// @return global rotation in Radians
		inline float Rotation()
		{
			return GetGlobalMatrix().GetRotation();

		}

		/// @brief rotation of this transform
		/// @return global rotation in Degrees
		inline float RotationDegrees()
		{
			return GetGlobalMatrix().GetRotationDegrees();
		}

		inline Vector2 Scale()
		{
			return GetGlobalMatrix().GetScale();
		}

		/* -------------------------------- POSITION -------------------------------- */

		inline void SetPosition( const Vector2& newGlobalPos ) {
			Mat2D parentGlobal = parent->GetGlobalMatrix();
			Mat2D invParent;
			parentGlobal.Inverse( invParent ); // We assume it’s invertible

			// Build a tmp with our new global position but the same rotation, scale as before:
			float oldRotRad = Rotation(); // or localRotation? 
			float sx = localScale.X();
			float sy = localScale.Y();
			Mat2D tmp = Mat2D::FromTRS(
				newGlobalPos.X(),
				newGlobalPos.Y(),
				oldRotRad,
				sx, sy
			);

			// localMatrix = invParent * tmp
			Mat2D::Multiply( invParent, tmp, localMatrix );
			// then you'd decompose localMatrix into localPosition, localRotation, localScale, 
			// or you can keep the matrix approach, etc.

			// todo 

			localMatrixDirty = true;
			markChildrenDirty();
		}
		inline void SetPosition( float x, float y ) {

		}

		inline void Move( const Vector2& posDelta )
		{

		}

		inline void Move( float x, float y )
		{

		}

		// directional movement
		inline void Move( float distance, const Vector2& direction ) {

		}

		inline void MoveForward( float distance ) {

		}

		/* -------------------------------- ROTATION -------------------------------- */

		inline void SetRotation( float newGlobalRotation ) {

		}
		inline void Rotate( float newGlobalEulerAngle ) {

		}
		/// @brief Rotates this Transform (globally) so that it points in the specified direction. The Direction
		/// does not need to be normalized.
		/// @param direction 
		inline void SetDirection( const Vector2& newGlobalDirection ) {

		}
		inline void LookAt( const Vector2& targetPosition ) {

		}

		/* ---------------------------------- SCALE --------------------------------- */

		inline void SetGlobalScale( const Vector2& newGlobalScale ) {

		}

		/* -------------------------------------------------------------------------- */
		/*                               TRANSFORMATIONS                              */
		/* -------------------------------------------------------------------------- */

		// inline Vector2 TransformDirection( const Vector2& localDirection ) const {
		// 	float radians = rotation * (3.14159265358979323846f / 180.0f);
		// 	float cosTheta = std::cos( radians );
		// 	float sinTheta = std::sin( radians );
		// 	return Vector2(
		// 		localDirection.X() * cosTheta - localDirection.Y() * sinTheta,
		// 		localDirection.X() * sinTheta + localDirection.Y() * cosTheta
		// 	);
		// }

		// inline Vector2 InverseTransformDirection( const Vector2& worldDirection ) const {
		// 	float radians = -rotation * (3.14159265358979323846f / 180.0f);
		// 	float cosTheta = std::cos( radians );
		// 	float sinTheta = std::sin( radians );
		// 	return Vector2(
		// 		worldDirection.X() * cosTheta - worldDirection.Y() * sinTheta,
		// 		worldDirection.X() * sinTheta + worldDirection.Y() * cosTheta
		// 	);
		// }

		// inline Vector2 TransformPoint( const Vector2& localPoint ) const {
		// 	return TransformDirection( localPoint ) + position;
		// }

		// inline Vector2 InverseTransformPoint( const Vector2& worldPoint ) const {
		// 	return InverseTransformDirection( worldPoint - position );
		// }


		/* ------------------- Get the Global Matrix (2D) ---------------------------- */
		// Combine parent's globalMatrix with our localMatrix
		inline const Mat2D& GetGlobalMatrix()
		{
			if (globalMatrixDirty) {
				rebuildGlobalMatrix();
				globalMatrixDirty = false;
			}
			return globalMatrix;
		}

	private:
		Transform* parent;

		// Local TRS data
		// Vector2 localPosition; compose / decompose directly from matrix (easy)
		float   localRotation;   // degrees
		Vector2 localScale;

		// Cached local matrix (2x3)
		Mat2D   localMatrix;
		bool    localMatrixDirty;

		// Cached global matrix (2x3)
		Mat2D   globalMatrix;
		bool    globalMatrixDirty;

		// Rebuilds localMatrix from localRotation, localScale
		inline void rebuildLocalMatrix()
		{
			localMatrix = Mat2D::FromTRS(
				localMatrix.m02, localMatrix.m12,
				localRotation,
				localScale.X(), localScale.Y()
			);
		}

		// Rebuilds globalMatrix by multiplying parent's globalMatrix * localMatrix
		// If no parent, globalMatrix = localMatrix
		inline void rebuildGlobalMatrix()
		{
			// ensure local is up to date
			if (localMatrixDirty) {
				rebuildLocalMatrix();
				localMatrixDirty = false;
			}
			if (parent) {
				const Mat2D& parentMat = parent->GetGlobalMatrix();
				Mat2D::Multiply( parentMat, localMatrix, globalMatrix );
			}
			else {
				globalMatrix = localMatrix;
			}
		}


		void markChildrenDirty();
	};

} // namespace game

MAKE_COMPONENT_UNIQUE( game::Transform );

#endif /*TRANSFORM_H*/
