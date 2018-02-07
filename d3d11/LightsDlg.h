#pragma once
#ifndef LightsDlg_h
#define LightsDlg_h

#include "Color.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class LightsDlg : public CDialog {
        DECLARE_MESSAGE_MAP()

        afx_msg void OnHScroll(UINT code, UINT pos, CScrollBar* scrollBar);

        static Color s_lowerHemisphereRGB;
        static Color s_upperHemisphereRGB;
        static Color s_omniDirectionalRGB;
        static float s_attenCoeff0;
        static float s_attenCoeff1;
        static float s_attenCoeff2;

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
        static float getAttenCoeff0() {
            return s_attenCoeff0;
        }
        static float getAttenCoeff1() {
            return s_attenCoeff1;
        }
        static float getAttenCoeff2() {
            return s_attenCoeff2;
        }

        virtual void PostNcDestroy();
        virtual BOOL OnInitDialog();
        virtual void OnCancel();
        virtual void OnOK();

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    BOOL WINAPI LightsDlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

}

#endif
