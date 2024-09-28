/* Vector2
*/

#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <cmath>

namespace game {
	struct Vector2 {

	private:
		float x;
		float y;

	public:

		Vector2() : x( 0 ), y( 0 ) {}
		Vector2( float x, float y ) : x( x ), y( y ) {}
		Vector2( const Vector2& other ) : x( other.x ), y( other.y ) {}

		float X() const { return x; }
		float Y() const { return y; }

		void SetX( float x ) { this->x = x; }
		void SetY( float y ) { this->y = y; }


		float Length() const {
			return std::sqrt( x * x + y * y );
		}

		Vector2 Normalize() const {
			float len = Length();
			if (len == 0) return { 0, 0 };  // or handle zero-length vector appropriately
			return { static_cast<float>(x / len), static_cast<float>(y / len) };
		}


		bool operator==( const Vector2& rhs ) const {
			return x == rhs.x && y == rhs.y;
		}
		bool operator!=( const Vector2& rhs ) const {
			return !(*this == rhs);
		}

		// Operator overloading for vector addition
		Vector2 operator+( const Vector2& rhs ) const {
			return { static_cast<float>(x + rhs.x), static_cast<float>(y + rhs.y) };
		}
		// Operator overloading for vector subtraction
		Vector2 operator-( const Vector2& rhs ) const {
			return { static_cast<float>(x - rhs.x), static_cast<float>(y - rhs.y) };
		}
		// Operator overloading for vector multiplication by an integer
		Vector2 operator*( int scalar ) const {
			return { static_cast<float>(x * scalar), static_cast<float>(y * scalar) };
		}
		// Operator overloading for vector multiplication by a float
		Vector2 operator*( float scalar ) const {
			return { x * scalar, y * scalar };
		}
		// Operator overloading for vector division by a float
		Vector2 operator/( float scalar ) const {
			if (scalar == 0) {
				// Handle division by zero appropriately
				return { 0, 0 };
			}
			return { x / scalar, y / scalar };
		}

		// Compound addition
		Vector2& operator+=( const Vector2& rhs ) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		// Compound subtraction
		Vector2& operator-=( const Vector2& rhs ) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		// Compound multiplication
		Vector2& operator*=( float scalar ) {
			x *= scalar;
			y *= scalar;
			return *this;
		}

		// Compound division
		Vector2& operator/=( float scalar ) {
			if (scalar != 0) {
				x /= scalar;
				y /= scalar;
			}
			return *this;
		}
	};
}

#endif // VECTOR_2_H
