#pragma once
#ifndef Avatar3DChildWnd_h
#define Avatar3DChildWnd_h

class Avatar3DApp;

namespace eve {
	class Avatar3DChildWnd : public CWnd {
		DECLARE_DYNCREATE(Avatar3DChildWnd)
		DECLARE_MESSAGE_MAP()
	public:
		Avatar3DChildWnd();

		BOOL PreCreateWindow(CREATESTRUCT& cs);
		afx_msg BOOL OnEraseBkgnd(CDC* device);
		afx_msg void OnPaint();

		afx_msg void OnLButtonDown(UINT flags, CPoint point);
		afx_msg void OnLButtonUp(UINT flags, CPoint point);
		afx_msg void OnRButtonDown(UINT flags, CPoint point);
		afx_msg void OnRButtonUp(UINT flags, CPoint point);
		afx_msg void OnMButtonDown(UINT flags, CPoint point);
		afx_msg void OnMButtonUp(UINT flags, CPoint point);
		afx_msg void OnMouseMove(UINT flags, CPoint point);

	protected:
		Avatar3DApp* m_app; // subclass of CWinApp 

		i32		m_leftX;
		i32		m_leftY;
		bool	m_leftButtonDown;

		i32		m_rightX;
		i32		m_rightY;
		bool	m_rightButtonDown;

		i32		m_middleX;
		i32		m_middleY;
		bool	m_middleButtonDown;
	};
}

#endif
