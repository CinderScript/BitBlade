/* Vector2
*/

#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <cmath>
#include <string>
#include <sstream>

namespace game {
	struct Vector2 {

	private:
		float x;
		float y;

	public:
		constexpr Vector2() : x( 0 ), y( 0 ) {}
		constexpr Vector2( float x, float y ) : x( x ), y( y ) {}
		constexpr Vector2( const Vector2& other ) : x( other.x ), y( other.y ) {}

		constexpr float X() const { return x; }
		constexpr float Y() const { return y; }

		void SetX( float x ) { this->x = x; }
		void SetY( float y ) { this->y = y; }
		void Set( float x, float y ) { this->x = x, this->y = y; }


		// Add methods
		void Add( float x, float y ) {
			this->x += x;
			this->y += y;
		}
		void Add( const Vector2& other ) {
			this->x += other.x;
			this->y += other.y;
		}

		// Subtract methods
		void Sub( float x, float y ) {
			this->x -= x;
			this->y -= y;
		}
		void Sub( const Vector2& other ) {
			this->x -= other.x;
			this->y -= other.y;
		}

		// Multiply methods
		void Mult( float scale ) {
			this->x *= scale;
			this->y *= scale;
		}
		void Mult( float x, float y ) {
			this->x *= x;
			this->y *= y;
		}
		void Mult( const Vector2& other ) {
			this->x *= other.x;
			this->y *= other.y;
		}

		// Divide methods
		void Div( float scale ) {
			if (scale != 0) {
				this->x /= scale;
				this->y /= scale;
			}
			else {
				// Handle division by zero appropriately
				this->x = 0;
				this->y = 0;
			}
		}
		Vector2& Normalize() {
			float len = Length();
			if (len != 0) {
				x /= len;
				y /= len;
			}

			return *this;
		}

		constexpr float Length() const {
			return std::sqrt( x * x + y * y );
		}

		constexpr bool operator==( const Vector2& rhs ) const {
			return x == rhs.x && y == rhs.y;
		}
		constexpr bool operator!=( const Vector2& rhs ) const {
			return !(*this == rhs);
		}

		// Operator overloading for vector addition
		constexpr Vector2 operator+( const Vector2& rhs ) const {
			return { static_cast<float>(x + rhs.x), static_cast<float>(y + rhs.y) };
		}
		// Operator overloading for vector subtraction
		constexpr Vector2 operator-( const Vector2& rhs ) const {
			return { static_cast<float>(x - rhs.x), static_cast<float>(y - rhs.y) };
		}
		// Operator overloading for vector multiplication by an integer
		constexpr Vector2 operator*( int scalar ) const {
			return { static_cast<float>(x * scalar), static_cast<float>(y * scalar) };
		}
		// Operator overloading for vector multiplication by a float
		constexpr Vector2 operator*( float scalar ) const {
			return { x * scalar, y * scalar };
		}
		// Operator overloading for vector division by a float
		constexpr Vector2 operator/( float scalar ) const {
			if (scalar == 0) {
				// Handle division by zero appropriately
				return { 0, 0 };
			}
			return { x / scalar, y / scalar };
		}

		// Compound addition
		constexpr Vector2& operator+=( const Vector2& rhs ) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		// Compound subtraction
		constexpr Vector2& operator-=( const Vector2& rhs ) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		// Compound multiplication
		constexpr Vector2& operator*=( float scalar ) {
			x *= scalar;
			y *= scalar;
			return *this;
		}

		// Compound division
		constexpr Vector2& operator/=( float scalar ) {
			if (scalar != 0) {
				x /= scalar;
				y /= scalar;
			}
			return *this;
		}

		const std::string ToString() const {
			std::ostringstream oss;
			oss << "(" << x << ", " << y << ")";
			return oss.str(); // RVO and move semantics optimize this
		}

		/* ---------------------------- STATIC FUNCTIONS ---------------------------- */

		static constexpr Vector2 Up() {
			return Vector2( 0, -1 );
		}
		static constexpr Vector2 Down() {
			return Vector2( 0, 1 );
		}
		static constexpr Vector2 Right() {
			return Vector2( 1, 0 );
		}
		static constexpr Vector2 Left() {
			return Vector2( -1, 0 );
		}

		static constexpr  float Dot( const Vector2& a, const Vector2& b ) {
			return (a.X() * b.X()) + (a.Y() * b.Y());
		}

		static constexpr float Cross( const Vector2& a, const Vector2& b ) {
			return (a.X() * b.Y()) - (a.Y() * b.X());
		}

		static constexpr float AngleBetween( const Vector2& a, const Vector2& b ) {
			float dot = Dot( a, b );
			float lengths = a.Length() * b.Length();
			if (lengths == 0) return 0.0f;
			return std::acos( dot / lengths ) * (180.0f / 3.14159265358979323846f); // degrees
		}

		static constexpr Vector2 Lerp( const Vector2& a, const Vector2& b, float t ) {
			return Vector2(
				a.X() + t * (b.X() - a.X()),
				a.Y() + t * (b.Y() - a.Y())
			);
		}

		static constexpr float Distance( const Vector2& a, const Vector2& b ) {
			return std::sqrt( (b.X() - a.X()) * (b.X() - a.X()) + (b.Y() - a.Y()) * (b.Y() - a.Y()) );
		}

		static constexpr Vector2 Normalize( Vector2 vector ) {
			float len = vector.Length();
			if (len == 0) return { 0, 0 };  // or handle zero-length vector appropriately
			return { static_cast<float>(vector.x / len), static_cast<float>(vector.y / len) };
		}

		static constexpr Vector2 Project( const Vector2& a, const Vector2& b ) {
			float dotProduct = Dot( a, b );
			float lengthSquared = b.X() * b.X() + b.Y() * b.Y();
			if (lengthSquared == 0) return Vector2( 0, 0 );
			float scalar = dotProduct / lengthSquared;
			return Vector2( b.X() * scalar, b.Y() * scalar );
		}

		static constexpr Vector2 Reflect( const Vector2& direction, const Vector2& normal ) {
			return direction - normal * 2.0f * Dot( direction, normal );
		}

	};
}

#endif // VECTOR_2_H
