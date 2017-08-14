/* 
 * Avatar3D by Kam Yin Yip
 * yy_kam@hotmail.com
 * http://www.geocities.com/tapdanceshoes
 */
#pragma once
#ifndef Avatar3DWnd_h
#define Avatar3DWnd_h

#include "Avatar3DChildWnd.h"

namespace eve {
	class Avatar3DApp;

	class Avatar3DWnd : public CFrameWnd {
		DECLARE_DYNCREATE(Avatar3DWnd)
		DECLARE_MESSAGE_MAP()

	public:
		Avatar3DWnd();
		CStatusBar m_statusBar;
		Avatar3DChildWnd m_childWnd;

		BOOL PreCreateWindow(CREATESTRUCT& cs);
		afx_msg int OnCreate(LPCREATESTRUCT s);
		afx_msg void OnSysCommand(UINT id, LPARAM lparam);

		afx_msg void OnLButtonDown(UINT flags, CPoint point);
		afx_msg void OnLButtonUp(UINT flags, CPoint point);
		afx_msg void OnRButtonDown(UINT flags, CPoint point);
		afx_msg void OnRButtonUp(UINT flags, CPoint point);
		afx_msg void OnMButtonDown(UINT flags, CPoint point);
		afx_msg void OnMButtonUp(UINT flags, CPoint point);
		afx_msg void OnMouseMove(UINT flags, CPoint point);

		afx_msg void OnFileOpen();
		afx_msg void OnFileExit();

		afx_msg void OnViewCameras(UINT id);
		afx_msg void OnViewLights();
		afx_msg void OnDebugWirefrm();
		afx_msg void OnDebugNormals();
		afx_msg void OnToggleFullscreen();
		afx_msg void OnHelpControls();
		afx_msg void OnHelpAbout();

		afx_msg void OnUpdateViewCameras(CCmdUI* ui);
		afx_msg void OnUpdateDebugWirefrm(CCmdUI* ui);
		afx_msg void OnUpdateDebugNormals(CCmdUI* ui);

	protected:
		Avatar3DApp* m_app; // subclass of CWinApp 
	};
}

#endif
