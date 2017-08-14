#include "BasicTypes.h"

namespace eve {
	Quaternion::Quaternion(Identity i) {
		m_x = m_y = m_z = 0.f; 
		m_w = 1.f;
	}
	Quaternion::Quaternion(const Vector1x4& p) {
		m_x = p.m_x; 
		m_y = p.m_y; 
		m_z = p.m_z; 
		m_w = 0.f;
	}
	Quaternion::Quaternion(const Vector1x4& u, f32 theta) {
		const f32 halfTheta = theta * 0.5f;
		const f32 sinHalfTheta = sinf(halfTheta);
		const f32 cosHalfTheta = cosf(halfTheta);

		m_x = u.m_x * sinHalfTheta;
		m_y = u.m_y * sinHalfTheta;
		m_z = u.m_z * sinHalfTheta;
		m_w = cosHalfTheta;
	}
	Quaternion::Quaternion(f32 x, f32 y, f32 z, f32 w) {
		m_x = x; m_y = y; m_z = z; m_w = w;
	}

	Quaternion Quaternion::slerp(const Quaternion& q, f32 t) const {
		f32 cosPQ = m_x * q.m_x + m_y * q.m_y + m_z * q.m_z + m_w * q.m_w;
		f32 sinPQ;
		f32 theta;
		f32 scale;
		f32 s1;
		f32 s2;

		// prevent excess spin
		if (cosPQ < 0.0f) { 
			cosPQ = -cosPQ;
			scale = -1.f;
		} else {
			scale = 1.f;
		}

		if (cosPQ < 0.996f) { // spherical linear interpolation
			theta = acosf(cosPQ);
			sinPQ = sinf(theta);	
			s1 = sinf(theta * (1.f - t)) / sinPQ;
			s2 = sinf(theta * t) / sinPQ * scale;
        } else { // linear interpolation
			s1 = 1.f - t;
			s2 = scale * t;
        }
		return Quaternion(	s1 * m_x + s2 * q.m_x,
							s1 * m_y + s2 * q.m_y,
							s1 * m_z + s2 * q.m_z,
							s1 * m_w + s2 * q.m_w);
	}
	Quaternion Quaternion::operator*(const Quaternion& rhs) const {
		return Quaternion(	(m_y * rhs.m_z) - (m_z * rhs.m_y) + (m_w * rhs.m_x) + (rhs.m_w * m_x),
							(m_z * rhs.m_x) - (m_x * rhs.m_z) + (m_w * rhs.m_y) + (rhs.m_w * m_y),
							(m_x * rhs.m_y) - (m_y * rhs.m_x) + (m_w * rhs.m_z) + (rhs.m_w * m_z),
							(m_w * rhs.m_w) - (m_x * rhs.m_x) + (m_y * rhs.m_y) + (m_z * rhs.m_z));	
	}
	Quaternion Quaternion::operator-() const {
		return Quaternion(-m_x, -m_y, -m_z, -m_w);
	}
	Quaternion Quaternion::operator!() const {
		return Quaternion(-m_x, -m_y, -m_z, m_w);
	}
	f32 Quaternion::operator%(const Quaternion& q) const {
		return m_x * q.m_x + m_y * q.m_y + m_z * q.m_z + m_w * q.m_w; 
	}

	f32 Quaternion::norm() const {
		return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
	}
}