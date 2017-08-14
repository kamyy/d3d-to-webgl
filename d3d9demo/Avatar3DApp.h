/* 
 * Avatar3D by Kam Yin Yip
 * yy_kam@hotmail.com
 * http://www.geocities.com/tapdanceshoes
 */
#pragma once
#ifndef Avatar3DApp_h
#define Avatar3DApp_h

#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Actor.h"
#include "Timer.h"
#include "ASE.h"

namespace eve {
	class Avatar3DApp : public CWinApp {
	public:
		Avatar3DApp();
	~	Avatar3DApp();
		BOOL InitInstance();
		BOOL OnIdle(LONG n);

		void initScene(const ASE& ase);
		void freeScene();

		bool isFullscreen() const {
			return !m_pp.Windowed;
		}
		void toggleFullscreen();

		void toggleDrawWirefrm() {
			m_drawWirefrm = !m_drawWirefrm;
		}
		void toggleDrawNormals() {
			m_drawNormals = !m_drawNormals;
		}

		bool isDrawWirefrmOn() const {
			return m_drawWirefrm;
		}
		bool isDrawNormalsOn() const {
			return m_drawNormals;
		}

		u32 getCameraCount() const {
			return (u32)m_cameraArray.size();
		}
		u32 getCamTgtCount() const {
			return (u32)m_camTgtArray.size();
		}

		bool setActiveCamera(u32 i);
		Camera* getActiveCamera() const;
		Camera* getCamera(u32 i) const;

		bool setActiveCamTgt(u32 i);
		RefFrame* getActiveCamTgt() const;
		RefFrame* getCamTgt(u32 i) const;

		void tick();
	protected:
		bool testDevice();
		void renderScene();
		void resetD3DResources();

		RECT		m_rect;
		HACCEL		m_haccel;
		Timer		m_timer;

		D3DPRESENT_PARAMETERS	m_pp;
		IDirect3D9*				m_d3d;
		IDirect3DDevice9*		m_dev;

		D3DResourceUserMgr*		m_rsrcUserMgr;
		MaterialMgr*			m_materialMgr;
		TextureMgr*				m_textureMgr;
		ShaderMgr*				m_shaderMgr;
		VtxMgr*					m_vertexMgr;

		RefFrame*	m_rootFrame;
		CameraArray	m_cameraArray;
		CamTgtArray	m_camTgtArray;
		RefFrame*	m_activeCamTgt;
		Camera*		m_activeCamera;
		Camera*		m_mirrorCamera;

		ActorList*	m_actors;
		Actor*		m_mirror;
		OmniLS*		m_omniLS;
		AmbientLS*	m_ambientLS;

		bool	m_drawWirefrm;
		bool	m_drawNormals;
	};
}

#endif

