#include "BasicTypes.h"
#include "Camera.h"

namespace eve {
	Camera::Camera(	f32 fieldOfView,
					f32 aspectRatio,
					f32 clipDistanceN,
					f32 clipDistanceF,
					RefFrame* parent) : RefFrame(parent) {
		m_type = CAMERA;
		m_fieldOfView = fieldOfView;
		m_aspectRatio = aspectRatio;
		m_clipDistanceN = clipDistanceN;
		m_clipDistanceF = clipDistanceF;
		m_invalidView = true;
		m_invalidProj = true;
	}

	void Camera::invalidateSubClass() {
		m_invalidView = true;
	}

	const Matrix4x4& Camera::getViewMatrix() {
		// right-handed camera system where +ve y-axis points in view direction
		if (m_invalidView) {
			m_invalidView = false;
			m_view = !getWorldMatrix();
		}
		return m_view;
	}
	const Matrix4x4& Camera::getProjMatrix() {
		if (m_invalidProj) {
			m_invalidProj = false;

			// standard D3D left-handed projection matrix
			D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)&m_proj, 
										m_fieldOfView, 
										m_aspectRatio, 
										m_clipDistanceN, 
										m_clipDistanceF);

			// flip y and z vertex coordinates to convert from RHS to LHS
			// for D3D by switching around rows 2 and 3
			const f32 x = m_proj.m_21;
			const f32 y = m_proj.m_22;
			const f32 z = m_proj.m_23;
			const f32 w = m_proj.m_24;
			m_proj.m_21 = m_proj.m_31;
			m_proj.m_22 = m_proj.m_32;
			m_proj.m_23 = m_proj.m_33;
			m_proj.m_24 = m_proj.m_34;
			m_proj.m_31 = x;
			m_proj.m_32 = y;
			m_proj.m_33 = z;
			m_proj.m_34 = w;
		}
		return m_proj;
	}
	const Matrix4x4& Camera::getViewProjMatrix() {
		if (m_invalidView || m_invalidProj) {
			m_viewProj = getViewMatrix() * getProjMatrix();
		}
		return m_viewProj;
	}
}
