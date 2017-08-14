#pragma once
#ifndef LightsDlg_h
#define LightsDlg_h

namespace eve {
	class LightsDlg : public CDialog {
		DECLARE_MESSAGE_MAP()
	public:
		LightsDlg(CWnd* parentWnd);

		static const Color& getLowerHemisphereRGB() { 
			return s_lowerHemisphereRGB; 
		}
		static const Color& getUpperHemisphereRGB() { 
			return s_upperHemisphereRGB; 
		}
		static const Color& getOmniDirectionalRGB() { 
			return s_omniDirectionalRGB; 
		}
		static f32 getAttenCoeff0() { 
			return s_attenCoeff0; 
		}
		static f32 getAttenCoeff1() { 
			return s_attenCoeff1; 
		}
		static f32 getAttenCoeff2() { 
			return s_attenCoeff2; 
		}

		virtual void PostNcDestroy();
		virtual BOOL OnInitDialog();
		virtual void OnCancel();
		virtual void OnOK();

	protected:
		afx_msg void OnHScroll(UINT code, UINT pos, CScrollBar* scrollBar);

		static Color s_lowerHemisphereRGB;
		static Color s_upperHemisphereRGB;
		static Color s_omniDirectionalRGB;
		static f32 s_attenCoeff0;
		static f32 s_attenCoeff1;
		static f32 s_attenCoeff2;
	};

	BOOL WINAPI LightsDlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
}

#endif
