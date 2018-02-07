#pragma once
#ifndef Light_h
#define Light_h

#include "RefFrame.h"
#include "Color.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct AmbientLS { // ambient light source
        AmbientLS(const Color& lowerHemisphere, const Color& upperHemisphere)
            : m_lowerHemisphereColor(lowerHemisphere)
            , m_upperHemisphereColor(upperHemisphere) {

        }

        virtual ~AmbientLS() {

        }

        Color m_lowerHemisphereColor;
        Color m_upperHemisphereColor;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct ParallelLS : public RefFrame { // parallel light source
        Color m_color;

        ParallelLS(const Color& color, shared_ptr<RefFrame> parent)
            : RefFrame(parent)
            , m_color(color) {

        }

        virtual ~ParallelLS() {

        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct OmniDirLS : public RefFrame { // omni light source
        Color m_color;
        float m_coeff0;
        float m_coeff1;
        float m_coeff2;

        OmniDirLS(const Color& color, float coeff0, float coeff1, float coeff2, shared_ptr<RefFrame> parent)
            : RefFrame(parent)
            , m_color(color) {
            m_coeff0 = coeff0;
            m_coeff1 = coeff1;
            m_coeff2 = coeff2;
        }

        virtual ~OmniDirLS() {

        }
    };

}

#endif
