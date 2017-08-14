#include "BasicTypes.h"
#include <commctrl.h>
#include "Resource.h"
#include "LightsDlg.h"
#include "Shader.h"

namespace eve {
	Color LightsDlg::s_lowerHemisphereRGB(.24f, .24f, .24f);
	Color LightsDlg::s_upperHemisphereRGB(.24f, .24f, .24f);
	Color LightsDlg::s_omniDirectionalRGB(.96f, .96f, .96f);
	f32 LightsDlg::s_attenCoeff0 = 1.0000f;
	f32 LightsDlg::s_attenCoeff1 = 0.0020f;
	f32 LightsDlg::s_attenCoeff2 = 0.0006f;

	const f32 kMinRGB = 0.0f;
	const f32 kMaxRGB = 3.0f;
	const f32 kMinCoeff0 = 1.0f;
	const f32 kMaxCoeff0 = 8.0f;
	const f32 kMinCoeff1 = 0.00f;
	const f32 kMaxCoeff1 = 0.10f;
	const f32 kMinCoeff2 = 0.00f;
	const f32 kMaxCoeff2 = 0.01f;

	BEGIN_MESSAGE_MAP(LightsDlg, CDialog)
		ON_WM_HSCROLL()
	END_MESSAGE_MAP()

	LightsDlg::LightsDlg(CWnd* parentWnd) {
		Create(IDD_LIGHTS, parentWnd);
	}

	BOOL LightsDlg::OnInitDialog() {
		char str[12];
		f32 value;

		CSliderCtrl* lhrSlider = (CSliderCtrl*)GetDlgItem(IDC_LHR_SLIDER);
		CSliderCtrl* lhgSlider = (CSliderCtrl*)GetDlgItem(IDC_LHG_SLIDER);
		CSliderCtrl* lhbSlider = (CSliderCtrl*)GetDlgItem(IDC_LHB_SLIDER);
		lhrSlider->SetRange(0, 100);
		lhgSlider->SetRange(0, 100);
		lhbSlider->SetRange(0, 100);
		lhrSlider->SetPageSize(5);
		lhgSlider->SetPageSize(5);
		lhbSlider->SetPageSize(5);
		value = (s_lowerHemisphereRGB.m_r - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		lhrSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_lowerHemisphereRGB.m_r);
		SetDlgItemText(IDC_LHR, str);
		value = (s_lowerHemisphereRGB.m_g - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		lhgSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_lowerHemisphereRGB.m_g);
		SetDlgItemText(IDC_LHG, str);
		value = (s_lowerHemisphereRGB.m_b - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		lhbSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_lowerHemisphereRGB.m_b);
		SetDlgItemText(IDC_LHB, str);

		CSliderCtrl* uhrSlider = (CSliderCtrl*)GetDlgItem(IDC_UHR_SLIDER);
		CSliderCtrl* uhgSlider = (CSliderCtrl*)GetDlgItem(IDC_UHG_SLIDER);
		CSliderCtrl* uhbSlider = (CSliderCtrl*)GetDlgItem(IDC_UHB_SLIDER);
		uhrSlider->SetRange(0, 100);
		uhgSlider->SetRange(0, 100);
		uhbSlider->SetRange(0, 100);
		uhrSlider->SetPageSize(5);
		uhgSlider->SetPageSize(5);
		uhbSlider->SetPageSize(5);
		value = (s_upperHemisphereRGB.m_r - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		uhrSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_upperHemisphereRGB.m_r);
		SetDlgItemText(IDC_UHR, str);
		value = (s_upperHemisphereRGB.m_g - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		uhgSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_upperHemisphereRGB.m_g);
		SetDlgItemText(IDC_UHG, str);
		value = (s_upperHemisphereRGB.m_b - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		uhbSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_upperHemisphereRGB.m_b);
		SetDlgItemText(IDC_UHB, str);

		CSliderCtrl* odrSlider = (CSliderCtrl*)GetDlgItem(IDC_ODR_SLIDER);
		CSliderCtrl* odgSlider = (CSliderCtrl*)GetDlgItem(IDC_ODG_SLIDER);
		CSliderCtrl* odbSlider = (CSliderCtrl*)GetDlgItem(IDC_ODB_SLIDER);
		odrSlider->SetRange(0, 100);
		odgSlider->SetRange(0, 100);
		odbSlider->SetRange(0, 100);
		odrSlider->SetPageSize(5);
		odgSlider->SetPageSize(5);
		odbSlider->SetPageSize(5);
		value = (s_omniDirectionalRGB.m_r - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		odrSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_omniDirectionalRGB.m_r);
		SetDlgItemText(IDC_OMNIR, str);
		value = (s_omniDirectionalRGB.m_g - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		odgSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_omniDirectionalRGB.m_g);
		SetDlgItemText(IDC_OMNIG, str);
		value = (s_omniDirectionalRGB.m_b - kMinRGB) / (kMaxRGB - kMinRGB) * 100.0f;
		odbSlider->SetPos(LONG(value + .5f));
		sprintf(str, "%-3.2f", s_omniDirectionalRGB.m_b);
		SetDlgItemText(IDC_OMNIB, str);

		CSliderCtrl* c0Slider = (CSliderCtrl*)GetDlgItem(IDC_C0_SLIDER);
		CSliderCtrl* c1Slider = (CSliderCtrl*)GetDlgItem(IDC_C1_SLIDER);
		CSliderCtrl* c2Slider = (CSliderCtrl*)GetDlgItem(IDC_C2_SLIDER);
		c0Slider->SetRange(0, 100);
		c1Slider->SetRange(0, 100);
		c2Slider->SetRange(0, 100);
		c0Slider->SetPageSize(5);
		c1Slider->SetPageSize(5);
		c2Slider->SetPageSize(5);
		value = (s_attenCoeff0 - kMinCoeff0) / (kMaxCoeff0 - kMinCoeff0) * 100.0f;
		c0Slider->SetPos(LONG(value + .5f));
		sprintf(str, "%-5.4f", s_attenCoeff0);
		SetDlgItemText(IDC_C0_SLIDER, str);
		value = (s_attenCoeff1 - kMinCoeff1) / (kMaxCoeff1 - kMinCoeff1) * 100.0f;
		c1Slider->SetPos(LONG(value + .5f));
		sprintf(str, "%-5.4f", s_attenCoeff1);
		SetDlgItemText(IDC_C1_SLIDER, str);
		value = (s_attenCoeff2 - kMinCoeff2) / (kMaxCoeff2 - kMinCoeff2) * 100.0f;
		c2Slider->SetPos(LONG(value + .5f));
		sprintf(str, "%-5.4f", s_attenCoeff2);
		SetDlgItemText(IDC_C2_SLIDER, str);

		return TRUE;
	}

