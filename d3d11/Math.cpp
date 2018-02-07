#include "StdAfx.h"
#include "Math.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    Vector1x4::Vector1x4(float x, float y, float z, float w) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }

    Vector1x4 Vector1x4::operator+(const Vector1x4& rhs) const {
        return Vector1x4(   m_x + rhs.m_x,
                            m_y + rhs.m_y,
                            m_z + rhs.m_z,
                            m_w + rhs.m_w);
    }

    Vector1x4 Vector1x4::operator-(const Vector1x4& rhs) const {
        return Vector1x4(   m_x - rhs.m_x,
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

    float Vector1x4::operator*(const Vector1x4& rhs) const {
        return (m_x * rhs.m_x) + (m_y * rhs.m_y) + (m_z * rhs.m_z);
    }

    Vector1x4 Vector1x4::operator^(const Vector1x4& rhs) const {
        return Vector1x4(   m_y * rhs.m_z - m_z * rhs.m_y,
                            m_z * rhs.m_x - m_x * rhs.m_z,
                            m_x * rhs.m_y - m_y * rhs.m_x,
                            0.f);
    }

    Vector1x4 Vector1x4::operator*(const Matrix4x4& rhs) const {
        return Vector1x4(   m_x * rhs.m_11 + m_y * rhs.m_21 + m_z * rhs.m_31 + m_w * rhs.m_41,
                            m_x * rhs.m_12 + m_y * rhs.m_22 + m_z * rhs.m_32 + m_w * rhs.m_42,
                            m_x * rhs.m_13 + m_y * rhs.m_23 + m_z * rhs.m_33 + m_w * rhs.m_43,
                            m_x * rhs.m_14 + m_y * rhs.m_24 + m_z * rhs.m_34 + m_w * rhs.m_44);
    }

    void Vector1x4::operator*=(const Matrix4x4& rhs) {
        const float x = m_x * rhs.m_11 + m_y * rhs.m_21 + m_z * rhs.m_31 + m_w * rhs.m_41;
        const float y = m_x * rhs.m_12 + m_y * rhs.m_22 + m_z * rhs.m_32 + m_w * rhs.m_42;
        const float z = m_x * rhs.m_13 + m_y * rhs.m_23 + m_z * rhs.m_33 + m_w * rhs.m_43;
        const float w = m_x * rhs.m_14 + m_y * rhs.m_24 + m_z * rhs.m_34 + m_w * rhs.m_44;
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }

    Vector1x4 Vector1x4::operator*(float k) const {
        return Vector1x4(m_x * k, m_y * k, m_z * k, m_w);
    }

    Vector1x4 Vector1x4::operator/(float k) const {
        return Vector1x4(m_x / k, m_y / k, m_z / k, m_w);
    }

    void Vector1x4::operator*=(float k) {
        m_x *= k;
        m_y *= k;
        m_z *= k;
    }

    void Vector1x4::operator/=(float k) {
        m_x /= k;
        m_y /= k;
        m_z /= k;
    }

    float Vector1x4::magnitude() const {
        return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
    }

    float Vector1x4::normalize() {
        float l = magnitude();
        m_x /= l;
        m_y /= l;
        m_z /= l;
        return l;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
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

    Quaternion::Quaternion(const Vector1x4& u, float theta) {
        const float halfTheta = theta * 0.5f;
        const float sinHalfTheta = sinf(halfTheta);
        const float cosHalfTheta = cosf(halfTheta);

        m_x = u.m_x * sinHalfTheta;
        m_y = u.m_y * sinHalfTheta;
        m_z = u.m_z * sinHalfTheta;
        m_w = cosHalfTheta;
    }

    Quaternion::Quaternion(float x, float y, float z, float w) {
        m_x = x; m_y = y; m_z = z; m_w = w;
    }

    Quaternion Quaternion::slerp(const Quaternion& q, float t) const {
        float cosPQ = m_x * q.m_x + m_y * q.m_y + m_z * q.m_z + m_w * q.m_w;
        float sinPQ;
        float theta;
        float scale;
        float s1;
        float s2;

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
        return Quaternion(  s1 * m_x + s2 * q.m_x,
                            s1 * m_y + s2 * q.m_y,
                            s1 * m_z + s2 * q.m_z,
                            s1 * m_w + s2 * q.m_w);
    }

    Quaternion Quaternion::operator*(const Quaternion& rhs) const {
        return Quaternion(  (m_y * rhs.m_z) - (m_z * rhs.m_y) + (m_w * rhs.m_x) + (rhs.m_w * m_x),
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

    float Quaternion::operator%(const Quaternion& q) const {
        return m_x * q.m_x + m_y * q.m_y + m_z * q.m_z + m_w * q.m_w;
    }

    float Quaternion::norm() const {
        return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    Matrix4x4::Matrix4x4(const Quaternion& q) {
        const float xx = q.m_x * q.m_x;
        const float wx = q.m_w * q.m_x;

        const float xy = q.m_x * q.m_y;
        const float yy = q.m_y * q.m_y;
        const float wy = q.m_w * q.m_y;

        const float xz = q.m_x * q.m_z;
        const float yz = q.m_y * q.m_z;
        const float zz = q.m_z * q.m_z;
        const float wz = q.m_w * q.m_z;

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

    Matrix4x4::Matrix4x4(   float _11, float _12, float _13, float _14,
                            float _21, float _22, float _23, float _24,
                            float _31, float _32, float _33, float _34,
                            float _41, float _42, float _43, float _44) {
        m_11 = _11; m_12 = _12; m_13 = _13; m_14 = _14;
        m_21 = _21; m_22 = _22; m_23 = _23; m_24 = _24;
        m_31 = _31; m_32 = _32; m_33 = _33; m_34 = _34;
        m_41 = _41; m_42 = _42; m_43 = _43; m_44 = _44;
    }

    Matrix4x4::Matrix4x4(Identity i) {
        setIdentity();
    }

    Matrix4x4::Matrix4x4(Rotation r, float theta) {
        setIdentity();
        const float cosTheta = cosf(theta);
        const float sinTheta = sinf(theta);
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
        const float xx = q.m_x * q.m_x;
        const float wx = q.m_w * q.m_x;

        const float xy = q.m_x * q.m_y;
        const float yy = q.m_y * q.m_y;
        const float wy = q.m_w * q.m_y;

        const float xz = q.m_x * q.m_z;
        const float yz = q.m_y * q.m_z;
        const float zz = q.m_z * q.m_z;
        const float wz = q.m_w * q.m_z;

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
        return Matrix4x4(   m_11 * rhs.m_11 + m_12 * rhs.m_21 + m_13 * rhs.m_31 + m_14 * rhs.m_41,
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
        const float _11 = m_11; const float _12 = m_12; const float _13 = m_13; const float _14 = m_14;
        const float _21 = m_21; const float _22 = m_22; const float _23 = m_23; const float _24 = m_24;
        const float _31 = m_31; const float _32 = m_32; const float _33 = m_33; const float _34 = m_34;
        const float _41 = m_41; const float _42 = m_42; const float _43 = m_43; const float _44 = m_44;

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
        return Matrix4x4(   m_11, m_21, m_31, 0.0f,
                            m_12, m_22, m_32, 0.0f,
                            m_13, m_23, m_33, 0.0f,
                            -(m_41 * m_11 + m_42 * m_12 + m_43 * m_13),
                            -(m_41 * m_21 + m_42 * m_22 + m_43 * m_23),
                            -(m_41 * m_31 + m_42 * m_32 + m_43 * m_33), 1.0f);
    }

    Matrix4x4 Matrix4x4::operator~() const {
        return Matrix4x4(   m_11, m_21, m_31, m_41,
                            m_12, m_22, m_32, m_42,
                            m_13, m_23, m_33, m_43,
                            m_14, m_24, m_34, m_44);
    }

    void Matrix4x4::setTranspose() {
        float t;
        t = m_12; m_12 = m_21; m_21 = t;
        t = m_13; m_13 = m_31; m_31 = t;
        t = m_14; m_14 = m_41; m_41 = t;
        t = m_23; m_23 = m_32; m_32 = t;
        t = m_24; m_24 = m_42; m_42 = t;
        t = m_34; m_34 = m_43; m_43 = t;
    }

    void Matrix4x4::setIdentity() {
        memset((float*)this, 0, sizeof(*this));
        m_11 = m_22 = m_33 = m_44 = 1.0f;
    }

    void Matrix4x4::setInverse() {
        const float _11 = m_11; const float _12 = m_12; const float _13 = m_13; const float _14 = m_14;
        const float _21 = m_21; const float _22 = m_22; const float _23 = m_23; const float _24 = m_24;
        const float _31 = m_31; const float _32 = m_32; const float _33 = m_33; const float _34 = m_34;
        const float _41 = m_41; const float _42 = m_42; const float _43 = m_43; const float _44 = m_44;

        m_12 = _21; m_13 = _31; m_14 = 0.0f;
        m_21 = _12; m_23 = _32; m_24 = 0.0f;
        m_31 = _13; m_32 = _23; m_34 = 0.0f;
        m_41 = -(_41 * _11 + _42 * _12 + _43 * _13);
        m_42 = -(_41 * _21 + _42 * _22 + _43 * _23);
        m_43 = -(_41 * _31 + _42 * _32 + _43 * _33);
        m_44 = 1.f;
    }

    void Matrix4x4::setTxyz(float tx, float ty, float tz) {
        setIdentity();
        m_41 = tx;
        m_42 = ty;
        m_43 = tz;
    }

    void Matrix4x4::setSxyz(float sx, float sy, float sz) {
        setIdentity();
        m_11 = sx;
        m_22 = sy;
        m_33 = sz;
    }

    void Matrix4x4::setRx(float theta) {
        const float cosTheta = cosf(theta);
        const float sinTheta = sinf(theta);
        m_11 = 1.0f;        m_12 = 0.0f;        m_13 = 0.0f;        m_14 = 0.0f;
        m_21 = 0.0f;        m_22 = cosTheta;    m_23 = sinTheta;    m_24 = 0.0f;
        m_31 = 0.0f;        m_32 = -sinTheta;   m_33 = cosTheta;    m_34 = 0.0f;
        m_41 = 0.0f;        m_42 = 0.0f;        m_43 = 0.0f;        m_44 = 1.0f;
    }

    void Matrix4x4::setRy(float theta) {
        const float cosTheta = cosf(theta);
        const float sinTheta = sinf(theta);
        m_11 = cosTheta;    m_12 = 0.0f;        m_13 = -sinTheta;   m_14 = 0.0f;
        m_21 = 0.0f;        m_22 = 1.0f;        m_23 = 0.0f;        m_24 = 0.0f;
        m_31 = sinTheta;    m_32 = 0.0f;        m_33 = cosTheta;    m_34 = 0.0f;
        m_41 = 0.0f;        m_42 = 0.0f;        m_43 = 0.0f;        m_44 = 1.0f;
    }

    void Matrix4x4::setRz(float theta) {
        const float cosTheta = cosf(theta);
        const float sinTheta = sinf(theta);
        m_11 = cosTheta;    m_12 = sinTheta;    m_13 = 0.0f;        m_14 = 0.0f;
        m_21 = -sinTheta;   m_22 = cosTheta;    m_23 = 0.0f;        m_24 = 0.0f;
        m_31 = 0.0f;        m_32 = 0.0f;        m_33 = 1.0f;        m_34 = 0.0f;
        m_41 = 0.0f;        m_42 = 0.0f;        m_43 = 0.0f;        m_44 = 1.0f;
    }

    void Matrix4x4::postCatTxyz(float tx, float ty, float tz) {
        m_41 += tx;
        m_42 += ty;
        m_43 += tz;
    }

    void Matrix4x4::postCatSxyz(float sx, float sy, float sz) {
        m_11 *= sx; m_12 *= sy; m_13 *= sz;
        m_21 *= sx; m_22 *= sy; m_23 *= sz;
        m_31 *= sx; m_32 *= sy; m_33 *= sz;
        m_41 *= sx; m_42 *= sy; m_43 *= sz;
    }

}
