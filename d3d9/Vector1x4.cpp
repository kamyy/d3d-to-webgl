#include "BasicTypes.h"

namespace eve {
	Vector1x4::Vector1x4(f32 x, f32 y, f32 z, f32 w) {
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	Vector1x4 Vector1x4::operator+(const Vector1x4& rhs) const {
		return Vector1x4(	m_x + rhs.m_x, 
							m_y + rhs.m_y, 
							m_z + rhs.m_z, 
							m_w + rhs.m_w);
	}
	Vector1x4 Vector1x4::operator-(const Vector1x4& rhs) const {
		return Vector1x4(	m_x - rhs.m_x, 
							m_y - rhs.m_y, 
							m_z - rhs.m_z, 
							0.f);
	}
	void Vector1x4::operator+=(const Vector1x4& rhs) {
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		m_w += rhs.m_w;
	}
	void Vector1x4::operator-=(const Vector1x4& rhs) {
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		m_w = 0.f;
	}
	Vector1x4 Vector1x4::operator-() const {
		return Vector1x4(-m_x, -m_y, -m_z, 1.f);
	}

	f32 Vector1x4::operator*(const Vector1x4& rhs) const { 
		return (m_x * rhs.m_x) + (m_y * rhs.m_y) + (m_z * rhs.m_z);
	}
	Vector1x4 Vector1x4::operator^(const Vector1x4& rhs) const {
		return Vector1x4(	m_y * rhs.m_z - m_z * rhs.m_y,
							m_z * rhs.m_x - m_x * rhs.m_z,
							m_x * rhs.m_y - m_y * rhs.m_x,
							0.f);
	}
	Vector1x4 Vector1x4::operator*(const Matrix4x4& rhs) const {
		return Vector1x4(	m_x * rhs.m_11 + m_y * rhs.m_21 + m_z * rhs.m_31 + m_w * rhs.m_41,
							m_x * rhs.m_12 + m_y * rhs.m_22 + m_z * rhs.m_32 + m_w * rhs.m_42,
							m_x * rhs.m_13 + m_y * rhs.m_23 + m_z * rhs.m_33 + m_w * rhs.m_43,
							m_x * rhs.m_14 + m_y * rhs.m_24 + m_z * rhs.m_34 + m_w * rhs.m_44);
	}
	void Vector1x4::operator*=(const Matrix4x4& rhs) {
		const f32 x = m_x * rhs.m_11 + m_y * rhs.m_21 + m_z * rhs.m_31 + m_w * rhs.m_41;
		const f32 y = m_x * rhs.m_12 + m_y * rhs.m_22 + m_z * rhs.m_32 + m_w * rhs.m_42;
		const f32 z = m_x * rhs.m_13 + m_y * rhs.m_23 + m_z * rhs.m_33 + m_w * rhs.m_43;
		const f32 w = m_x * rhs.m_14 + m_y * rhs.m_24 + m_z * rhs.m_34 + m_w * rhs.m_44;
		m_x = x;
		m_y = y;
		m_z = z;
		m_w = w;
	}

	Vector1x4 Vector1x4::operator*(f32 k) const {
		return Vector1x4(m_x * k, m_y * k, m_z * k, m_w);
	}
	Vector1x4 Vector1x4::operator/(f32 k) const {
		return Vector1x4(m_x / k, m_y / k, m_z / k, m_w);
	}
	void Vector1x4::operator*=(f32 k) {
		m_x *= k;
		m_y *= k;
		m_z *= k;
	}
	void Vector1x4::operator/=(f32 k) {
		m_x /= k;
		m_y /= k;
		m_z /= k;
	}

	f32 Vector1x4::magnitude() const {
		return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
	}
	f32 Vector1x4::normalize() {
		f32 l = magnitude();
		m_x /= l;
		m_y /= l;
		m_z /= l;
		return l;
	}
}