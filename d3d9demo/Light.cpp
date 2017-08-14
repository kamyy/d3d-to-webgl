#include "BasicTypes.h"
#include "Light.h"

namespace eve {
	AmbientLS::AmbientLS(const Color& lowerHemisphere, const Color& upperHemisphere) :
		m_lowerHemisphereColor(lowerHemisphere), m_upperHemisphereColor(upperHemisphere) {
	}

	PrllLS::PrllLS(const Color& color, RefFrame* parent) 
		: RefFrame(parent), m_color(color) {
		m_type = PRLL_LIGHT;
	}

	OmniLS::OmniLS(const Color& color, RefFrame* parent, f32 coeff0, f32 coeff1, f32 coeff2) 
		: RefFrame(parent), m_color(color) {
		m_type = OMNI_LIGHT;
		m_coeff0 = coeff0;
		m_coeff1 = coeff1;
		m_coeff2 = coeff2;
	}
}