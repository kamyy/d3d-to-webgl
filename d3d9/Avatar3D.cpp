/* 
 * Avatar3D by Kam Yin Yip
 * yy_kam@hotmail.com
 * http://www.geocities.com/tapdanceshoes
 */
#include "BasicTypes.h"
#include "Resource.h"
#include "Avatar3D.h"
#include "LightsDlg.h"
#include "ASE.h"

#include <direct.h>
#include <stdlib.h>

using namespace eve;

namespace {
	/*
	Avatar3D app;
	LRESULT WINAPI wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		return app.wndProc(hwnd, msg, wparam, lparam);
	}
	*/
}

namespace eve {
/*

	LRESULT Avatar3D::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		case WM_COMMAND:
			switch(LOWORD(wparam)) {
			case ID_FILE_OPENASE:
				{
				OPENFILENAME ofn;
				char file[256] = "";
				memset(&ofn, 0, sizeof(OPENFILENAME));
				ofn.lStructSize		= sizeof(OPENFILENAME);
				ofn.hwndOwner		= hwnd;
				ofn.lpstrFile		= file;
				ofn.nMaxFile		= sizeof(file);
				ofn.lpstrFilter		= "3DS-Max ASCII Export Files (*.ase)\0*.ASE\0";
				ofn.nFilterIndex	= 1;
				ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_HIDEREADONLY;
				ofn.lpstrDefExt		= "ASE";

				if (GetOpenFileName(&ofn)) {
					ASE ase;
					if (false == ase.read(file)) {
						MessageBox(hwnd, "Error reading 3DS-Max ASE file!", NULL, MB_OK | MB_ICONERROR);
					} else {
						initScene(ase);
					}
				}
				break;
				}
			case ID_FILE_EXIT:
				PostMessage(hwnd, WM_CLOSE, 0L, 0L);
				break;

			case ID_DEBUG_WIREFRAME:
				if ((m_drawWireFrm = !m_drawWireFrm)) {
					CheckMenuItem(GetSubMenu(GetMenu(hwnd), 1), 
						ID_DEBUG_WIREFRAME, MF_BYCOMMAND | MF_CHECKED);
				} else {
					CheckMenuItem(GetSubMenu(GetMenu(hwnd), 1), 
						ID_DEBUG_WIREFRAME, MF_BYCOMMAND | MF_UNCHECKED);
				}
				break;
			case ID_DEBUG_NORMALS:
				if ((m_drawNormals = !m_drawNormals)) {
					CheckMenuItem(GetSubMenu(GetMenu(hwnd), 1), 
						ID_DEBUG_NORMALS, MF_BYCOMMAND | MF_CHECKED);
				} else {
					CheckMenuItem(GetSubMenu(GetMenu(hwnd), 1), 
						ID_DEBUG_NORMALS, MF_BYCOMMAND | MF_UNCHECKED);
				}
				break;

			case ID_VIEW_CAMERA1:
				if (m_cameras) {
					CheckMenuRadioItem(GetSubMenu(GetMenu(hwnd), 1), 0, 2, 0, MF_BYPOSITION);
					m_camera = m_cameras->at(0);
					m_target = m_camTgts->at(0);
				}
				break;
			case ID_VIEW_CAMERA2:
				if (m_cameras) {
					CheckMenuRadioItem(GetSubMenu(GetMenu(hwnd), 1), 0, 2, 1, MF_BYPOSITION);
					m_camera = m_cameras->at(1);
					m_target = m_camTgts->at(1);
				}
				break;
			case ID_VIEW_CAMERA3:
				if (m_cameras) {
					CheckMenuRadioItem(GetSubMenu(GetMenu(hwnd), 1), 0, 2, 2, MF_BYPOSITION);
					m_camera = m_cameras->at(2);
					m_target = m_camTgts->at(2);
				}
				break;

			case ID_VIEW_LIGHTS:
				if (!IsWindow(LightsDlg::s_hwndDlg)) {
					LightsDlg::s_hwndDlg = CreateDialog(GetModuleHandle(NULL),
														MAKEINTRESOURCE(IDD_LIGHTS),
														m_hwnd,
														LightsDlgProc);
				}
				break;

			case ID_VIEW_TOGGLEFULLSCREEN:
				m_pp.Windowed = !m_pp.Windowed;
				resetD3DResources();
				m_timer->reset();
				break;

			case ID_HELP_CONTROLS:
				MessageBox(hwnd,"Left drag to tumble\n"
								"Middle drag to pan\n"
								"Right drag to dolly",
								"Help With Camera Controls", MB_OK | MB_ICONINFORMATION);
				break;
			case ID_HELP_ABOUT:
				MessageBox(hwnd,
					"Avatar3D by Kam Yin Yip\n"
					"yy_kam@hotmail.com\n\n"
					"http://www.geocities.com/tapdanceshoes",
					"About Avatar3D", MB_OK | MB_ICONINFORMATION);
				break;

			default:
				break;
			}
			break;

		case WM_INITMENUPOPUP:
			if (m_cameras && GetSubMenu(GetMenu(hwnd), 1) == (HMENU)wparam) {
				HMENU hmenu = GetSubMenu(GetMenu(hwnd), 1); u32 i;

				for (i = 0; i < 3 && i < (u32)m_cameras->size(); ++i)
					EnableMenuItem(hmenu, i, MF_BYPOSITION | MF_ENABLED	);
				for (i = i; i < 3; ++i)
					EnableMenuItem(hmenu, i, MF_BYPOSITION | MF_GRAYED	);

				if (!m_cameras->empty()) {
					if (m_cameras->size() > 0 && m_cameras->at(0) == m_camera)
						CheckMenuRadioItem(hmenu, 0, 2, 0, MF_BYPOSITION);
					else if (m_cameras->size() > 1 && m_cameras->at(1) == m_camera)
						CheckMenuRadioItem(hmenu, 0, 2, 1, MF_BYPOSITION);
					else if (m_cameras->size() > 2 && m_cameras->at(2) == m_camera)
						CheckMenuRadioItem(hmenu, 0, 2, 2, MF_BYPOSITION);
				} else {
					CheckMenuItem(hmenu, 0, MF_BYPOSITION | MF_UNCHECKED);
					CheckMenuItem(hmenu, 1, MF_BYPOSITION | MF_UNCHECKED);
					CheckMenuItem(hmenu, 2, MF_BYPOSITION | MF_UNCHECKED);
				}
			}
			break;

		case WM_LBUTTONDOWN:
			m_leftX = GET_X_LPARAM(lparam);
			m_leftY = GET_Y_LPARAM(lparam);
			m_leftButtonDown = true;
			if (GetCapture() != hwnd) {
				SetCapture(hwnd);
			}
			break;
		case WM_LBUTTONUP:
			m_leftButtonDown = false;
			if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown &&
				GetCapture() == hwnd) {
				ReleaseCapture();
			}
			break;

		case WM_RBUTTONDOWN:
			m_rightX = GET_X_LPARAM(lparam);
			m_rightY = GET_Y_LPARAM(lparam);
			m_rightButtonDown = true;
			if (GetCapture() != hwnd) {
				SetCapture(hwnd);
			}
			break;
		case WM_RBUTTONUP:
			m_rightButtonDown = false;
			if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown && 
				GetCapture() == hwnd) {
				ReleaseCapture();
			}
			break;

		case WM_MBUTTONDOWN:
			m_middleX = GET_X_LPARAM(lparam);
			m_middleY = GET_Y_LPARAM(lparam);
			m_middleButtonDown = true;
			if (GetCapture() != hwnd) {
				SetCapture(m_hwnd);
			}
			break;
		case WM_MBUTTONUP:
			m_middleButtonDown = false;
			if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown && 
				GetCapture() == hwnd) {
				ReleaseCapture();
			}
			break;

		case WM_MOUSEMOVE:
			if (m_leftButtonDown) {
				i32 x = GET_X_LPARAM(lparam);
				i32 y = GET_Y_LPARAM(lparam);
				if (m_camera) {
					m_camera->rotateX(D3DXToRadian(f32(m_leftY - y)), m_target);
					m_target->rotateZ(D3DXToRadian(f32(m_leftX - x)));
				}
				m_leftX = x;
				m_leftY = y;
			}
			if (m_rightButtonDown) {
				i32 x = GET_X_LPARAM(lparam);
				i32 y = GET_Y_LPARAM(lparam);
				if (m_camera) {
					m_camera->translate(Vector1x4(0.0f, f32(x - m_rightX) * 0.1f, 0.0f, 1.0f));
				}
				m_rightX = x;
				m_rightY = y;
			}
			if (m_middleButtonDown) {
				i32 x = GET_X_LPARAM(lparam);
				i32 y = GET_Y_LPARAM(lparam);
				f32 dx = f32(m_middleX - x) * 0.1f;
				f32 dz = f32(y - m_middleY) * 0.1f;
				if (m_camera) {
					Vector1x4 dir = m_camera->map(Vector1x4(dx, 0.f, dz, 0.f), m_target);
					m_target->translate(dir);
				}
				m_middleX = x;
				m_middleY = y;
			}
			break;

		case WM_SYSCOMMAND:
			switch (wparam) {
			case SC_MOVE:
			case SC_SIZE:
			case SC_KEYMENU:
			case SC_MINIMIZE:
			case SC_MAXIMIZE:
			case SC_MONITORPOWER:
				if (!m_pp.Windowed) { // ignore if running fullscreen
					return 1;
				}
				break;

			default:
				break;
			}
			break;

		case WM_NCHITTEST:
			if (!m_pp.Windowed) {
				return HTCLIENT;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	void Avatar3D::initWindow(u32 w, u32 h) {
		WNDCLASSEX wndClassEx = {
			sizeof(WNDCLASSEX), 
			CS_CLASSDC|CS_DBLCLKS, 
			::wndProc,
			0L, 
			0L, 
			GetModuleHandle(NULL), 
			LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)), 
			NULL, 
			NULL, 
			MAKEINTRESOURCE(IDR_MENU1),
			"Avatar3D",
			NULL
		};
		RegisterClassEx(&wndClassEx);

		m_wd = w; // client area width
		m_ht = h; // and height
		RECT rect = { 0, 0, w, h };

		// resize rect to also include other window components before creating window
		AdjustWindowRectEx(&rect, WS_BORDER|WS_CAPTION|WS_SYSMENU, TRUE, WS_EX_ACCEPTFILES);

		m_hwnd = CreateWindowEx(WS_EX_ACCEPTFILES,
								"Avatar3D", 
								"Avatar3D", 
								WS_BORDER|WS_CAPTION|WS_SYSMENU,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								rect.right - rect.left,
								rect.bottom - rect.top,
								GetDesktopWindow(),
								NULL, 
								GetModuleHandle(NULL),
								NULL);
		if (m_hwnd) {
			GetWindowRect(m_hwnd, &m_rect); // save window pos and size
		}
		m_haccel = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	}
	bool Avatar3D::testDevice() {
		D3DCAPS9 caps;
		m_dev->GetDeviceCaps(&caps);

		if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0)) {
			MessageBox(	m_hwnd,
						"Direct3D device does not support pixel shader 2.0 or greater.\n",
						"Error creating D3D Device", MB_OK | MB_ICONERROR);
			return false;
		}
		if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0)) {
			MessageBox(	m_hwnd,
						"Direct3D device does not support vertex shader 2.0 or greater.\n",
						"Error creating D3D Device", MB_OK | MB_ICONERROR);
			return false;
		}
		return true;
	}
	void Avatar3D::initScene(const ASE& ase) {
		freeScene();
		m_rootFrame = new RefFrame;

		m_cameras = new CameraArray;
		m_camTgts = new CamTgtArray;
		ase.createCameras(m_cameras, m_camTgts, m_rootFrame);
		if (m_cameras->empty()) { // no cameras in ase so create a default
			RefFrame* tgt = new RefFrame(m_rootFrame);
			Camera	* cam = new Camera(	CAMERA_DEFAULT_FOV, 
										CAMERA_DEFAULT_ASPECT_RATIO, 
										CAMERA_DEFAULT_N_CLIP_DISTANCE, 
										CAMERA_DEFAULT_F_CLIP_DISTANCE, tgt);
			m_camTgts->push_back(tgt);
			m_cameras->push_back(cam);
			cam->translate(Vector1x4(0.0f, -12.0f, 1.0f, 1.0f));
		}
		m_camera = m_cameras->at(0);
		m_target = m_camTgts->at(0);
		m_actors = new ActorList;

		m_omniLS = new OmniLS(	LightsDlg::getOmniDirectionalRGB(), m_camera,
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
	void Avatar3D::freeScene() {
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
		if (m_omniLS) {
			m_omniLS->rmvRef();
			m_omniLS = 0;
		}
		if (m_ambientLS) {
			m_ambientLS->rmvRef();
			m_ambientLS = 0;
		}
		if (m_mirrorCamera) {
			m_mirrorCamera->rmvRef();
			m_mirrorCamera = 0;
		}

		if (m_cameras) {
			for (CameraArray::iterator i = m_cameras->begin(); i != m_cameras->end(); ++i)
				(*i)->rmvRef();
			delete m_cameras;
			m_cameras = 0;
		}
		if (m_camTgts) {
			for (CamTgtArray::iterator j = m_camTgts->begin(); j != m_camTgts->end(); ++j)
				(*j)->rmvRef();
			delete m_camTgts;
			m_camTgts = 0;
		}
		m_camera = 0;
		m_target = 0;
	}

	HRESULT Avatar3D::initialize(i32 w, i32 h) {
		HRESULT hr = E_FAIL;
		HRESULT hr1;
		HRESULT hr2;

		initWindow(w, h);
		if ((m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION)) && m_hwnd) {
			D3DDISPLAYMODE mode;
			EVE_VALIDATE_HR(m_d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode));
			hr1 = m_d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format,
											D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8);
			hr2 = m_d3d9->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format,
											D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1);
			if (FAILED(hr1) && FAILED(hr2)) {
				MessageBox(	m_hwnd,
							"Neither D3DFMT_D24S8 nor D3DFMT_D15S1 depth stencil formats supported.\n",
							"Error creating D3D Device", 
							MB_OK | MB_ICONERROR);
				return E_FAIL;
			}
			m_pp.BackBufferWidth			= 0;
			m_pp.BackBufferHeight			= 0;
			m_pp.BackBufferFormat			= D3DFMT_UNKNOWN;
			m_pp.BackBufferCount			= 1;
			m_pp.MultiSampleType			= D3DMULTISAMPLE_NONE;
			m_pp.MultiSampleQuality			= 0;
			m_pp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
			m_pp.hDeviceWindow				= m_hwnd;
			m_pp.Windowed					= TRUE;
			m_pp.EnableAutoDepthStencil		= TRUE;
			m_pp.AutoDepthStencilFormat		= SUCCEEDED(hr1) ? D3DFMT_D24S8 : D3DFMT_D15S1;
			m_pp.Flags						= 0;
			m_pp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
			m_pp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;

#if 0
			EVE_VALIDATE_HR(m_d3d9->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hwnd,
    												D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_pp, &m_dev));
#else
			EVE_VALIDATE_HR(m_d3d9->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd,
    												D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_pp, &m_dev));
#endif

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
				return S_OK;
			}
		}
		return E_FAIL;
	}
	void Avatar3D::cleanup() {
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

		if (m_haccel) {
			DestroyAcceleratorTable(m_haccel);
		}
		if (m_dev) {
			m_dev->Release();
		}
		if (m_d3d9) {
			m_d3d9->Release();
		}
	}
	void Avatar3D::msgLoop() {
		ShowWindow(m_hwnd, SW_SHOWDEFAULT);
		HRESULT hr;
		MSG msg;

		msg.message = WM_NULL;
		while (msg.message != WM_QUIT) {
			m_timer->begTiming();
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				if (!IsWindow(LightsDlg::s_hwndDlg) || !IsDialogMessage(LightsDlg::s_hwndDlg, &msg)) { 
					// if successful TranslateAccelerator passes msg directly to window procedure
					if (TranslateAccelerator(m_hwnd, m_haccel, &msg) == 0) {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			} else { // no messages to process so run main game loop
				hr = m_dev->TestCooperativeLevel();
				switch (hr) {
				case D3D_OK:
					draw();
					break;
				case D3DERR_DEVICELOST:
					Sleep(500); // unable to reset device just yet
					break;
				case D3DERR_DEVICENOTRESET:
					resetD3DResources(); // try and reset d3d device and resources
					m_timer->reset();
					break;
				}
			}
			m_timer->endTiming();
		}
	}
	void Avatar3D::draw() {
		m_dev->Clear(0, NULL,	
						D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
						D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);
		if (SUCCEEDED(m_dev->BeginScene())) {
			if (m_drawWireFrm) {
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
				m_dev->SetRenderState(D3DRS_STENCILPASS,	D3DSTENCILOP_REPLACE);
				// stencil test always passes and value written into stencil is 1
				m_dev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
				m_dev->SetRenderState(D3DRS_STENCILREF, 0x00000001);

				Shader::setCamera(m_camera); // render using non-mirrored camera
				m_mirror->drawAtOnce(); // draw mirror into the stencil

				// cull CCW and allow normal rendering into the color buffer
				m_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				m_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				m_dev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
				
				// don't modify stencil when rendering where stencil is setup
				m_dev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
				m_dev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

				Matrix4x4 m = m_camera->getWorldMatrix(); // setup mirror cam
				m *= !m_mirror->getRefFrame(0)->getWorldMatrix();
				m.postCatSxyz(1.0f, 1.0f, -1.0f);
				m *= m_mirror->getRefFrame(0)->getWorldMatrix();

				m_mirrorCamera->setWorldMatrix(m);
				m_mirrorCamera->setFieldOfView(m_camera->getFieldOfView());
				m_mirrorCamera->setAspectRatio(m_camera->getAspectRatio());
				m_mirrorCamera->setClipDistanceN(m_camera->getClipDistanceN());
				m_mirrorCamera->setClipDistanceF(m_camera->getClipDistanceF());
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
			Shader::setCamera(m_camera);

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

	void Avatar3D::resetD3DResources() {
		if (m_pp.Windowed) { // select windowed mode
			SetWindowLong(m_hwnd, GWL_STYLE, WS_BORDER|WS_CAPTION|WS_SYSMENU|WS_VISIBLE);
			SetWindowLong(m_hwnd, GWL_EXSTYLE, WS_EX_ACCEPTFILES);

			// restore saved window size and position on screen
			MoveWindow(	m_hwnd, m_rect.left, m_rect.top, 
						m_rect.right - m_rect.left, 
						m_rect.bottom - m_rect.top, FALSE);

			m_pp.BackBufferWidth = 0;
			m_pp.BackBufferHeight = 0;
			m_pp.BackBufferFormat = D3DFMT_UNKNOWN;
			m_pp.BackBufferCount = 1;

		} else { // select fullscreen exclusive mode
			SetWindowLong(m_hwnd, GWL_STYLE, WS_VISIBLE);

			// retrieve desktop display mode settings
			D3DDISPLAYMODE mode;
			m_dev->GetDisplayMode(0, &mode);

			m_pp.BackBufferWidth = mode.Width;
			m_pp.BackBufferHeight = mode.Height;
			m_pp.BackBufferFormat = mode.Format;
			m_pp.BackBufferCount = 1;
		}

		m_rsrcUserMgr->freeD3DResources();
		HRESULT hr = m_dev->Reset(&m_pp);
		if (SUCCEEDED(hr)) {
			m_rsrcUserMgr->initD3DResources();
		}
	}
	*/
}

/*
i32 WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprev, LPSTR cmdLine, i32 cmdShow) {
	HRESULT hr = app.initialize(800, 600);
	if (SUCCEEDED(hr)) {
		app.msgLoop();
	}
	app.cleanup();
	return 0;
}
*/