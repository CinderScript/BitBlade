#ifndef POSITION_H
#define POSITION_H

#include "Vector2.h"
#include "Component.h"
#include "Event.h"

namespace game
{

	struct Transform : public Component
	{
	public:
		Transform( GameObject* owner );
		~Transform();

		Event<> OnChanged;

		const Vector2& Position() const { return position; }
		const float Rotation() const { return rotation; }
		const Vector2& Scale() const { return scale; }
		const Vector2& Forward() const { return forward; }

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



		inline void SetScale( const Vector2& scl ) {
			scale = scl;
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

		inline void Forward( float distance ) {
			position += forward * distance;
			OnChanged.Invoke();
		}

		// Clockwise rotation
		inline void Rotate( float eulerAngle ) {
			rotation += eulerAngle;
			normalizeRotation();
			updateForwardVector();
			OnChanged.Invoke();
		}

	private:
		Vector2 position;
		float rotation;
		Vector2 scale;
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


#endif /*POSITION_H*/
