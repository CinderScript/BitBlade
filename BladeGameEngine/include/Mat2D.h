#ifndef MAT2D_H
#define MAT2D_H

#include "MathConstants.h"
#include "Vector2.h"

#include <cmath>

namespace game
{
	//
	// A 2D transform matrix in row-major format:
	//
	// [ m00 m01 m02 ]
	// [ m10 m11 m12 ]
	//
	// This can represent translation, rotation (IN RADIANS), and scale in 2D.
	//
	// By convention:
	//   (m00, m01) = rotated and scaled X-axis
	//   (m10, m11) = rotated and scaled Y-axis
	//   (m02, m12) = translation
	//
	struct Mat2D
	{
		float m00, m01, m02;  // Row 0
		float m10, m11, m12;  // Row 1

		/// @brief Constructor - Initializes to Identity
		constexpr Mat2D()
			: m00( 1.f ), m01( 0.f ), m02( 0.f ),
			m10( 0.f ), m11( 1.f ), m12( 0.f )
		{}


		inline constexpr Vector2 GetPosition() const {
			return Vector2( m02, m12 );
		}
		/// @brief We approximate the angle by looking at the rotated X-axis. 
		/// @return rotation in radians
		inline constexpr float GetRotation() const {
			return std::atan2( m10, m00 );
		}
		/// @brief We approximate the angle by looking at the rotated X-axis. 
		/// @return rotation in degrees
		inline constexpr float GetRotationDegrees() const {
			return GetRotation() * RAD2DEG;
		}
		inline constexpr Vector2 GetScale() const {
			return Vector2(
				std::sqrt( m00 * m00 + m10 * m10 ),  // X scale
				std::sqrt( m01 * m01 + m11 * m11 )   // Y scale
			);
		}

		/// @brief Return an identity matrix
		/// @return identity matrix
		static constexpr inline Mat2D Identity()
		{
			return Mat2D(); // Uses default constructor
		}

		/// @brief Builds a matrix from translation (tx, ty), rotation (in RADIANS), and scale (sx, sy).
		///
		/// Matrix layout if we do scale then rotation then translation:
		///   [ sx*cos(r)   -sy*sin(r)   tx ]
		///   [ sx*sin(r)    sy*cos(r)   ty ]
		/// @param tx 
		/// @param ty 
		/// @param rotationRad 
		/// @param sx 
		/// @param sy 
		/// @return 
		static inline constexpr Mat2D FromTRS( float tx, float ty, float rotationRad, float sx, float sy )
		{
			Mat2D m;
			float cosT = std::cos( rotationRad );
			float sinT = std::sin( rotationRad );

			m.m00 = cosT * sx;
			m.m01 = -sinT * sy;
			m.m02 = tx;

			m.m10 = sinT * sx;
			m.m11 = cosT * sy;
			m.m12 = ty;

			return m;
		}

		/// @brief Multiply: out = A * B  (2x3 * 2x3 matrix)
		/// @param A 
		/// @param B 
		/// @param out 
		static inline constexpr void Multiply( const Mat2D& A, const Mat2D& B, Mat2D& out )
		{
			// Row 0
			out.m00 = A.m00 * B.m00 + A.m01 * B.m10;
			out.m01 = A.m00 * B.m01 + A.m01 * B.m11;
			out.m02 = A.m00 * B.m02 + A.m01 * B.m12 + A.m02;

			// Row 1
			out.m10 = A.m10 * B.m00 + A.m11 * B.m10;
			out.m11 = A.m10 * B.m01 + A.m11 * B.m11;
			out.m12 = A.m10 * B.m02 + A.m11 * B.m12 + A.m12;
		}

		/// @brief Multiply: out = A * B  (2x3 * 2x3 matrix)
		/// @param A 
		/// @param B 
		/// @return result 
		static inline constexpr Mat2D Multiply( const Mat2D& A, const Mat2D& B )
		{
			Mat2D out;
			// Row 0
			out.m00 = A.m00 * B.m00 + A.m01 * B.m10;
			out.m01 = A.m00 * B.m01 + A.m01 * B.m11;
			out.m02 = A.m00 * B.m02 + A.m01 * B.m12 + A.m02;

			// Row 1
			out.m10 = A.m10 * B.m00 + A.m11 * B.m10;
			out.m11 = A.m10 * B.m01 + A.m11 * B.m11;
			out.m12 = A.m10 * B.m02 + A.m11 * B.m12 + A.m12;

			return out;
		}

		/// @brief Computes the inverse of this matrix (assuming it's invertible).
		///  If it's not invertible (det = 0), we do a simple check and return an identity.
		inline constexpr Mat2D Inverse() const
		{
			Mat2D inverted = Mat2D::Identity();
			// We'll name them for clarity:
			float a = m00;
			float b = m01;
			float c = m10;
			float d = m11;
			float tx = m02;
			float ty = m12;

			// determinant of the 2x2 linear part
			float det = a * d - b * c;
			if (std::fabs( det ) < 1e-9f) {
				// Not invertible, return identity or handle gracefully
				return Mat2D::Identity();
			}
			float invDet = 1.0f / det;

			// Invert the linear part
			inverted.m00 = d * invDet;    //  d/det
			inverted.m01 = -b * invDet;    // -b/det
			inverted.m10 = -c * invDet;    // -c/det
			inverted.m11 = a * invDet;    //  a/det

			// Now compute the inverse translation.
			// We want to solve:  X' = L^-1 * (X - T)
			// So the new translation is -L^-1 * T
			inverted.m02 = -(inverted.m00 * tx + inverted.m01 * ty);
			inverted.m12 = -(inverted.m10 * tx + inverted.m11 * ty);

			return inverted;
		}


		/// @brief Transforms a point (x, y) using the full 2D affine transform.
		/// The result is stored in outX and outY.
		/// @param x 
		/// @param y 
		/// @param outX 
		/// @param outY 
		inline constexpr void TransformPoint( float x, float y, float& outX, float& outY ) const {
			outX = m00 * x + m01 * y + m02;
			outY = m10 * x + m11 * y + m12;
		}


		/// @param x 
		/// @param y 
		/// @param outX 
		/// @param outY 

		/// @brief Transforms a point (x, y) using the full 2D affine transform.
		/// @param point 
		/// @return The transformed point as a Vector2
		inline constexpr Vector2 TransformPoint( const Vector2& point ) const {
			return Vector2(
				m00 * point.X() + m01 * point.Y() + m02,
				m10 * point.X() + m11 * point.Y() + m12
			);
		}

		/// @brief InverseTransformPoint(x, y, outX, outY)
		/// 	Transforms a point by the INVERSE of this matrix,
		/// 	without modifying or returning a new matrix.			
		/// @param x 
		/// @param y 
		/// @param outX 
		/// @param outY 
		inline constexpr void InverseTransformPoint( float x, float y, float& outX, float& outY ) const
		{
			// Remove the translation
			float X = x - m02;
			float Y = y - m12;

			// Compute the determinant of the 2x2 linear part
			float det = m00 * m11 - m01 * m10;
			if (std::fabs( det ) < 1e-9f)
			{
				// Handle non-invertible case appropriately.
				// Here, we simply return the original point or you could set an error flag.
				outX = x;
				outY = y;
				return;
			}
			float invDet = 1.0f / det;

			// Solve for the original coordinates
			outX = (m11 * X - m01 * Y) * invDet;
			outY = (-m10 * X + m00 * Y) * invDet;
		}


		inline constexpr Mat2D operator*( const Mat2D& other ) const
		{
			return Mat2D::Multiply( *this, other );
		}
	};
}

#endif /*MAT2D_H*/