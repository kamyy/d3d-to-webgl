#include "BasicTypes.h"

namespace eve {
	Matrix4x4::Matrix4x4(const Quaternion& q) {
		const f32 xx = q.m_x * q.m_x;
		const f32 wx = q.m_w * q.m_x;

		const f32 xy = q.m_x * q.m_y;
		const f32 yy = q.m_y * q.m_y;
		const f32 wy = q.m_w * q.m_y;

		const f32 xz = q.m_x * q.m_z;
		const f32 yz = q.m_y * q.m_z;
		const f32 zz = q.m_z * q.m_z;
		const f32 wz = q.m_w * q.m_z;

		m_11 = 1.f - 2.f * (yy + zz);
		m_12 = 2.f * (xy + wz);
		m_13 = 2.f * (xz - wy);
		m_14 = 0.f;

		m_21 = 2.f * (xy - wz);
		m_22 = 1.f - 2.f * (xx + zz);
		m_23 = 2.f * (yz + wx);
		m_24 = 0.f;

		m_31 = 2.f * (xz + wy);
		m_32 = 2.f * (yz - wx);
		m_33 = 1.f - 2.f * (xx + yy);
		m_34 = 0.f;

		m_41 = 0.f;
		m_42 = 0.f;
		m_43 = 0.f;
		m_44 = 1.f;
	}
	Matrix4x4::Matrix4x4(	f32 _11, f32 _12, f32 _13, f32 _14,
							f32 _21, f32 _22, f32 _23, f32 _24,
							f32 _31, f32 _32, f32 _33, f32 _34,
							f32 _41, f32 _42, f32 _43, f32 _44) {
		m_11 = _11; m_12 = _12; m_13 = _13; m_14 = _14;
		m_21 = _21; m_22 = _22; m_23 = _23; m_24 = _24;
		m_31 = _31; m_32 = _32; m_33 = _33; m_34 = _34;
		m_41 = _41; m_42 = _42; m_43 = _43; m_44 = _44;
	}
	Matrix4x4::Matrix4x4(Identity i) {
		setIdentity();
	}
	Matrix4x4::Matrix4x4(Rotation r, f32 theta) {
		setIdentity();
		const f32 cosTheta = cosf(theta);
		const f32 sinTheta = sinf(theta);
		switch (r) {
		case ROTATE_X:
			m_22 = +cosTheta;	
			m_23 = +sinTheta;
			m_32 = -sinTheta;	
			m_33 = +cosTheta;
			break;
		case ROTATE_Y:
			m_11 = +cosTheta;	
			m_13 = -sinTheta;
			m_31 = +sinTheta;	
			m_33 = +cosTheta;
			break;
		case ROTATE_Z:
			m_11 = +cosTheta;	
			m_12 = +sinTheta;
			m_21 = -sinTheta;	
			m_22 = +cosTheta;
		}
	}

