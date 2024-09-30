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

		void SetPosition( const Vector2& pos ) {
			position = pos;
			OnChanged.Invoke();
		}
		void SetPosition( float x, float y ) {
			position.Set( x, y );
			OnChanged.Invoke();
		}

		void SetRotation( float rot ) {
			rotation = rot;
			updateForwardVector();
			OnChanged.Invoke();
		}


		void SetScale( const Vector2& scl ) {
			scale = scl;
			OnChanged.Invoke();
		}

		void Move( const Vector2& delta ) {
			position.Add( delta );
			OnChanged.Invoke();
		}
		void Move( float x, float y ) {
			position.Add( x, y );
			OnChanged.Invoke();
		}

		// directional movement
		void Move( float distance, const Vector2& direction ) {
			Vector2 normalizedDir = Vector2::Normalize( direction );
			position.Add( normalizedDir * distance );
			OnChanged.Invoke();
		}

		void Forward( float distance ) {
			position.Add( forward * distance );
			OnChanged.Invoke();
		}

		// Clockwise rotation
		void Rotate( float eulerAngle ) {
			rotation += eulerAngle;
			// Ensure rotation stays within [0, 360) range
			if (rotation >= 360.0f) rotation -= 360.0f;
			else if (rotation < 0.0f) rotation += 360.0f;
			updateForwardVector();
			OnChanged.Invoke();
		}




	private:
		Vector2 position;
		float rotation;
		Vector2 scale;
		Vector2 forward;

		void updateForwardVector() {
			float radians = rotation * (3.14159265358979323846f / 180.0f); // Convert degrees to radians
			forward.Set( std::cos( radians ), std::sin( radians ) );
			forward.Normalize(); // Ensure it's a unit vector
		}
	};

} // namespace game


#endif /*POSITION_H*/
