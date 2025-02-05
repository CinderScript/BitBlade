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

		inline void SetPosition( float x, float y ) {
			if (parent) {

				parent->GetGlobalMatrix().InverseTransformPoint(
					x,
					y,
					localMatrix.m02,
					localMatrix.m12 );
			}
			else {
				localMatrix.m02 = x;
				localMatrix.m12 = y;
			}

			globalMatrixDirty = true;
			markChildrenDirty();
		}
		inline void SetPosition( const Vector2& newGlobalPos )
		{
			SetPosition( newGlobalPos.X(), newGlobalPos.Y() );
		}

		inline void Move( float x, float y )
		{
			// find new global position and use SetPosition
			SetPosition( globalMatrix.m02 + x, globalMatrix.m12 + y );
		}

		inline void Move( const Vector2& posDelta )
		{
			Move( posDelta.X(), posDelta.Y() );
		}

		inline void Move( float distance, const Vector2& direction ) {
			// Ensure direction is normalized (avoid scaling issues)
			Vector2 normalizedDir = Vector2::Normalized( direction );

			// Compute global movement delta
			Vector2 delta = normalizedDir * distance;

			// Move using the global Move method
			Move( delta );
		}

		inline void MoveForward( float distance ) {

		}

		/* -------------------------------- ROTATION -------------------------------- */

		inline void SetRotation( float newGlobalRotation ) {
			if (parent) {
				// Compute local rotation by subtracting parent's global rotation
				float parentRotation = parent->Rotation();
				localRotation = newGlobalRotation - parentRotation;
			}
			else {
				// No parent: local rotation is the same as global rotation
				localRotation = newGlobalRotation;
			}

			rebuildLocalMatrix();

			// Mark global transform as dirty (will be recalculated)
			globalMatrixDirty = true;
			markChildrenDirty();
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

		inline Vector2 TransformDirection( const Vector2& localDirection ) const {
			const Mat2D& gm = GetGlobalMatrix();
			// Only use the linear part (m00, m01, m10, m11)
			return Vector2(
				gm.m00 * localDirection.X() + gm.m01 * localDirection.Y(),
				gm.m10 * localDirection.X() + gm.m11 * localDirection.Y()
			);
		}

		inline Vector2 InverseTransformDirection( const Vector2& worldDirection ) const {
			Mat2D inv = GetGlobalMatrix().Inverse();  // This includes translation,
			// but for a pure direction, we ignore the translation component.
			return Vector2(
				inv.m00 * worldDirection.X() + inv.m01 * worldDirection.Y(),
				inv.m10 * worldDirection.X() + inv.m11 * worldDirection.Y()
			);
		}


		inline Vector2 TransformPoint( const Vector2& localPoint ) const {
			float x, y;
			GetGlobalMatrix().TransformPoint( localPoint.X(), localPoint.Y(), x, y );
			return Vector2( x, y );
		}


		inline Vector2 InverseTransformPoint( const Vector2& worldPoint ) const {
			float x, y;
			GetGlobalMatrix().InverseTransformPoint( worldPoint.X(), worldPoint.Y(), x, y );
			return Vector2( x, y );
		}



		/* -------------------------------------------------------------------------- */
		/*                                 Directions                                 */
		/* -------------------------------------------------------------------------- */

		inline Vector2 Forward() const {
			// Transform local UP (0, -1) by the global matrix
			Vector2 globalForward = globalMatrix.TransformPoint( Vector2::Up() );

			// Subtract the translation (since TransformPoint includes it)
			globalForward.Sub( globalMatrix.m02, globalMatrix.m12 );

			// Normalize to ensure consistency
			return globalForward.Normalize();
		}

		// Other directional vectors
		inline Vector2 Up() const { return Forward(); }
		inline Vector2 Down() const { return -Forward(); }
		inline Vector2 Right() const { return Vector2( -Forward().Y(), Forward().X() ); }
		inline Vector2 Left() const { return Vector2( Forward().Y(), -Forward().X() ); }

		/* ------------------- Get the Global Matrix (2D) ---------------------------- */
		inline const Mat2D& GetGlobalMatrix() const
		{
			if (globalMatrixDirty) {
				// Use const_cast to modify the cache even though this is a const function.
				const_cast<Transform*>(this)->rebuildGlobalMatrix();
				const_cast<Transform*>(this)->globalMatrixDirty = false;
			}
			return globalMatrix;
		}

	private:
		Transform* parent;

		// Local TRS data
		// Vector2 localPosition; compose / decompose directly from matrix (easy)
		float   localRotation;   // degrees
		Vector2 localScale;

		// local matrix (2x3)
		Mat2D   localMatrix;
		bool    localMatrixDirty;

		// Cached global matrix (2x3)
		mutable Mat2D   globalMatrix;
		mutable bool    globalMatrixDirty;

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