	void Matrix4x4::operator=(const Quaternion& q) {
		const f32 xx = q.m_x * q.m_x;
		const f32 wx = q.m_w * q.m_x;

		const f32 xy = q.m_x * q.m_y;
		const f32 yy = q.m_y * q.m_y;
		const f32 wy = q.m_w * q.m_y;

		const f32 xz = q.m_x * q.m_z;
		const f32 yz = q.m_y * q.m_z;
		const f32 zz = q.m_z * q.m_z;
		const f32 wz = q.m_w * q.m_z;

		m_11 = 1.f - 2.f * (yy + zz);
		m_12 = 2.f * (xy + wz);
		m_13 = 2.f * (xz - wy);
		m_14 = 0.f;

		m_21 = 2.f * (xy - wz);
		m_22 = 1.f - 2.f * (xx + zz);
		m_23 = 2.f * (yz + wx);
		m_24 = 0.f;

		m_31 = 2.f * (xz + wy);
		m_32 = 2.f * (yz - wx);
		m_33 = 1.f - 2.f * (xx + yy);
		m_34 = 0.f;

		m_41 = 0.f;
		m_42 = 0.f;
		m_43 = 0.f;
		m_44 = 1.f;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
		return Matrix4x4(	m_11 * rhs.m_11 + m_12 * rhs.m_21 + m_13 * rhs.m_31 + m_14 * rhs.m_41,
							m_11 * rhs.m_12 + m_12 * rhs.m_22 + m_13 * rhs.m_32 + m_14 * rhs.m_42,
							m_11 * rhs.m_13 + m_12 * rhs.m_23 + m_13 * rhs.m_33 + m_14 * rhs.m_43,
							m_11 * rhs.m_14 + m_12 * rhs.m_24 + m_13 * rhs.m_34 + m_14 * rhs.m_44,

							m_21 * rhs.m_11 + m_22 * rhs.m_21 + m_23 * rhs.m_31 + m_24 * rhs.m_41,
							m_21 * rhs.m_12 + m_22 * rhs.m_22 + m_23 * rhs.m_32 + m_24 * rhs.m_42,
							m_21 * rhs.m_13 + m_22 * rhs.m_23 + m_23 * rhs.m_33 + m_24 * rhs.m_43,
							m_21 * rhs.m_14 + m_22 * rhs.m_24 + m_23 * rhs.m_34 + m_24 * rhs.m_44,

							m_31 * rhs.m_11 + m_32 * rhs.m_21 + m_33 * rhs.m_31 + m_34 * rhs.m_41,
							m_31 * rhs.m_12 + m_32 * rhs.m_22 + m_33 * rhs.m_32 + m_34 * rhs.m_42,
							m_31 * rhs.m_13 + m_32 * rhs.m_23 + m_33 * rhs.m_33 + m_34 * rhs.m_43,
							m_31 * rhs.m_14 + m_32 * rhs.m_24 + m_33 * rhs.m_34 + m_34 * rhs.m_44,

							m_41 * rhs.m_11 + m_42 * rhs.m_21 + m_43 * rhs.m_31 + m_44 * rhs.m_41,
							m_41 * rhs.m_12 + m_42 * rhs.m_22 + m_43 * rhs.m_32 + m_44 * rhs.m_42,
							m_41 * rhs.m_13 + m_42 * rhs.m_23 + m_43 * rhs.m_33 + m_44 * rhs.m_43,
							m_41 * rhs.m_14 + m_42 * rhs.m_24 + m_43 * rhs.m_34 + m_44 * rhs.m_44);
	}
	void Matrix4x4::operator*=(const Matrix4x4& rhs) {
		const f32 _11 = m_11; const f32 _12 = m_12; const f32 _13 = m_13; const f32 _14 = m_14;
		const f32 _21 = m_21; const f32 _22 = m_22; const f32 _23 = m_23; const f32 _24 = m_24;
		const f32 _31 = m_31; const f32 _32 = m_32; const f32 _33 = m_33; const f32 _34 = m_34;
		const f32 _41 = m_41; const f32 _42 = m_42; const f32 _43 = m_43; const f32 _44 = m_44;

		m_11 = _11 * rhs.m_11 + _12 * rhs.m_21 + _13 * rhs.m_31 + _14 * rhs.m_41;
		m_12 = _11 * rhs.m_12 + _12 * rhs.m_22 + _13 * rhs.m_32 + _14 * rhs.m_42;
		m_13 = _11 * rhs.m_13 + _12 * rhs.m_23 + _13 * rhs.m_33 + _14 * rhs.m_43;
		m_14 = _11 * rhs.m_14 + _12 * rhs.m_24 + _13 * rhs.m_34 + _14 * rhs.m_44;

		m_21 = _21 * rhs.m_11 + _22 * rhs.m_21 + _23 * rhs.m_31 + _24 * rhs.m_41;
		m_22 = _21 * rhs.m_12 + _22 * rhs.m_22 + _23 * rhs.m_32 + _24 * rhs.m_42;
		m_23 = _21 * rhs.m_13 + _22 * rhs.m_23 + _23 * rhs.m_33 + _24 * rhs.m_43;
		m_24 = _21 * rhs.m_14 + _22 * rhs.m_24 + _23 * rhs.m_34 + _24 * rhs.m_44;

		m_31 = _31 * rhs.m_11 + _32 * rhs.m_21 + _33 * rhs.m_31 + _34 * rhs.m_41;
		m_32 = _31 * rhs.m_12 + _32 * rhs.m_22 + _33 * rhs.m_32 + _34 * rhs.m_42;
		m_33 = _31 * rhs.m_13 + _32 * rhs.m_23 + _33 * rhs.m_33 + _34 * rhs.m_43;
		m_34 = _31 * rhs.m_14 + _32 * rhs.m_24 + _33 * rhs.m_34 + _34 * rhs.m_44;

		m_41 = _41 * rhs.m_11 + _42 * rhs.m_21 + _43 * rhs.m_31 + _44 * rhs.m_41;
		m_42 = _41 * rhs.m_12 + _42 * rhs.m_22 + _43 * rhs.m_32 + _44 * rhs.m_42;
		m_43 = _41 * rhs.m_13 + _42 * rhs.m_23 + _43 * rhs.m_33 + _44 * rhs.m_43;
		m_44 = _41 * rhs.m_14 + _42 * rhs.m_24 + _43 * rhs.m_34 + _44 * rhs.m_44;
	}

	Matrix4x4 Matrix4x4::operator!() const {
		return Matrix4x4(	m_11, m_21, m_31, 0.0f,
							m_12, m_22, m_32, 0.0f,
							m_13, m_23, m_33, 0.0f,
							-(m_41 * m_11 + m_42 * m_12 + m_43 * m_13),
							-(m_41 * m_21 + m_42 * m_22 + m_43 * m_23),
							-(m_41 * m_31 + m_42 * m_32 + m_43 * m_33), 1.0f);
	}
	Matrix4x4 Matrix4x4::operator~() const {
		return Matrix4x4(	m_11, m_21, m_31, m_41,
							m_12, m_22, m_32, m_42,
							m_13, m_23, m_33, m_43,
							m_14, m_24, m_34, m_44);
	}

