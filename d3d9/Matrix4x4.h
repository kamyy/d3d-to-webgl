#pragma once
#ifndef Matrix4x4_h
#define Matrix4x4_h

namespace eve {
	class Quaternion;

	class Matrix4x4 {
	public:
		f32 m_11, m_12, m_13, m_14;
		f32 m_21, m_22, m_23, m_24;
		f32 m_31, m_32, m_33, m_34;
		f32 m_41, m_42, m_43, m_44;

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
		Matrix4x4(	f32 _11, f32 _12, f32 _13, f32 _14,
					f32 _21, f32 _22, f32 _23, f32 _24,
					f32 _31, f32 _32, f32 _33, f32 _34,
					f32 _41, f32 _42, f32 _43, f32 _44);
		Matrix4x4(Identity i);
		Matrix4x4(Rotation r, f32 theta);

		void operator=(const Quaternion& q);
		Matrix4x4 operator*(const Matrix4x4& rhs) const;
		void operator*=(const Matrix4x4& rhs);
		Matrix4x4 operator!() const;
		Matrix4x4 operator~() const;

		void setTranspose();
		void setIdentity();
		void setInverse();

		void setTxyz(f32 tx, f32 ty, f32 tz);
		void setSxyz(f32 sx, f32 sy, f32 sz);
		void setRx(f32 theta);
		void setRy(f32 theta);
		void setRz(f32 theta);

		void postCatTxyz(f32 tx, f32 ty, f32 tz);
		void postCatSxyz(f32 sx, f32 sy, f32 sz);
		void postCatRx(f32 theta);
		void postCatRy(f32 theta);
		void postCatRz(f32 theta);

		void preCatTxyz(f32 tx, f32 ty, f32 tz);
		void preCatSxyz(f32 sx, f32 sy, f32 sz);
		void preCatRx(f32 theta);
		void preCatRy(f32 theta);
		void preCatRz(f32 theta);

		operator const f32*() const { 
			return &m_11; 
		}
		operator f32*() { 
			return &m_11; 
		}
	};
}

#endif
