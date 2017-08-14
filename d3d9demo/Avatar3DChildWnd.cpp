#include "BasicTypes.h"
#include "Avatar3DApp.h"
#include "Avatar3DChildWnd.h"

namespace eve {
	IMPLEMENT_DYNCREATE(Avatar3DChildWnd, CWnd)

	BEGIN_MESSAGE_MAP(Avatar3DChildWnd, CWnd)
		ON_WM_ERASEBKGND()
		ON_WM_PAINT()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_RBUTTONDOWN()
		ON_WM_RBUTTONUP()
		ON_WM_MBUTTONDOWN()
		ON_WM_MBUTTONUP()
		ON_WM_MOUSEMOVE()
	END_MESSAGE_MAP()

	Avatar3DChildWnd::Avatar3DChildWnd() {
		m_app = (Avatar3DApp*)AfxGetApp();
		m_leftX = 0;
		m_leftY = 0;
		m_leftButtonDown = false;
		m_rightX = 0;
		m_rightY = 0;
		m_rightButtonDown = false;
		m_middleX = 0;
		m_middleY = 0;
		m_middleButtonDown = false;
	}
	BOOL Avatar3DChildWnd::PreCreateWindow(CREATESTRUCT& cs) {
		if (!CWnd::PreCreateWindow(cs)) {
			return FALSE;
		}
		return TRUE;
	}

	BOOL Avatar3DChildWnd::OnEraseBkgnd(CDC* dev) {
		return TRUE;
	}
	void Avatar3DChildWnd::OnPaint() {
		CWnd::OnPaint();
		m_app->tick();
	}

	void Avatar3DChildWnd::OnLButtonDown(UINT flags, CPoint point) {
		m_leftX = point.x;
		m_leftY = point.y;
		m_leftButtonDown = true;
		if (GetCapture() != this) {
			SetCapture();
		}
	}
	void Avatar3DChildWnd::OnLButtonUp(UINT flags, CPoint point) {
		m_leftButtonDown = false;
		if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown 
				&& GetCapture() == this) {
			ReleaseCapture();
		}
	}

	void Avatar3DChildWnd::OnRButtonDown(UINT flags, CPoint point) {
		m_rightX = point.x;
		m_rightY = point.y;
		m_rightButtonDown = true;
		if (GetCapture() != this) {
			SetCapture();
		}
	}
	void Avatar3DChildWnd::OnRButtonUp(UINT flags, CPoint point) {
		m_rightButtonDown = false;
		if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown 
				&& GetCapture() == this) {
			ReleaseCapture();
		}
	}

	void Avatar3DChildWnd::OnMButtonDown(UINT flags, CPoint point) {
		m_middleX = point.x;
		m_middleY = point.y;
		m_middleButtonDown = true;
		if (GetCapture() != this) {
			SetCapture();
		}
	}
	void Avatar3DChildWnd::OnMButtonUp(UINT flags, CPoint point) {
		m_middleButtonDown = false;
		if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown 
				&& GetCapture() == this) {
			ReleaseCapture();
		}
	}

	void Avatar3DChildWnd::OnMouseMove(UINT flags, CPoint point) {
		RefFrame* target = m_app->getActiveCamTgt();
		Camera* camera = m_app->getActiveCamera();

		if (m_leftButtonDown) {
			i32 x = point.x;
			i32 y = point.y;
			if (camera) {
				camera->rotateX(D3DXToRadian(f32(m_leftY - y)), target); // pitch about camera target's x-axis
				target->rotateZ(D3DXToRadian(f32(m_leftX - x))); // yaw about camera's own z-axis
			}
			m_leftX = x;
			m_leftY = y;
		}
		if (m_rightButtonDown) {
			i32 x = point.x;
			i32 y = point.y;
			if (camera) {
				camera->translate(Vector1x4(0.0f, // move along camera's own y-axis
											f32(x - m_rightX) * 0.1f, 
											0.0f, 
											1.0f)); 
			}
			m_rightX = x;
			m_rightY = y;
		}
		if (m_middleButtonDown) {
			i32 x = point.x;
			i32 y = point.y;
			f32 dx = f32(m_middleX - x) * 0.1f;
			f32 dz = f32(y - m_middleY) * 0.1f;
			if (camera) { // map movement in camera space into target space
				Vector1x4 dir = camera->map(Vector1x4(dx, 0.f, dz, 0.f), target); 
				target->translate(dir); // move along own axes
			}
			m_middleX = x;
			m_middleY = y;
		}
	}

}
