#include "StdAfx.h"
#include "D3D11Demo.h"
#include "Shader.h"

#if defined(_DEBUG)
    #include "Shaders/P3C4_PS_Debug.h"
    #include "Shaders/P3C4_VS_Debug.h"
    #include "Shaders/P3N3_PS_Debug.h"
    #include "Shaders/P3N3_VS_Debug.h"
    #include "Shaders/P3N3B3T2_PS_Debug.h"
    #include "Shaders/P3N3B3T2_VS_Debug.h"
    #include "Shaders/P3N3T2_PS_Debug.h"
    #include "Shaders/P3N3T2_VS_Debug.h"
#else
    #include "Shaders/P3C4_PS_Release.h"
    #include "Shaders/P3C4_VS_Release.h"
    #include "Shaders/P3N3_PS_Release.h"
    #include "Shaders/P3N3_VS_Release.h"
    #include "Shaders/P3N3B3T2_PS_Release.h"
    #include "Shaders/P3N3B3T2_VS_Release.h"
    #include "Shaders/P3N3T2_PS_Release.h"
    #include "Shaders/P3N3T2_VS_Release.h"
#endif

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ShaderMap Shader::s_shaderMap;

    ID3D11Buffer* Shader::s_VS_REG_B0 = nullptr;
    ID3D11Buffer* Shader::s_VS_REG_B1 = nullptr;
    ID3D11Buffer* Shader::s_PS_REG_B0 = nullptr;
    ID3D11Buffer* Shader::s_PS_REG_B1 = nullptr;
    ID3D11Buffer* Shader::s_PS_REG_B2 = nullptr;

    void Shader::initD3D11Resources() {
        ThrowIfFailed(g_app.getD3D11Device()->CreateVertexShader(m_vsBlob, m_vsSize, NULL, &m_vs));
        ThrowIfFailed(g_app.getD3D11Device()->CreatePixelShader(m_psBlob, m_psSize, NULL, &m_ps));
        ThrowIfFailed(g_app.getD3D11Device()->CreateInputLayout(m_elements, m_numElements, m_vsBlob, m_vsSize, &m_inputLayout));

        if (s_VS_REG_B0 == nullptr) {
            D3D11_BUFFER_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.Usage     = D3D11_USAGE_DEFAULT;

            desc.ByteWidth = sizeof(VS_REG_B0);
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, nullptr, &s_VS_REG_B0));

            desc.ByteWidth = sizeof(VS_REG_B1);
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, nullptr, &s_VS_REG_B1));

            desc.ByteWidth = sizeof(PS_REG_B0);
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, nullptr, &s_PS_REG_B0));

            desc.ByteWidth = sizeof(PS_REG_B1);
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, nullptr, &s_PS_REG_B1));

            desc.ByteWidth = sizeof(PS_REG_B2);
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, nullptr, &s_PS_REG_B2));
        }
    }

    void Shader::freeD3D11Resources() {
        if (m_vs) {
            m_vs->Release();
            m_vs = nullptr;
        }
        if (m_ps) {
            m_ps->Release();
            m_ps = nullptr;
        }
        if (m_inputLayout) {
            m_inputLayout->Release();
            m_inputLayout = nullptr;
        }
    }

    void Shader::initSingleton() {
        assert(s_shaderMap.empty());

    }

    void Shader::freeSingleton() {
        for (auto& p : s_shaderMap) {
            delete p.second;
        }
        s_shaderMap.clear();

        if (s_VS_REG_B0) {
            s_VS_REG_B0->Release();
            s_VS_REG_B0 = nullptr;
        }
        if (s_VS_REG_B1) {
            s_VS_REG_B1->Release();
            s_VS_REG_B1 = nullptr;
        }
        if (s_PS_REG_B0) {
            s_PS_REG_B0->Release();
            s_PS_REG_B0 = nullptr;
        }
        if (s_PS_REG_B1) {
            s_PS_REG_B1->Release();
            s_PS_REG_B1 = nullptr;
        }
        if (s_PS_REG_B2) {
            s_PS_REG_B2->Release();
            s_PS_REG_B2 = nullptr;
        }
    }

    Shader* Shader::get(const std::string& name) {
        auto i = s_shaderMap.find(name);
        if (i == s_shaderMap.end()) {
            if (name == Shader_P3C4::getName()) {
                return new Shader_P3C4;
            }
            if (name == Shader_P3N3::getName()) {
                return new Shader_P3N3;
            }
            if (name == Shader_P3N3T2::getName()) {
                return new Shader_P3N3T2;
            }
            if (name == Shader_P3N3B3T2::getName()) {
                return new Shader_P3N3B3T2;
            }
            assert(!"Unable to create shader!");
            return nullptr;
        }
        return i->second;
    }

    void Shader::setVS_REG_B0(const VS_REG_B0& reg) {
        g_app.getD3D11DeviceContext()->UpdateSubresource(s_VS_REG_B0, 0, nullptr, &reg, 0, 0);
        g_app.getD3D11DeviceContext()->VSSetConstantBuffers(0, 1, &s_VS_REG_B0);
    }

    void Shader::setVS_REG_B1(const VS_REG_B1& reg) {
        g_app.getD3D11DeviceContext()->UpdateSubresource(s_VS_REG_B1, 0, nullptr, &reg, 0, 0);
        g_app.getD3D11DeviceContext()->VSSetConstantBuffers(1, 1, &s_VS_REG_B1);
    }

    void Shader::setPS_REG_B0(const PS_REG_B0& reg) {
        g_app.getD3D11DeviceContext()->UpdateSubresource(s_PS_REG_B0, 0, nullptr, &reg, 0, 0);
        g_app.getD3D11DeviceContext()->PSSetConstantBuffers(0, 1, &s_PS_REG_B0);
    }

    void Shader::setPS_REG_B1(const PS_REG_B1& reg) {
        g_app.getD3D11DeviceContext()->UpdateSubresource(s_PS_REG_B1, 0, nullptr, &reg, 0, 0);
        g_app.getD3D11DeviceContext()->PSSetConstantBuffers(1, 1, &s_PS_REG_B1);
    }

    void Shader::setPS_REG_B2(const PS_REG_B2& reg) {
        g_app.getD3D11DeviceContext()->UpdateSubresource(s_PS_REG_B2, 0, nullptr, &reg, 0, 0);
        g_app.getD3D11DeviceContext()->PSSetConstantBuffers(2, 1, &s_PS_REG_B2);
    }

    Shader::Shader(const std::string& name, const D3D11_INPUT_ELEMENT_DESC* elements, UINT numElements,
                   const BYTE* vsBlob, size_t vsSize,
                   const BYTE* psBlob, size_t psSize)
        : m_name(name)
        , m_elements(elements)
        , m_numElements(numElements)
        , m_vsBlob(vsBlob)
        , m_vsSize(vsSize)
        , m_psBlob(psBlob)
        , m_psSize(psSize) {

        s_shaderMap[name] = this;
        initD3D11Resources();
    }

    Shader::~Shader() {
        freeD3D11Resources();
    }

    void Shader::applyShaderToD3D11() const {
        g_app.getD3D11DeviceContext()->IASetInputLayout(m_inputLayout);
        g_app.getD3D11DeviceContext()->VSSetShader(m_vs, nullptr, 0);
        g_app.getD3D11DeviceContext()->PSSetShader(m_ps, nullptr, 0);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    const D3D11_INPUT_ELEMENT_DESC Layout_P3C4[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Shader_P3C4::Shader_P3C4()
        : Shader(Shader_P3C4::getName(),
                 Layout_P3C4, ARRAYSIZE(Layout_P3C4),
                 g_P3C4_VS, sizeof(g_P3C4_VS),
                 g_P3C4_PS, sizeof(g_P3C4_PS)) {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    const D3D11_INPUT_ELEMENT_DESC Layout_P3N3[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Shader_P3N3::Shader_P3N3() :
        Shader(Shader_P3N3::getName(),
               Layout_P3N3, ARRAYSIZE(Layout_P3N3),
               g_P3N3_VS, sizeof(g_P3N3_VS),
               g_P3N3_PS, sizeof(g_P3N3_PS)) {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    const D3D11_INPUT_ELEMENT_DESC Layout_P3N3B3T2[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Shader_P3N3B3T2::Shader_P3N3B3T2()
        : Shader(Shader_P3N3B3T2::getName(),
                 Layout_P3N3B3T2, ARRAYSIZE(Layout_P3N3B3T2),
                 g_P3N3B3T2_VS, sizeof(g_P3N3B3T2_VS),
                 g_P3N3B3T2_PS, sizeof(g_P3N3B3T2_PS)) {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    const D3D11_INPUT_ELEMENT_DESC Layout_P3N3T2[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Shader_P3N3T2::Shader_P3N3T2()
        : Shader(Shader_P3N3T2::getName(),
                 Layout_P3N3T2, ARRAYSIZE(Layout_P3N3T2),
                 g_P3N3T2_VS, sizeof(g_P3N3T2_VS),
                 g_P3N3T2_PS, sizeof(g_P3N3T2_PS)) {
    }

}