	void LightsDlg::PostNcDestroy() {
		CDialog::PostNcDestroy();
		delete this;
	}
	void LightsDlg::OnCancel() {
		DestroyWindow();
	}
	void LightsDlg::OnOK() {
		CDialog::OnOK();
	}

	void LightsDlg::OnHScroll(UINT code, UINT pos, CScrollBar* scrollBar) {
		CSliderCtrl* slider = (CSliderCtrl*)scrollBar;
		f32 value = (f32)slider->GetPos();
		char str[12];

		switch (slider->GetDlgCtrlID()) {
		case IDC_LHR_SLIDER:
			s_lowerHemisphereRGB.m_r = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_lowerHemisphereRGB.m_r);
			SetDlgItemText(IDC_LHR, str);
			break;
		case IDC_LHG_SLIDER:
			s_lowerHemisphereRGB.m_g = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_lowerHemisphereRGB.m_g);
			SetDlgItemText(IDC_LHG, str);
			break;
		case IDC_LHB_SLIDER:
			s_lowerHemisphereRGB.m_b = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_lowerHemisphereRGB.m_b);
			SetDlgItemText(IDC_LHB, str);
			break;

		case IDC_UHR_SLIDER:
			s_upperHemisphereRGB.m_r = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_upperHemisphereRGB.m_r);
			SetDlgItemText(IDC_UHR, str);
			break;
		case IDC_UHG_SLIDER:
			s_upperHemisphereRGB.m_g = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_upperHemisphereRGB.m_g);
			SetDlgItemText(IDC_UHG, str);
			break;
		case IDC_UHB_SLIDER:
			s_upperHemisphereRGB.m_b = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_upperHemisphereRGB.m_b);
			SetDlgItemText(IDC_UHB, str);
			break;

		case IDC_ODR_SLIDER:
			s_omniDirectionalRGB.m_r = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_omniDirectionalRGB.m_r);
			SetDlgItemText(IDC_OMNIR, str);
			break;
		case IDC_ODG_SLIDER:
			s_omniDirectionalRGB.m_g = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_omniDirectionalRGB.m_g);
			SetDlgItemText(IDC_OMNIG, str);
			break;
		case IDC_ODB_SLIDER:
			s_omniDirectionalRGB.m_b = kMinRGB + value * 0.01f * (kMaxRGB - kMinRGB);
			sprintf(str, "%-3.2f", s_omniDirectionalRGB.m_b);
			SetDlgItemText(IDC_OMNIB, str);
			break;

		case IDC_C0_SLIDER:
			s_attenCoeff0 = kMinCoeff0 + value * 0.01f * (kMaxCoeff0 - kMinCoeff0);
			sprintf(str, "%-5.4f", s_attenCoeff0);
			SetDlgItemText(IDC_C0, str);
			break;
		case IDC_C1_SLIDER:
			s_attenCoeff1 = kMinCoeff1 + value * 0.01f * (kMaxCoeff1 - kMinCoeff1);
			sprintf(str, "%-5.4f", s_attenCoeff1);
			SetDlgItemText(IDC_C1, str);
			break;
		case IDC_C2_SLIDER:
			s_attenCoeff2 = kMinCoeff2 + value * 0.01f * (kMaxCoeff2 - kMinCoeff2);
			sprintf(str, "%-5.4f", s_attenCoeff2);
			SetDlgItemText(IDC_C2, str);
			break;
		}

		switch (slider->GetDlgCtrlID()) {
		case IDC_LHR_SLIDER:
		case IDC_LHG_SLIDER:
		case IDC_LHB_SLIDER:
		case IDC_UHR_SLIDER:
		case IDC_UHG_SLIDER:
		case IDC_UHB_SLIDER:
			if (Shader::getAmbientLS()) {
				Shader::getAmbientLS()->m_lowerHemisphereColor = s_lowerHemisphereRGB;
				Shader::getAmbientLS()->m_upperHemisphereColor = s_upperHemisphereRGB;
			}
			break;
		case IDC_ODR_SLIDER:
		case IDC_ODG_SLIDER:
		case IDC_ODB_SLIDER:
		case IDC_C0_SLIDER:
		case IDC_C1_SLIDER:
		case IDC_C2_SLIDER:
			if (Shader::getOmniLS()) {
				Shader::getOmniLS()->m_color = s_omniDirectionalRGB;
				Shader::getOmniLS()->m_coeff0 = s_attenCoeff0;
				Shader::getOmniLS()->m_coeff1 = s_attenCoeff1;
				Shader::getOmniLS()->m_coeff2 = s_attenCoeff2;
			}
			break;
		}
	}
}
