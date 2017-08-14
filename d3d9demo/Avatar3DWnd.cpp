/* 
 * Avatar3D by Kam Yin Yip
 * yy_kam@hotmail.com
 * http://www.geocities.com/tapdanceshoes
 */
#include "BasicTypes.h"

#include "Avatar3DApp.h"
#include "Avatar3DWnd.h"
#include "Avatar3DChildWnd.h"

#include "LightsDlg.h"
#include "Resource.h"

namespace eve {
	IMPLEMENT_DYNCREATE(Avatar3DWnd, CFrameWnd)

	BEGIN_MESSAGE_MAP(Avatar3DWnd, CFrameWnd)
		ON_WM_CREATE()
		ON_WM_SYSCOMMAND()

		// Avatar3DWnd client area mouse activity is redirected to Avatar3DChildWnd
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_RBUTTONDOWN()
		ON_WM_RBUTTONUP()
		ON_WM_MBUTTONDOWN()
		ON_WM_MBUTTONUP()
		ON_WM_MOUSEMOVE()

		ON_COMMAND(ID_FILE_OPENASE,	OnFileOpen)
		ON_COMMAND(ID_FILE_EXIT,	OnFileExit)

		ON_COMMAND_RANGE(ID_VIEW_CAMERA1, ID_VIEW_CAMERA3, OnViewCameras)
		ON_COMMAND(ID_VIEW_TOGGLEFULLSCREEN, OnToggleFullscreen)
		ON_COMMAND(ID_DEBUG_WIREFRAME,	OnDebugWirefrm)
		ON_COMMAND(ID_DEBUG_NORMALS,	OnDebugNormals)
		ON_COMMAND(ID_VIEW_LIGHTS,		OnViewLights)
		ON_COMMAND(ID_HELP_CONTROLS,	OnHelpControls)
		ON_COMMAND(ID_HELP_ABOUT,		OnHelpAbout)

		ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_CAMERA1, ID_VIEW_CAMERA3, OnUpdateViewCameras)
		ON_UPDATE_COMMAND_UI(ID_DEBUG_WIREFRAME,	OnUpdateDebugWirefrm)
		ON_UPDATE_COMMAND_UI(ID_DEBUG_NORMALS,		OnUpdateDebugNormals)
	END_MESSAGE_MAP()

	Avatar3DWnd::Avatar3DWnd() {
		m_app = (Avatar3DApp*)AfxGetApp();
	}

	BOOL Avatar3DWnd::PreCreateWindow(CREATESTRUCT& cs) {
		if (!CFrameWnd::PreCreateWindow(cs)) {
			return FALSE;
		}

		RECT rect = { 0, 0, 800, 600 }; // desired width and height of client area
		AdjustWindowRectEx(	&rect, // factor in menu bar, title bar, border etc.
							WS_BORDER|WS_CAPTION|WS_SYSMENU, 
							TRUE, 
							0);

		cs.style		= WS_BORDER|WS_CAPTION|WS_SYSMENU;
 		cs.cx			= rect.right - rect.left;
		cs.cy			= rect.bottom - rect.top;

		return TRUE;
	}
	int Avatar3DWnd::OnCreate(LPCREATESTRUCT s) {
		if (CFrameWnd::OnCreate(s) != -1) {
			if (!m_childWnd.CreateEx(	0, NULL, NULL, WS_CHILD|WS_VISIBLE, 
										CRect(0, 0, 800, 600), this, AFX_IDW_PANE_FIRST)) {
				return -1;
			}

			if (!m_statusBar.Create(this)) {
				return -1;
			}

			UINT indicators[] = {	
				ID_SEPARATOR,
				IDS_AVG_FPS,
				IDS_AVG_SPF,
				IDS_CLIENT_SIZE
			};
			m_statusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
			m_statusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL | SBPS_STRETCH, 0);
			m_statusBar.SetPaneInfo(1, IDS_AVG_FPS, SBPS_NORMAL, 128);
			m_statusBar.SetPaneInfo(2, IDS_AVG_SPF, SBPS_NORMAL, 128);
			m_statusBar.SetPaneInfo(3, IDS_CLIENT_SIZE, SBPS_NORMAL, 128);

			return 0;
		}
		return -1;
	}

	void Avatar3DWnd::OnSysCommand(UINT id, LPARAM lparam) {
		switch (id & 0xFFF0) {
		case SC_MOVE:
		case SC_SIZE:
		case SC_KEYMENU:
		case SC_HSCROLL:
		case SC_VSCROLL:
		case SC_MINIMIZE:
		case SC_MAXIMIZE:
		case SC_MONITORPOWER:
			if (m_app->isFullscreen()) { // ignore if running fullscreen
				break;
			}
		default:
			CFrameWnd::OnSysCommand(id, lparam);
			break;
		}
	}

	void Avatar3DWnd::OnLButtonDown(UINT flags, CPoint point) {
		m_childWnd.OnLButtonDown(flags, point);
	}
	void Avatar3DWnd::OnLButtonUp(UINT flags, CPoint point) {
		m_childWnd.OnLButtonUp(flags, point);
	}

	void Avatar3DWnd::OnRButtonDown(UINT flags, CPoint point) {
		m_childWnd.OnRButtonDown(flags, point);
	}
	void Avatar3DWnd::OnRButtonUp(UINT flags, CPoint point) {
		m_childWnd.OnRButtonUp(flags, point);
	}

	void Avatar3DWnd::OnMButtonDown(UINT flags, CPoint point) {
		m_childWnd.OnMButtonDown(flags, point);
	}
	void Avatar3DWnd::OnMButtonUp(UINT flags, CPoint point) {
		m_childWnd.OnMButtonUp(flags, point);
	}

	void Avatar3DWnd::OnMouseMove(UINT flags, CPoint point) {
		m_childWnd.OnMouseMove(flags, point);
	}

	void Avatar3DWnd::OnFileOpen() {
		static CString file = "";

		CFileDialog dialog(	TRUE, "ASE", file, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, 
							"3DS-Max ASCII Export Files (*.ase)|*.ASE||");
		if (dialog.DoModal() == IDOK) {
			ASE ase;
			if (!ase.read(dialog.GetPathName())) {
				MessageBox("Error reading 3DS-Max ASE file!", NULL, MB_OK | MB_ICONERROR);
			} else {
				file = dialog.GetPathName();
				m_app->initScene(ase);
			}
		}
	}
	void Avatar3DWnd::OnFileExit() {
		PostMessage(WM_CLOSE, 0L, 0L);
	}

	void Avatar3DWnd::OnViewCameras(UINT id) {
		if (m_app->getCameraCount() > 0) {
			CMenu* menu = GetMenu()->GetSubMenu(1);
			menu->CheckMenuRadioItem(ID_VIEW_CAMERA1, ID_VIEW_CAMERA3, id, MF_BYCOMMAND);
			const u32 i = id - ID_VIEW_CAMERA1;
			m_app->setActiveCamera(i);
			m_app->setActiveCamTgt(i);
		}
	}
	void Avatar3DWnd::OnViewLights() {
		// LightDlg::PostNcDestroy() will release the memory
		LightsDlg* lightsDlg = new LightsDlg(this);
	}

	void Avatar3DWnd::OnDebugWirefrm() {
		m_app->toggleDrawWirefrm();
	}
	void Avatar3DWnd::OnDebugNormals() {
		m_app->toggleDrawNormals();
	}
	void Avatar3DWnd::OnToggleFullscreen() {
		m_app->toggleFullscreen();
	}

	void Avatar3DWnd::OnHelpControls() {
		MessageBox(	"Left drag to tumble\n"
					"Middle drag to pan\n"
					"Right drag to dolly",
					"Help With Camera Controls", MB_OK | MB_ICONINFORMATION);
	}
	void Avatar3DWnd::OnHelpAbout() {
		MessageBox(	"Avatar3D by Kam Yin Yip\n"
					"yy_kam@hotmail.com\n\n"
					"http://www.geocities.com/tapdanceshoes",
					"About Avatar3D", MB_OK | MB_ICONINFORMATION);
	}

	void Avatar3DWnd::OnUpdateViewCameras(CCmdUI* ui) {
		u32 cameraCount = m_app->getCameraCount();

		ui->Enable(TRUE);
		ui->SetRadio(FALSE);

		switch (ui->m_nID) {
		case ID_VIEW_CAMERA1:
			if (cameraCount > 0) {
				ui->SetRadio(m_app->getActiveCamera() == m_app->getCamera(0));
			} else {
				ui->Enable(FALSE);
			}
			break;
		case ID_VIEW_CAMERA2:
			if (cameraCount > 1) {
				ui->SetRadio(m_app->getActiveCamera() == m_app->getCamera(1));
			} else {
				ui->Enable(FALSE);
			}
			break;
		case ID_VIEW_CAMERA3:
			if (cameraCount > 2) {
				ui->SetRadio(m_app->getActiveCamera() == m_app->getCamera(2));
			} else {
				ui->Enable(FALSE);
			}
			break;
		}
	}
	void Avatar3DWnd::OnUpdateDebugWirefrm(CCmdUI* ui) {
		if (m_app->isDrawWirefrmOn()) {
			ui->SetCheck(1);
		} else {
			ui->SetCheck(0);
		}
	}
	void Avatar3DWnd::OnUpdateDebugNormals(CCmdUI* ui) {
		if (m_app->isDrawNormalsOn()) {
			ui->SetCheck(1);
		} else {
			ui->SetCheck(0);
		}
	}
}
