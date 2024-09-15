/* Vector2
*/

#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <cmath>

struct Vector2 {

private:
	float x;
	float y;

public:

	Vector2() : x( 0 ), y( 0 ) {}
	Vector2( float X, float Y ) : x( X ), y( Y ) {}

	float X() const { return x; }
	float Y() const { return y; }

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

};

#endif // VECTOR_2_H
