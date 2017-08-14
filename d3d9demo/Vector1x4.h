#pragma once
#ifndef Vector1x4_h
#define Vector1x4_h

namespace eve {
	class Matrix4x4;

	class Vector1x4 {
	public:
		f32 m_x;
		f32 m_y;
		f32 m_z;
		f32 m_w;

		Vector1x4() {}
		Vector1x4(f32 x, f32 y, f32 z, f32 w);

		Vector1x4 operator+(const Vector1x4& rhs) const;
		Vector1x4 operator-(const Vector1x4& rhs) const;
		void operator+=(const Vector1x4& rhs);
		void operator-=(const Vector1x4& rhs);
		Vector1x4 operator-() const;

		f32 operator*(const Vector1x4& rhs) const;
		Vector1x4 operator^(const Vector1x4& rhs) const;
		Vector1x4 operator*(const Matrix4x4& rhs) const;
		void operator*=(const Matrix4x4& rhs);

		friend Vector1x4 operator*(f32 k, const Vector1x4& v) { return v * k; }
		friend Vector1x4 operator/(f32 k, const Vector1x4& v) { return v / k; }
		Vector1x4 operator*(f32 k) const;
		Vector1x4 operator/(f32 k) const;
		void operator*=(f32 k);
		void operator/=(f32 k);

		operator const f32*() const {
			return &m_x;
		}
		operator f32*() {
			return &m_x;
		}

		f32 magnitude() const;
		f32 normalize();
	};
}

#endif
