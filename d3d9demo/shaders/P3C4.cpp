#include "BasicTypes.h"
#ifdef _DEBUG
	#include "P3C4_VS_D.h"
	#include "P3C4_PS_D.h"
#else
	#include "P3C4_VS_R.h"
	#include "P3C4_PS_R.h"
#endif
#include "../Shader.h"

namespace eve {
	P3C4::P3C4() : Shader(getName(), VtxMgr::DI_P3C4, g_vs20_main, g_ps20_main) {}

	void P3C4::setVSConstData(const Material* mat) const {
		setVSConstReg(0, 4, s_frame->getWorldMatrix() * s_camera->getViewProjMatrix());
	}
	void P3C4::setPSConstData(const Material* mat) const {
	}
}
