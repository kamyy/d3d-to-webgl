/* 
 * Avatar3D by Kam Yin Yip
 * yy_kam@hotmail.com
 * http://www.geocities.com/tapdanceshoes
 */
#include "BasicTypes.h"
#include "Avatar3DApp.h"
#include "Avatar3DWnd.h"
#include "LightsDlg.h"
#include "Resource.h"
#include "ASE.h"

#include <direct.h>

namespace eve {
	Avatar3DApp g_app;

	Avatar3DApp::Avatar3DApp() {
		ZeroMemory(&m_rect, sizeof(m_rect));
		ZeroMemory(&m_pp, sizeof(m_pp));
		m_d3d = 0;
		m_dev = 0;
		m_rsrcUserMgr	= 0;
		m_materialMgr	= 0;
		m_textureMgr	= 0;
		m_shaderMgr		= 0;
		m_vertexMgr		= 0;
		m_rootFrame		= 0;
		m_actors = 0;
		m_mirror = 0;
		m_activeCamTgt = 0;
		m_activeCamera = 0;
		m_mirrorCamera = 0;
		m_omniLS	= 0;
		m_ambientLS	= 0;
		m_drawWirefrm = false;
		m_drawNormals = false;
	}
	Avatar3DApp::~Avatar3DApp() {
		freeScene();
		if (m_vertexMgr)
			m_vertexMgr->rmvRef();
		if (m_shaderMgr)
			m_shaderMgr->rmvRef();
		if (m_textureMgr)
			m_textureMgr->rmvRef();
		if (m_materialMgr)
			m_materialMgr->rmvRef();
		if (m_rsrcUserMgr)
			m_rsrcUserMgr->rmvRef();
		if (m_dev) {
			m_dev->Release();
		}
		if (m_d3d) {
			m_d3d->Release();
		}
	}

	BOOL Avatar3DApp::InitInstance() {
		InitCommonControls();

		Avatar3DWnd* frm = new Avatar3DWnd;
		m_pMainWnd = frm;

		frm->LoadFrame(IDR_AVATAR3D, 0);
		if (frm->m_hWnd && frm->m_childWnd.m_hWnd && (m_d3d = Direct3DCreate9(D3D_SDK_VERSION))) {
			D3DDISPLAYMODE mode;

			EVE_VALIDATE_HR(m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode));
			HRESULT hr1 = m_d3d->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format,
													D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8);
			HRESULT hr2 = m_d3d->CheckDeviceFormat(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format,
													D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1);
			if (FAILED(hr1) && FAILED(hr2)) {
				frm->MessageBox("Neither D3DFMT_D24S8 nor D3DFMT_D15S1 depth stencil formats supported.\n",
								"Cannot create D3D Device", MB_OK | MB_ICONERROR);
				return FALSE;
			}
			ZeroMemory(&m_pp, sizeof(m_pp));
			m_pp.BackBufferWidth			= 0;
			m_pp.BackBufferHeight			= 0;
			m_pp.BackBufferFormat			= D3DFMT_UNKNOWN;
			m_pp.BackBufferCount			= 1;
			m_pp.MultiSampleType			= D3DMULTISAMPLE_NONE;
			m_pp.MultiSampleQuality			= 0;
			m_pp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
			m_pp.hDeviceWindow				= frm->m_childWnd.m_hWnd;
			m_pp.Windowed					= TRUE;
			m_pp.EnableAutoDepthStencil		= TRUE;
			m_pp.AutoDepthStencilFormat		= SUCCEEDED(hr1) ? D3DFMT_D24S8 : D3DFMT_D15S1;
			m_pp.Flags						= 0;
			m_pp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
			m_pp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;

//#ifdef _DEBUG
//			EVE_VALIDATE_HR(m_d3d->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, frm->m_hWnd,
//  												D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_pp, &m_dev));
//#else
			EVE_VALIDATE_HR(m_d3d->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, frm->m_hWnd,
    												D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_pp, &m_dev));