	void Matrix4x4::setTranspose() {
		f32 t;
		t = m_12; m_12 = m_21; m_21 = t;
		t = m_13; m_13 = m_31; m_31 = t;
		t = m_14; m_14 = m_41; m_41 = t;
		t = m_23; m_23 = m_32; m_32 = t;
		t = m_24; m_24 = m_42; m_42 = t;
		t = m_34; m_34 = m_43; m_43 = t;
	}
	void Matrix4x4::setIdentity() {
		memset((f32*)this, 0, sizeof(*this));
		m_11 = m_22 = m_33 = m_44 = 1.0f;
	}
	void Matrix4x4::setInverse() {
		const f32 _11 = m_11; const f32 _12 = m_12; const f32 _13 = m_13; const f32 _14 = m_14;
		const f32 _21 = m_21; const f32 _22 = m_22; const f32 _23 = m_23; const f32 _24 = m_24;
		const f32 _31 = m_31; const f32 _32 = m_32; const f32 _33 = m_33; const f32 _34 = m_34;
		const f32 _41 = m_41; const f32 _42 = m_42; const f32 _43 = m_43; const f32 _44 = m_44;

		m_12 = _21; m_13 = _31; m_14 = 0.0f;
		m_21 = _12; m_23 = _32; m_24 = 0.0f;
		m_31 = _13; m_32 = _23; m_34 = 0.0f;
		m_41 = -(_41 * _11 + _42 * _12 + _43 * _13);
		m_42 = -(_41 * _21 + _42 * _22 + _43 * _23);
		m_43 = -(_41 * _31 + _42 * _32 + _43 * _33);
		m_44 = 1.f;
	}

	void Matrix4x4::setTxyz(f32 tx, f32 ty, f32 tz) {
		setIdentity();
		m_41 = tx;
		m_42 = ty;
		m_43 = tz;
	}
	void Matrix4x4::setSxyz(f32 sx, f32 sy, f32 sz) {
		setIdentity();
		m_11 = sx;
		m_22 = sy;
		m_33 = sz;
	}
	void Matrix4x4::setRx(f32 theta) {	
		const f32 cosTheta = cosf(theta);
		const f32 sinTheta = sinf(theta);
		m_11 = 1.0f;		m_12 = 0.0f;		m_13 = 0.0f;		m_14 = 0.0f;
		m_21 = 0.0f;		m_22 = cosTheta;	m_23 = sinTheta;	m_24 = 0.0f;
		m_31 = 0.0f;		m_32 = -sinTheta;	m_33 = cosTheta;	m_34 = 0.0f;
		m_41 = 0.0f;		m_42 = 0.0f;		m_43 = 0.0f;		m_44 = 1.0f;
	}
	void Matrix4x4::setRy(f32 theta) {	
		const f32 cosTheta = cosf(theta);
		const f32 sinTheta = sinf(theta);
		m_11 = cosTheta;	m_12 = 0.0f;		m_13 = -sinTheta;	m_14 = 0.0f;
		m_21 = 0.0f;		m_22 = 1.0f;		m_23 = 0.0f;		m_24 = 0.0f;
		m_31 = sinTheta;	m_32 = 0.0f;		m_33 = cosTheta;	m_34 = 0.0f;
		m_41 = 0.0f;		m_42 = 0.0f;		m_43 = 0.0f;		m_44 = 1.0f;
	}
	void Matrix4x4::setRz(f32 theta) {
		const f32 cosTheta = cosf(theta);
		const f32 sinTheta = sinf(theta);
		m_11 = cosTheta;	m_12 = sinTheta;	m_13 = 0.0f;		m_14 = 0.0f;
		m_21 = -sinTheta;	m_22 = cosTheta;	m_23 = 0.0f;		m_24 = 0.0f;
		m_31 = 0.0f;		m_32 = 0.0f;		m_33 = 1.0f;		m_34 = 0.0f;
		m_41 = 0.0f;		m_42 = 0.0f;		m_43 = 0.0f;		m_44 = 1.0f;
	}

	void Matrix4x4::postCatTxyz(f32 tx, f32 ty, f32 tz) {
		m_41 += tx;
		m_42 += ty;
		m_43 += tz;
	}
	void Matrix4x4::postCatSxyz(f32 sx, f32 sy, f32 sz) {
		m_11 *= sx; m_12 *= sy; m_13 *= sz;
		m_21 *= sx; m_22 *= sy; m_23 *= sz;
		m_31 *= sx; m_32 *= sy; m_33 *= sz;
		m_41 *= sx; m_42 *= sy; m_43 *= sz;
	}
}