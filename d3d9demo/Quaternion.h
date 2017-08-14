#pragma once
#ifndef Quaternion_h
#define Quaternion_h

namespace eve {
	class Vector1x4;

	class Quaternion {
	public:
		f32 m_x;
		f32 m_y; 
		f32 m_z; 
		f32 m_w;

		enum Identity {
			IDENTITY
		};		
		Quaternion() {}
		Quaternion(Identity i);
		Quaternion(const Vector1x4& position);
		Quaternion(const Vector1x4& unitAxis, f32 theta);
		Quaternion(f32 x, f32 y, f32 z, f32 w);

		Quaternion slerp(const Quaternion& q, f32 t) const;
		Quaternion operator*(const Quaternion& rhs) const;
		Quaternion operator-() const;
		Quaternion operator!() const;
		f32 operator%(const Quaternion& q) const;

		f32 norm() const; // xx + yy + zz + ww
	};
}

#endif
