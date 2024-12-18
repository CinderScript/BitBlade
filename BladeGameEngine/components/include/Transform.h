#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2.h"
#include "Component.h"
#include "Event.h"
#include "UniqueComponentTrait.h"

namespace game
{

	class Transform : public Component
	{
	public:
		Transform( GameObject* owner );
		~Transform();

		Event<> OnChanged;

		inline const Vector2& Position() const { return position; }
		inline float Rotation() const { return rotation; }
		inline const Vector2& Scale() const { return scale; }
		inline Vector2 Up() const { return forward; }
		inline Vector2 Down() const { return Vector2( -forward ); }
		inline Vector2 Right() const { return Vector2( -forward.Y(), forward.X() ); }
		inline Vector2 Left() const { return Vector2( forward.Y(), -forward.X() ); }

		inline void SetPosition( const Vector2& pos ) {
			position = pos;
			OnChanged.Invoke();
		}
		inline void SetPosition( float x, float y ) {
			position.Set( x, y );
			OnChanged.Invoke();
		}

		inline void SetRotation( float rot ) {
			rotation = rot;
			normalizeRotation();
			updateForwardVector();
			OnChanged.Invoke();
		}

		inline void SetDirection( const Vector2& direction ) {
			Vector2 normalizedDir = Vector2::Normalize( direction );
			rotation = std::atan2( normalizedDir.Y(), normalizedDir.X() ) * (180.0f / 3.14159265358979323846f);
			normalizeRotation();
			forward = normalizedDir;
			OnChanged.Invoke();
		}

		inline void LookAt( const Vector2& targetPosition ) {
			Vector2 direction = targetPosition - position;
			SetDirection( direction );
		}

		// Clockwise rotation
		inline void Rotate( float eulerAngle ) {
			rotation += eulerAngle;
			normalizeRotation();
			updateForwardVector();
			OnChanged.Invoke();
		}

		inline void Move( const Vector2& delta ) {
			position += delta;
			OnChanged.Invoke();
		}
		inline void Move( float x, float y ) {
			position.Add( x, y );
			OnChanged.Invoke();
		}

		// directional movement
		inline void Move( float distance, const Vector2& direction ) {
			Vector2 normalizedDir = Vector2::Normalize( direction );
			position += normalizedDir * distance;
			OnChanged.Invoke();
		}

		inline void MoveForward( float distance ) {
			position += forward * distance;
			OnChanged.Invoke();
		}

		inline void SetScale( const Vector2& scl ) {
			scale = scl;
			OnChanged.Invoke();
		}
		inline void ScaleBy( const Vector2& scaleFactor ) {
			scale.Mult( scaleFactor );
			OnChanged.Invoke();
		}


		inline Vector2 TransformDirection( const Vector2& localDirection ) const {
			float radians = rotation * (3.14159265358979323846f / 180.0f);
			float cosTheta = std::cos( radians );
			float sinTheta = std::sin( radians );
			return Vector2(
				localDirection.X() * cosTheta - localDirection.Y() * sinTheta,
				localDirection.X() * sinTheta + localDirection.Y() * cosTheta
			);
		}

		inline Vector2 InverseTransformDirection( const Vector2& worldDirection ) const {
			float radians = -rotation * (3.14159265358979323846f / 180.0f);
			float cosTheta = std::cos( radians );
			float sinTheta = std::sin( radians );
			return Vector2(
				worldDirection.X() * cosTheta - worldDirection.Y() * sinTheta,
				worldDirection.X() * sinTheta + worldDirection.Y() * cosTheta
			);
		}

		inline Vector2 TransformPoint( const Vector2& localPoint ) const {
			return TransformDirection( localPoint ) + position;
		}

		inline Vector2 InverseTransformPoint( const Vector2& worldPoint ) const {
			return InverseTransformDirection( worldPoint - position );
		}


	private:
		Vector2 position;
		Vector2 localPosition;
		float rotation;
		float localRotation;
		Vector2 scale;
		Vector2 localScale;
		Vector2 forward;

		inline void updateForwardVector() {
			float radians = rotation * (3.14159265358979323846f / 180.0f); // Convert degrees to radians
			forward.Set( std::cos( radians ), std::sin( radians ) );
			forward.Normalize(); // Ensure it's a unit vector
		}

		inline void normalizeRotation() {
			rotation = std::fmod( rotation, 360.0f );
			if (rotation < 0.0f) {
				rotation += 360.0f;
			}
		}
	};

} // namespace game

MAKE_COMPONENT_UNIQUE( game::Transform );

#endif /*TRANSFORM_H*/
