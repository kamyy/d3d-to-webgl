#include "BasicTypes.h"

namespace eve {
	Color::Color(f32 r, f32 g, f32 b, f32 a) {
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
	}

	Color Color::operator*(const Color& rhs) const {
		return Color(	m_r * rhs.m_r,
						m_g * rhs.m_g,
						m_b * rhs.m_b,
						m_a * rhs.m_a);
	}
	Color Color::operator+(const Color& rhs) const {
		return Color(	m_r + rhs.m_r,
						m_g + rhs.m_g,
						m_b + rhs.m_b,
						m_a + rhs.m_a);	
	}
	Color Color::operator-(const Color& rhs) const {
		return Color(	m_r - rhs.m_r,
						m_g - rhs.m_g,
						m_b - rhs.m_b,
						m_a - rhs.m_a);
	}
	Color Color::operator*(f32 k) const {
		return Color(m_r * k, m_g * k, m_b * k, m_a);
	}
	Color Color::operator/(f32 k) const {
		return Color(m_r / k, m_g / k, m_b / k, m_a);
	}
}