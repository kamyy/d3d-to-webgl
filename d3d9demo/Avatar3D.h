/* 
 * Avatar3D by Kam Yin Yip
 * yy_kam@hotmail.com
 * http://www.geocities.com/tapdanceshoes
 */
#pragma once
#ifndef Avatar3D_h
#define Avatar3D_h

#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Actor.h"
#include "Timer.h"
#include "ASE.h"

namespace eve {
	class Avatar3D {
	public:
		LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		HRESULT initialize(i32 w, i32 h);
		void cleanup();
		void msgLoop();
		void draw();

		Avatar3D() {
			memset(this, 0, sizeof(Avatar3D));
			m_timer = new Timer;
		}
	~	Avatar3D() {
			delete m_timer;
		}
	private:
		i32						m_wd;
		i32						m_ht;
		HWND					m_hwnd;
		RECT					m_rect;
		HACCEL					m_haccel;
		Timer*					m_timer;

		D3DPRESENT_PARAMETERS	m_pp;
		IDirect3D9*				m_d3d9;
		IDirect3DDevice9*		m_device;

		D3DResourceUserMgr*		m_rsrcUserMgr;
		MaterialMgr*			m_materialMgr;
		TextureMgr*				m_textureMgr;
		ShaderMgr*				m_shaderMgr;
		VtxMgr*					m_vertexMgr;

		RefFrame*				m_rootFrame;
		CameraArray*			m_cameras;
		CamTgtArray*			m_camTgts;

		ActorList*				m_actors;
		Actor*					m_mirror;
		RefFrame*				m_target;
		Camera*					m_camera;
		Camera*					m_mirrorCamera;
		OmniLS*					m_omniLS;
		AmbientLS*				m_ambientLS;

		bool					m_drawWireFrm;
		bool					m_drawNormals;

		i32						m_leftX;
		i32						m_leftY;
		bool					m_leftButtonDown;

		i32						m_rightX;
		i32						m_rightY;
		bool					m_rightButtonDown;

		i32						m_middleX;
		i32						m_middleY;
		bool					m_middleButtonDown;

		void initWindow(u32 w, u32 h);
		bool testDevice();
		void initScene(const ASE&);
		void freeScene();
		void resetD3DResources();
	};
}

#endif

