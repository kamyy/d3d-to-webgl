#include "StdAfx.h"

#include "D3D11Demo.h"
#include "Texture.h"
#include "Shader.h"

#include "Material.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    MaterialMap Material::s_materialMap;

    Material::Material(const std::string& name) : m_name(name) {
        s_materialMap[m_name] = this;
    }

    const MaterialMap& Material::getMap() {
        return Material::s_materialMap;
    }

    void Material::initSingleton() {
        assert(s_materialMap.empty());
    }

    void Material::freeSingleton() {
        for (auto& p : s_materialMap) {
            delete p.second;
        }
        s_materialMap.clear();
    }

    Material* Material::get(const std::string& name) {
        auto i = s_materialMap.find(name);
        if (i == s_materialMap.end()) {
            return new Material(name);
        } else {
            return i->second;
        }
    }

    void Material::applyMaterialToD3D11() const {
        for (uint32_t i = 0; i < MAX_TEXTURE_STAGES; ++i) {
            if (m_textures[i]) {
                m_textures[i]->applyTextureToD3D11(i);
            }
        }

        PS_REG_B2 reg;
        reg.g_a = m_diff; // serves as global ambient
        reg.g_d.m_r = g_app.getOmniDirLS()->m_color.m_r * m_diff.m_r;
        reg.g_d.m_g = g_app.getOmniDirLS()->m_color.m_g * m_diff.m_g;
        reg.g_d.m_b = g_app.getOmniDirLS()->m_color.m_b * m_diff.m_b;
        reg.g_d.m_a = 1.0f;
        reg.g_s.m_r = g_app.getOmniDirLS()->m_color.m_r * m_spec.m_r;
        reg.g_s.m_g = g_app.getOmniDirLS()->m_color.m_g * m_spec.m_g;
        reg.g_s.m_b = g_app.getOmniDirLS()->m_color.m_b * m_spec.m_b;
        reg.g_s.m_a = m_shininess;
        m_shader->setPS_REG_B2(reg);

        m_shader->applyShaderToD3D11();
    }

    void Material::setTexture(uint32_t stage, Texture* texture) {
        if (stage < MAX_TEXTURE_STAGES) {
            m_textures[stage] = texture;

            m_translucent = false;
            for (auto p : m_textures) {
                if (p && p->hasAlpha()) {
                    m_translucent = true;
                    break;
                }
            }
        }
    }

    const Texture* Material::getTexture(uint32_t stage) const {
        return m_textures[stage];
    }

    Texture* Material::getTexture(uint32_t stage) {
        return m_textures[stage];
    }

    void Material::setShader(class Shader* shader) {
        m_shader = shader;
    }

    const Shader* Material::getShader() const {
        return m_shader;
    }

    Shader* Material::getShader() {
        return m_shader;
    }

    void to_json(json& out, const MaterialMap& map) {
        for (auto& p : map) {
            const Material* mat = p.second;

            nlohmann::json obj = {
                { "name", p.first.c_str() },
                { "diff", { mat->getDiff().m_r, mat->getDiff().m_g, mat->getDiff().m_b } },
                { "spec", { mat->getSpec().m_r, mat->getSpec().m_g, mat->getSpec().m_b } },
                { "shinyExponent", mat->getShininess()  },
                { "isTranslucent", mat->isTranslucent() },
                { "shaderProgram", mat->getShader()->getName().c_str() },
                { "textures", { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr } }
            };

            const Texture* stage0 = mat->getTexture(0);
            if (stage0) {
                obj["textures"][0] = stage0->getName();
            }

            const Texture* stage1 = mat->getTexture(1);
            if (stage1) {
                obj["textures"][1] = stage1->getName();
            }

            out.push_back(obj);
        }
    }
}
