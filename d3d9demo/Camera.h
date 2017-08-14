#pragma once
#ifndef Camera_h
#define Camera_h

#include "RefFrame.h"

namespace eve {
	const f32 CAMERA_DEFAULT_N_CLIP_DISTANCE = 0.50f;
	const f32 CAMERA_DEFAULT_F_CLIP_DISTANCE = 100.f;
	const f32 CAMERA_DEFAULT_ASPECT_RATIO = 1.333333f;
	const f32 CAMERA_DEFAULT_FOV = D3DXToRadian(50.0f);

	class Camera : public RefFrame {
	public:
		Camera(	f32 fieldOfView,
				f32 aspectRatio,
				f32 clipDistanceN,
				f32 clipDistanceF, RefFrame* parent);

		f32 getFieldOfView() const { 
			return m_fieldOfView; 
		}
		f32 getAspectRatio() const { 
			return m_aspectRatio; 
		}
		f32 getClipDistanceN() const { 
			return m_clipDistanceN; 
		}
		f32 getClipDistanceF() const { 
			return m_clipDistanceF; 
		}

		void setFieldOfView(f32 fieldOfView) { 
			m_fieldOfView = fieldOfView; 
			m_invalidProj = true; 
		}
		void setAspectRatio(f32 aspectRatio) {
			m_aspectRatio = aspectRatio; 
			m_invalidProj = true; 
		}
		void setClipDistanceN(f32 clipDistance) { 
			m_clipDistanceN = clipDistance; 
			m_invalidProj = true; 
		}
		void setClipDistanceF(f32 clipDistance) { 
			m_clipDistanceF = clipDistance; 
			m_invalidProj = true; 
		}

		const Matrix4x4& getViewMatrix();
		const Matrix4x4& getProjMatrix();
		const Matrix4x4& getViewProjMatrix();

	protected:
		f32 m_fieldOfView;
		f32 m_aspectRatio;
		f32 m_clipDistanceN;
		f32 m_clipDistanceF;

		Matrix4x4 m_view;
		Matrix4x4 m_proj;
		bool m_invalidView;
		bool m_invalidProj;
		Matrix4x4 m_viewProj;

		void invalidateSubClass();

	private:
		Camera(const Camera&);
		Camera operator=(const Camera&) const;
	};

	typedef	std::vector<Camera*> CameraArray;
	typedef std::vector<RefFrame*> CamTgtArray;
}

#endif



