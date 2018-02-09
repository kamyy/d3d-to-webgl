#pragma once
#ifndef Light_h
#define Light_h

#include "RefFrame.h"
#include "Color.h"

#include "json/single_include/nlohmann/json.hpp"

namespace d3d11demo {

    using json = nlohmann::json;

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

        virtual json toJSON() const override {
            const Matrix4x4& m = getModelMatrix();

            json node = {
                { "nodeType", "OmniDirLS" },
                { "modelMatrix", { m.m_11, m.m_12, m.m_13, m.m_14, m.m_21, m.m_22, m.m_23, m.m_24, m.m_31, m.m_32, m.m_33, m.m_34, m.m_41, m.m_42, m.m_43, m.m_44 } },
                { "color", { {"r", m_color.m_r}, {"g", m_color.m_g}, {"b", m_color.m_b} } },
                { "coeff0", m_coeff0 },
                { "coeff1", m_coeff1 },
                { "coeff2", m_coeff2 }
            };

            for (auto p = getChild(); p != nullptr; p = p->getNext()) {
                node["children"].push_back(p->toJSON());
            }

            return node;
        }
    };

}

#endif
