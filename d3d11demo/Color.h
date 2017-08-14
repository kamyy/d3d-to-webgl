#pragma once
#ifndef Color_h
#define Color_h

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct Color {
        float m_r;
        float m_g;
        float m_b;
        float m_a;

        Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
            m_r = r;
            m_g = g;
            m_b = b;
            m_a = a;
        }

        Color operator*(const Color& rhs) const {
            return Color(   m_r * rhs.m_r,
                            m_g * rhs.m_g,
                            m_b * rhs.m_b,
                            m_a * rhs.m_a);
        }

        Color operator+(const Color& rhs) const {
            return Color(   m_r + rhs.m_r,
                            m_g + rhs.m_g,
                            m_b + rhs.m_b,
                            m_a + rhs.m_a);
        }

        Color operator-(const Color& rhs) const {
            return Color(   m_r - rhs.m_r,
                            m_g - rhs.m_g,
                            m_b - rhs.m_b,
                            m_a - rhs.m_a);
        }

        Color operator*(float k) const {
            return Color(m_r * k, m_g * k, m_b * k, m_a);
        }

        Color operator/(float k) const {
            return Color(m_r / k, m_g / k, m_b / k, m_a);
        }

        operator const float*() const {
            return &m_r;
        }

        operator float*() {
            return &m_r;
        }
    };

}

#endif
