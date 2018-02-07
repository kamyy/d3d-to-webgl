#include "BasicTypes.h"
#ifdef _DEBUG
	#include "P3N3B3T2_VS_D.h"
	#include "P3N3B3T2_PS_D.h"
#else
	#include "P3N3B3T2_VS_R.h"
	#include "P3N3B3T2_PS_R.h"
#endif
#include "../Shader.h"

namespace eve {
	P3N3B3T2::P3N3B3T2() : Shader(getName(), VtxMgr::DI_P3N3B3T2, g_vs20_main, g_ps20_main) {}

	void P3N3B3T2::setVSConstData(const Material* mat) const {
		static const Vector1x4 up(0.0f, 0.0f, 1.0f, 0.0f); // world up

		// concatenated world view projection transform
		setVSConstReg(0, 4, s_frame->getWorldMatrix() * s_camera->getViewProjMatrix());
		setVSConstReg(4, 1, s_camera->getPos(s_frame)); // camera pos in model space
		setVSConstReg(5, 1, s_omniLS->getPos(s_frame)); // omniLS pos in model space
		setVSConstReg(6, 1, Vector1x4(s_omniLS->m_coeff0, s_omniLS->m_coeff1, s_omniLS->m_coeff2, 0.f));

		setVSConstReg(7, 1, up * !s_frame->getWorldMatrix()); // world up in model space
	}
	void P3N3B3T2::setPSConstData(const Material* mat) const {
		setPSConstReg(0, 1, s_ambientLS->m_lowerHemisphereColor);
		setPSConstReg(1, 1, s_ambientLS->m_upperHemisphereColor);
		setPSConstReg(2, 1, s_omniLS->m_color);

		Color c(s_omniLS->m_color.m_r * mat->getSpec().m_r,
				s_omniLS->m_color.m_g * mat->getSpec().m_g,
				s_omniLS->m_color.m_b * mat->getSpec().m_b, mat->getShininess());
		setPSConstReg(3, 1, c);
	}
}
