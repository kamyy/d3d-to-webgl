#pragma once
#ifndef Light_h
#define Light_h

#include "RefFrame.h"
#include "Color.h"

namespace eve {
	struct AmbientLS : public RefCountedObject {
		AmbientLS(const Color& gnd, const Color& sky);
		Color m_lowerHemisphereColor;
		Color m_upperHemisphereColor;
	};

	struct PrllLS : public RefFrame { // prll light source
		PrllLS(const Color& color, RefFrame* parent);
		Color m_color;
	};

	struct OmniLS : public RefFrame { // omni light source
		OmniLS(const Color& color, RefFrame* parent, f32 coeff0, f32 m_coeff1, f32 m_coeff2);
		Color m_color;
		f32 m_coeff0;
		f32 m_coeff1;
		f32 m_coeff2;
	};
}

#endif