//#endif

			if (testDevice()) {
				m_rsrcUserMgr = new D3DResourceUserMgr(m_dev);
				m_materialMgr = new MaterialMgr;
				m_textureMgr = new TextureMgr;
				m_shaderMgr = new ShaderMgr;
				m_vertexMgr = new VtxMgr;

				ASE ase;
				_chdir("scene/avatar3d");
				if (ase.read("avatar3d.ase")) {
					initScene(ase);
				}
				frm->ShowWindow(SW_SHOW);
				frm->UpdateWindow();
				return TRUE;
			}
		}
		return FALSE;
	}


	BOOL Avatar3DApp::OnIdle(LONG n) {
		if (!CWinApp::OnIdle(n)) {
			tick();
		}
		return TRUE;
	}

	void Avatar3DApp::initScene(const ASE& ase) {
		freeScene();

		m_rootFrame = new RefFrame;

		ase.createCameras(m_cameraArray, m_camTgtArray, m_rootFrame);
		if (m_cameraArray.empty()) { // no cameras in ase so create a default along with something to target
			RefFrame* tgt = new RefFrame(m_rootFrame);
			Camera	* cam = new Camera(	CAMERA_DEFAULT_FOV, 
										CAMERA_DEFAULT_ASPECT_RATIO, 
										CAMERA_DEFAULT_N_CLIP_DISTANCE, 
										CAMERA_DEFAULT_F_CLIP_DISTANCE, tgt);
			m_camTgtArray.push_back(tgt);
			m_cameraArray.push_back(cam);
			cam->translate(Vector1x4(0.0f, -12.0f, 1.0f, 1.0f));
		}
		m_activeCamera = m_cameraArray.at(0);
		m_activeCamTgt = m_camTgtArray.at(0);
		m_actors = new ActorList;

		m_omniLS = new OmniLS(	LightsDlg::getOmniDirectionalRGB(), m_activeCamera,
								LightsDlg::getAttenCoeff0(),
								LightsDlg::getAttenCoeff1(),
								LightsDlg::getAttenCoeff2());

		Shader::setOmniLS(m_omniLS);

		m_ambientLS = new AmbientLS(LightsDlg::getLowerHemisphereRGB(),
									LightsDlg::getUpperHemisphereRGB());
		Shader::setAmbientLS(m_ambientLS);

		ase.createActors(m_actors, m_rootFrame);
		for (ActorList::iterator i = m_actors->begin(); i != m_actors->end(); ++i) {
			if (!strcmp((*i)->getName(), "Avatar3DMirror")) {
				m_mirror = (*i);
				m_actors->erase(i);
				m_mirrorCamera = new Camera(CAMERA_DEFAULT_FOV, 
											CAMERA_DEFAULT_ASPECT_RATIO, 
											CAMERA_DEFAULT_N_CLIP_DISTANCE, 
											CAMERA_DEFAULT_F_CLIP_DISTANCE, m_rootFrame);
				break;
			}
		}
	}
	void Avatar3DApp::freeScene() {
		if (m_rootFrame) {
			m_rootFrame->rmvRef();
			m_rootFrame = 0;
		}
		if (m_actors) {
			delete m_actors;
			m_actors = 0;
		}
		if (m_mirror) {
			m_mirror->rmvRef();
			m_mirror = 0;
		}

		for (CameraArray::iterator i = m_cameraArray.begin(); i != m_cameraArray.end(); ++i) {
			(*i)->rmvRef();
		}
		for (CamTgtArray::iterator j = m_camTgtArray.begin(); j != m_camTgtArray.end(); ++j) {
			(*j)->rmvRef();
		}
		m_cameraArray.clear();
		m_camTgtArray.clear();
		m_activeCamera = 0;
		m_activeCamTgt = 0;

		if (m_mirrorCamera) {
			m_mirrorCamera->rmvRef();
			m_mirrorCamera = 0;
		}

		if (m_omniLS) {
			m_omniLS->rmvRef();
			m_omniLS = 0;
		}
		if (m_ambientLS) {
			m_ambientLS->rmvRef();
			m_ambientLS = 0;
		}
	}

	void Avatar3DApp::toggleFullscreen() {
		if (m_pp.Windowed) {
			m_pMainWnd->GetWindowRect(&m_rect); // save window pos and size before going fullscreen
		}
		m_pp.Windowed = !m_pp.Windowed;
		resetD3DResources();
//		m_timer->reset();
	}

	bool Avatar3DApp::setActiveCamera(u32 i) {
		if (i < m_cameraArray.size()) {
			m_activeCamera = m_cameraArray[i];
			return true;
		}
		return false;
	}
	Camera* Avatar3DApp::getActiveCamera() const {
		return m_activeCamera;
	}
	Camera* Avatar3DApp::getCamera(u32 i) const {
		if (i < m_cameraArray.size())
			return m_cameraArray[i];
		return 0;
	}

	bool Avatar3DApp::setActiveCamTgt(u32 i) {
		if (i < m_camTgtArray.size()) {
			m_activeCamTgt = m_camTgtArray[i];
			return true;
		}
		return false;
	}
	RefFrame* Avatar3DApp::getActiveCamTgt() const {
		return m_activeCamTgt;
	}
	RefFrame* Avatar3DApp::getCamTgt(u32 i) const {
		if (i < m_camTgtArray.size())
			return m_camTgtArray[i];
		return 0;
	}

	void Avatar3DApp::tick() {
		HRESULT hr = m_dev->TestCooperativeLevel();
		switch (hr) {
		case D3D_OK:
			renderScene();
			break;
		case D3DERR_DEVICELOST:
			Sleep(500); // unable to reset device just yet
			break;
		case D3DERR_DEVICENOTRESET:
			resetD3DResources(); // try and reset d3d device and resources
//			m_timer->reset();
			break;
		case D3DERR_DRIVERINTERNALERROR:
			EVE_VALIDATE(!"IDirect3DDevice9::TestCooperativeLevel() == D3DERR_DRIVERINTERNALERROR");
			break;
		}
	}

	bool Avatar3DApp::testDevice() {
		D3DCAPS9 caps;
		m_dev->GetDeviceCaps(&caps);

		if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
			m_pMainWnd->MessageBox(	"Primary Direct3D device does not support pixel shader 2.0 or greater.\n",
									"Error creating D3D Device", MB_OK | MB_ICONERROR);
			return false;
		}
		if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0)) {
			m_pMainWnd->MessageBox(	"Primary Direct3D device does not support vertex shader 2.0 or greater.\n",
									"Error creating D3D Device", MB_OK | MB_ICONERROR);
			return false;
		}
		return true;
	}
	void Avatar3DApp::renderScene() {
		m_dev->Clear(0, NULL,	
						D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
						D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);
		if (SUCCEEDED(m_dev->BeginScene())) {
			if (m_drawWirefrm) {
				m_dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			} else {
				m_dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			if (m_mirror) { // draw into the stencil
				// enable stencil buffer but disable z buffer writes
				m_dev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
				m_dev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				
				// don't allow rendering into the color frame buffer
				m_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				m_dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
				m_dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				// write into the stencil buffer where z-test and stencil test pass
				m_dev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
				// stencil test always passes and value written into stencil is 1
				m_dev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
				m_dev->SetRenderState(D3DRS_STENCILREF, 0x00000001);

				Shader::setCamera(m_activeCamera); // render using non-mirrored camera
				m_mirror->drawAtOnce(); // draw mirror into the stencil

				// cull CCW and allow ordinary rendering into the color buffer
				m_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				m_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				m_dev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				
				// don't modify stencil when rendering where stencil is setup
				m_dev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
				m_dev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

				Matrix4x4 m = m_activeCamera->getWorldMatrix(); // setup mirror camera
				m *= !m_mirror->getRefFrame(0)->getWorldMatrix();
				m.postCatSxyz(1.0f, 1.0f, -1.0f);
				m *= m_mirror->getRefFrame(0)->getWorldMatrix();

				m_mirrorCamera->setWorldMatrix(m);
				m_mirrorCamera->setFieldOfView(m_activeCamera->getFieldOfView());
				m_mirrorCamera->setAspectRatio(m_activeCamera->getAspectRatio());
				m_mirrorCamera->setClipDistanceN(m_activeCamera->getClipDistanceN());
				m_mirrorCamera->setClipDistanceF(m_activeCamera->getClipDistanceF());
				Shader::setCamera(m_mirrorCamera);

				// draw all mirrored actors
				Drawable::setupAlphaQ();
				m_actors->draw();
				Drawable::flushAlphaQ();
			}
			m_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_dev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
			m_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			m_dev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			Shader::setCamera(m_activeCamera);

			if (m_actors && m_drawNormals) {
				m_actors->drawNormals();
			}
			Drawable::setupAlphaQ();
			if (m_mirror) {
				m_mirror->draw();
			}
			if (m_actors) {
				m_actors->draw();
			}
			Drawable::flushAlphaQ();

			m_dev->EndScene();
		}
		m_dev->Present(0, 0, 0, 0);
	}

	void Avatar3DApp::resetD3DResources() {
		if (m_pp.Windowed) { // select windowed mode
			// restore saved non-fullscreen window size and position on screen
			m_pMainWnd->MoveWindow(	m_rect.left, m_rect.top, 
									m_rect.right - m_rect.left, 
									m_rect.bottom - m_rect.top, FALSE);

			m_pp.BackBufferWidth	= 0;
			m_pp.BackBufferHeight	= 0;
			m_pp.BackBufferFormat	= D3DFMT_UNKNOWN;
			m_pp.BackBufferCount	= 1;
			m_pp.hDeviceWindow		= ((Avatar3DWnd*)m_pMainWnd)->m_childWnd.m_hWnd;

		} else { // select fullscreen exclusive mode
			// retrieve desktop display mode settings (screen size and bit depth)
			D3DDISPLAYMODE mode;
			m_dev->GetDisplayMode(0, &mode);

			m_pp.BackBufferWidth	= mode.Width;
			m_pp.BackBufferHeight	= mode.Height;
			m_pp.BackBufferFormat	= mode.Format;
			m_pp.BackBufferCount	= 1;
			m_pp.hDeviceWindow		= ((Avatar3DWnd*)m_pMainWnd)->m_hWnd;
		}

		m_rsrcUserMgr->freeD3DResources();
		EVE_VALIDATE_HR(m_dev->Reset(&m_pp));
		m_rsrcUserMgr->initD3DResources();
	}
}