#pragma once
#ifndef Shader_h
#define Shader_h

#include "D3D11Resource.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Math.h"

#include "json/single_include/nlohmann/json.hpp"

/*
// concatenated world view projection transform
setVSConstReg(6, 1, Vector1x4(s_omniLS->m_coeff0, s_omniLS->m_coeff1, s_omniLS->m_coeff2, 0.f));

setPSConstReg(0, 1, s_ambientLS->m_lowerHemisphereColor);
setPSConstReg(1, 1, s_ambientLS->m_upperHemisphereColor);
*/

namespace d3d11demo {

    using json = nlohmann::json;

    using ShaderMap = std::map<std::string, class Shader*>;
    using VtxVector = std::vector<float>;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct VS_REG_B0 { // per tick
        float g_att_coeff[4]; // omni light source attenuation coefficients
    };

    struct VS_REG_B1 { // per model
        Matrix4x4 g_model_view_proj_matrix; // model to view to projection space transform
        Vector1x4 g_camera_pos; // camera position in model space
        Vector1x4 g_omniLS_pos; // omniLS position in model space
        Vector1x4 g_up_dir; // world up in model space
    };

    struct PS_REG_B0 { // per tick
        Color g_intensity; // omni light source intensity
        Color g_gnd; // ground hemisphere light intensity
        Color g_sky; // sky hemisphere light intensity
    };

    struct PS_REG_B1 { // per model
        Vector1x4 g_up_dir; // world up in model space
    };

    struct PS_REG_B2 { // per material
        Color g_a; // material ambient color
        Color g_d; // omni light intensity * material diff color
        Color g_s; // omni light intensity * material spec color and material shininess
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Shader : public D3D11Resource {
        static ShaderMap s_shaderMap;

        static ID3D11Buffer* s_VS_REG_B0;
        static ID3D11Buffer* s_VS_REG_B1;

        static ID3D11Buffer* s_PS_REG_B0;
        static ID3D11Buffer* s_PS_REG_B1;
        static ID3D11Buffer* s_PS_REG_B2;

        ID3D11VertexShader* m_vs = nullptr;
        ID3D11PixelShader * m_ps = nullptr;
        ID3D11InputLayout * m_inputLayout = nullptr;

        const std::string m_name;
        const D3D11_INPUT_ELEMENT_DESC* m_elements;
        const UINT m_numElements;

        const BYTE*  m_vsBlob;
        const BYTE*  m_psBlob;
        const size_t m_vsSize;
        const size_t m_psSize;

        virtual void initD3D11Resources() override;
        virtual void freeD3D11Resources() override;

    public:
        static void initSingleton();
        static void freeSingleton();
        static Shader* get(const std::string& name);

        static void setVS_REG_B0(const VS_REG_B0& reg);
        static void setVS_REG_B1(const VS_REG_B1& reg);
        static void setPS_REG_B0(const PS_REG_B0& reg);
        static void setPS_REG_B1(const PS_REG_B1& reg);
        static void setPS_REG_B2(const PS_REG_B2& reg);

        Shader(const std::string& name, const D3D11_INPUT_ELEMENT_DESC* elements, UINT numElements,
               const BYTE* vsBlob, size_t vsSize,
               const BYTE* psBlob, size_t psSize);
       ~Shader();

        const std::string& getName() const {
            return m_name;
        }

        void applyShaderToD3D11() const;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct Vertex_P3C4 {
        float m_x, m_y, m_z, m_r, m_g, m_b, m_a;

        Vertex_P3C4() {}
        Vertex_P3C4(float x, float y, float z, float r, float g, float b, float a)
                : m_x(x), m_y(y), m_z(z), m_r(r), m_g(g), m_b(b), m_a(a) {}

        bool operator<(const Vertex_P3C4& rhs) const { return memcmp(this, &rhs, sizeof(float) * 7) < 0; }
        operator VtxVector() const { return { m_x, m_y, m_z, m_r, m_g, m_b }; }
    };

    struct Shader_P3C4 : public Shader {
        static const std::string& getName() {
            static std::string s_name("P3C4");
            return s_name;
        }
        Shader_P3C4();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct Vertex_P3N3 {
        float m_x, m_y, m_z, m_nx, m_ny, m_nz;
        bool operator<(const Vertex_P3N3& rhs) const { return memcmp(this, &rhs, sizeof(float) * 6) < 0; }
        operator VtxVector() const { return { m_x, m_y, m_z, m_nx, m_ny, m_nz }; }
    };

    struct Shader_P3N3 : public Shader {
        static const std::string& getName() {
            static std::string s_name("P3N3");
            return s_name;
        }
        Shader_P3N3();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct Vertex_P3N3T2 {
        float m_x, m_y, m_z, m_nx, m_ny, m_nz, m_u0, m_v0;
        bool operator<(const Vertex_P3N3T2& rhs) const { return memcmp(this, &rhs, sizeof(float) * 8) < 0; }
        operator VtxVector() const { return { m_x, m_y, m_z, m_nx, m_ny, m_nz, m_u0, m_v0 }; }
    };

    struct Shader_P3N3T2 : public Shader {
        static const std::string& getName() {
            static std::string s_name("P3N3T2");
            return s_name;
        }
        Shader_P3N3T2();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct Vertex_P3N3B3T2 {
        float m_x,  m_y,  m_z;
        float m_nx, m_ny, m_nz;
        float m_bx, m_by, m_bz;
        float m_u0, m_v0;
        bool operator<(const Vertex_P3N3B3T2& rhs) const { return memcmp(this, &rhs, sizeof(float) * 11) < 0; }
        operator VtxVector() const { return { m_x, m_y, m_z, m_nx, m_ny, m_nz, m_bx, m_by, m_bz, m_u0, m_v0 }; }
    };

    struct Shader_P3N3B3T2 : public Shader {
        static const std::string& getName() {
            static std::string s_name("P3N3B3T2");
            return s_name;
        }

        Shader_P3N3B3T2();
    };

}

#endif
