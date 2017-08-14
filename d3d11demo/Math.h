#pragma once
#ifndef Matrix4x4_h
#define Matrix4x4_h

namespace d3d11demo {

    class Matrix4x4;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    inline float DegressToRadians(float degrees) {
        return (degrees * 3.141592653589793f / 180.0f);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Vector1x4 {
    public:
        float m_x;
        float m_y;
        float m_z;
        float m_w;

        Vector1x4() {}
        Vector1x4(float x, float y, float z, float w);

        Vector1x4 operator+(const Vector1x4& rhs) const;
        Vector1x4 operator-(const Vector1x4& rhs) const;
        void operator+=(const Vector1x4& rhs);
        void operator-=(const Vector1x4& rhs);
        Vector1x4 operator-() const;

        float operator*(const Vector1x4& rhs) const;
        Vector1x4 operator^(const Vector1x4& rhs) const;
        Vector1x4 operator*(const Matrix4x4& rhs) const;
        void operator*=(const Matrix4x4& rhs);

        friend Vector1x4 operator*(float k, const Vector1x4& v) { return v * k; }
        friend Vector1x4 operator/(float k, const Vector1x4& v) { return v / k; }
        Vector1x4 operator*(float k) const;
        Vector1x4 operator/(float k) const;
        void operator*=(float k);
        void operator/=(float k);

        operator const float*() const {
            return &m_x;
        }
        operator float*() {
            return &m_x;
        }

        float magnitude() const;
        float normalize();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Quaternion {
    public:
        float m_x;
        float m_y;
        float m_z;
        float m_w;

        enum Identity {
            IDENTITY
        };
        Quaternion() {}
        Quaternion(Identity i);
        Quaternion(const Vector1x4& position);
        Quaternion(const Vector1x4& unitAxis, float theta);
        Quaternion(float x, float y, float z, float w);

        Quaternion slerp(const Quaternion& q, float t) const;
        Quaternion operator*(const Quaternion& rhs) const;
        Quaternion operator-() const;
        Quaternion operator!() const;
        float operator%(const Quaternion& q) const;

        float norm() const; // xx + yy + zz + ww
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Matrix4x4 {
    public:
        float m_11, m_12, m_13, m_14;
        float m_21, m_22, m_23, m_24;
        float m_31, m_32, m_33, m_34;
        float m_41, m_42, m_43, m_44;

        enum Identity {
            IDENTITY
        };
        enum Rotation {
            ROTATE_X,
            ROTATE_Y,
            ROTATE_Z
        };

        Matrix4x4() {}
        Matrix4x4(const Quaternion& q);
        Matrix4x4(  float _11, float _12, float _13, float _14,
                    float _21, float _22, float _23, float _24,
                    float _31, float _32, float _33, float _34,
                    float _41, float _42, float _43, float _44);
        Matrix4x4(Identity i);
        Matrix4x4(Rotation r, float theta);

        void operator=(const Quaternion& q);
        Matrix4x4 operator*(const Matrix4x4& rhs) const;
        void operator*=(const Matrix4x4& rhs);
        Matrix4x4 operator!() const;
        Matrix4x4 operator~() const;

        void setTranspose();
        void setIdentity();
        void setInverse();

        void setTxyz(float tx, float ty, float tz);
        void setSxyz(float sx, float sy, float sz);
        void setRx(float theta);
        void setRy(float theta);
        void setRz(float theta);

        void postCatTxyz(float tx, float ty, float tz);
        void postCatSxyz(float sx, float sy, float sz);
        void postCatRx(float theta);
        void postCatRy(float theta);
        void postCatRz(float theta);

        void preCatTxyz(float tx, float ty, float tz);
        void preCatSxyz(float sx, float sy, float sz);
        void preCatRx(float theta);
        void preCatRy(float theta);
        void preCatRz(float theta);

        operator const float*() const {
            return &m_11;
        }
        operator float*() {
            return &m_11;
        }
    };

}

#endif
