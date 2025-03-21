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

	/// @brief Provides local and global position, rotation, and scale. 
	class Transform : public Component
	{
	public:
		// sets parent transform when GameObject sets a new parent
		friend class GameObject;


		Transform( GameObject* owner );
		~Transform();

		Transform* Parent() { return parent; }

		// used by Texture so that it is notified of a position change, that way
		// every game object doesn't need to check if it has a sprite each time
		// its position changes.
		Event<> OnChanged;

		/* -------------------------------------------------------------------------- */
		/*                  Local TRS( Position / Rotation / Scale )                  */
		/* -------------------------------------------------------------------------- */

		// The "authoritative" data global data is based on

		/// @brief Returns the current local position of this Transform
		/// @return Local Position
		inline const Vector2 LocalPosition() const { return localMatrix.GetPosition(); }

		/// @brief Returns the Local Rotation in radians of this Transform
		/// @return Local Rotation (degrees)
		inline float LocalRotation() const { return localRotation * RAD2DEG; }

		/// @brief Returns the Local Rotation in radians of this Transform
		/// @return Local Rotation (radians)
		inline float LocalRotationRad() const { return localRotation; }

		/// @brief Returns the local scale of this Transform
		/// @return Local Scale
		inline const Vector2& LocalScale() const { return localScale; }


		// Set local position
		inline void SetLocalPosition( const Vector2& pos ) {

			// update local matrix so we don't have to do a full update
			localMatrix.m02 = pos.X();
			localMatrix.m12 = pos.Y();

			markGlobalMatrixDirtyBFS(); // child global's are dirty
		}
		inline void SetLocalPosition( float x, float y ) {

			localMatrix.m02 = x;
			localMatrix.m12 = y;

			markGlobalMatrixDirtyBFS();
		}

		/// @brief Set the local rotation of this transform (radians)
		/// @param newLocalRotationRadians 
		inline void SetLocalRotationRad( float newLocalRotationRadians ) {
			localRotation = newLocalRotationRadians;
			localMatrixDirty = true;
			markGlobalMatrixDirtyBFS();
		}

		/// @brief Set the local rotation of this transform (degrees)
		/// @param nnewLocalRotationDegrees 
		inline void SetLocalRotation( float nnewLocalRotationDegrees ) {
			SetLocalRotationRad( nnewLocalRotationDegrees * DEG2RAD );
		}

		// Set local scale
		inline void SetLocalScale( const Vector2& newLocalScale ) {
			localScale = newLocalScale;
			localMatrixDirty = true;
			markGlobalMatrixDirtyBFS();
		}
		inline void SetLocalScale( float sx, float sy ) {
			localScale.Set( sx, sy );
			localMatrixDirty = true;
			markGlobalMatrixDirtyBFS();
		}

		/* -------------------------------------------------------------------------- */
		/*                             GLOBAL TRS GETTERS                             */
		/* -------------------------------------------------------------------------- */

		/// @brief The Global Position of this transform
		/// @return 
		inline Vector2 Position()
		{
			return GetGlobalMatrix().GetPosition();
		}

		/// @brief The Global Rotation of this transform
		/// @return Global Rotation (degrees)
		inline float Rotation()
		{
			return GetGlobalMatrix().GetRotationDegrees();
		}

		/// @brief The Global Rotation of this transform
		/// @return Global Rotation (radians)
		inline float RotationRad()
		{
			return GetGlobalMatrix().GetRotation();
		}

		/// @brief The global Scale of this transform
		/// @return Scale (global)
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
			markGlobalMatrixDirtyBFS();
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

		/// @brief Moves this transform in the given direction the given distance. The direction
		/// Vector2 is normalized in this function.
		/// @param distance 
		/// @param direction 
		inline void Move( float distance, const Vector2& direction ) {
			Vector2 normalizedDir = Vector2::Normalized( direction );
			Vector2 delta = normalizedDir * distance;
			Move( delta );
		}

		/// @brief Moves this transform according to its Forward (Up) direction.
		/// @param distance 
		inline void MoveForward( float distance ) {
			SetPosition( Position() + (Forward() * distance) );
		}

		/* -------------------------------- ROTATION -------------------------------- */

		/// @brief Sets the Global Rotation of this transform
		/// @param Global Rotation (radians) 
		inline void SetRotationRad( float newGlobalRotation ) {
			if (parent) {
				// Compute local rotation by subtracting parent's global rotation
				float parentRotation = parent->RotationRad();
				localRotation = newGlobalRotation - parentRotation;
			}
			else {
				// No parent: local rotation is the same as global rotation
				localRotation = newGlobalRotation;
			}

			rebuildLocalMatrix();

			// Mark global transform as dirty (will be recalculated)
			globalMatrixDirty = true;
			markGlobalMatrixDirtyBFS();
		}

		/// @brief Sets the Global Rotation of this transform
		/// @param Global Rotation (degrees) 
		inline void SetRotation( float newGlobalRotationDegrees ) {
			SetRotationRad( newGlobalRotationDegrees * DEG2RAD );
		}

		/// @brief Rotates this transform by radians
		/// @param newGlobalEulerAngle Rotation added (radians)
		inline void RotateRad( float rotationDeltaRads ) {
			SetRotationRad( RotationRad() + rotationDeltaRads );
		}

		/// @brief Rotates this transform by degrees
		/// @param newGlobalEulerAngle Rotation added (degrees)
		inline void Rotate( float newGlobalEulerAngle ) {
			RotateRad( newGlobalEulerAngle * DEG2RAD );
		}

		/// @brief Rotates this Transform (globally) so that it points in the specified direction. The Direction
		/// does not need to be normalized.
		/// @param direction 
		inline void SetDirection( const Vector2& newGlobalDirection ) {
			Vector2 dir = Vector2::Normalized( newGlobalDirection );
			// Calculate the desired global rotation in radians.
			SetRotationRad( std::atan2( dir.X(), -dir.Y() ) );
		}

		/// @brief Rotates this transform so that it faces the target position
		/// @param targetPosition 
		inline void LookAt( const Vector2& targetPosition ) {
			Vector2 currentPos = Position();
			Vector2 dir = targetPosition - currentPos;
			if (dir.Length() > 0.0001f) { // Avoid division by zero.
				SetDirection( dir );
			}
		}

		/* ---------------------------------- SCALE --------------------------------- */

		/// @brief Sets the Global Scale of this Transform. Modifies the local scale so that
		/// the Parent's global scale * this local scale = the desired scale.
		/// @param newGlobalScale 
		inline void SetGlobalScale( const Vector2& newGlobalScale ) {
			if (parent) {
				Vector2 parentGlobalScale = parent->Scale();
				// Compute the new local scale by component‐wise division.
				// (Assuming parent's scale components are nonzero.)
				localScale.Set( newGlobalScale.X() / parentGlobalScale.X(),
					newGlobalScale.Y() / parentGlobalScale.Y() );
			}
			else {
				localScale = newGlobalScale;
			}

			rebuildLocalMatrix();

			globalMatrixDirty = true;
			markGlobalMatrixDirtyBFS();
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

		/// @brief The direction this transform is pointing. Defined by the transform's Up
		/// direction. This value is normalized.
		/// @return Upward vector (normalized)
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
		GameObject* owner;

		// Local TRS data
		// Vector2 localPosition; compose / decompose directly from matrix (easy)
		float   localRotation;   // radians
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

		/// @brief Marks this Transform's global matrix as dirty as well as 
		///	all of the owning GameObject's childrens' Transforms
		void markGlobalMatrixDirtyBFS();
	};

} // namespace game

MAKE_COMPONENT_UNIQUE( game::Transform );

#endif /*TRANSFORM_H*/
