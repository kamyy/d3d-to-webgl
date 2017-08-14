#pragma once
#ifndef Color_h
#define Color_h

namespace eve {
	struct Color {
		f32 m_r;
		f32 m_g;
		f32 m_b;
		f32 m_a;

		Color(f32 r = 0.0f, f32 g = 0.0f, f32 b = 0.0f, f32 a = 1.0f);
		Color operator*(const Color& rhs) const;
		Color operator+(const Color& rhs) const;
		Color operator-(const Color& rhs) const;
		Color operator*(f32 k) const;
		Color operator/(f32 k) const;
		operator const f32*() const {
			return &m_r;
		}
		operator f32*() {
			return &m_r;
		}
	};
}

#endif