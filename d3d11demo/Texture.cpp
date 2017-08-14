#include "StdAfx.h"

#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

#include "D3D11Demo.h"
#include "Texture.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    TextureMap Texture::s_textureMap;

    void Texture::initD3D11Resources() {
        assert(!m_shaderResourceView && !m_samplerState);

        const char* ansi_name = m_name.c_str();
        size_t      s = strlen(ansi_name) + 1;
        wchar_t*    wide_name = new wchar_t[s];
        size_t      n = 0;
        mbstowcs_s(&n, wide_name, s, ansi_name, _TRUNCATE);

        ThrowIfFailed(
            DirectX::CreateDDSTextureFromFile(g_app.getD3D11Device(), wide_name, nullptr, &m_shaderResourceView, 0)
        );
        delete[] wide_name;

        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        m_shaderResourceView->GetDesc(&desc);
        m_hasAlpha = desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM ? true : false;

        ThrowIfFailed(
            g_app.getD3D11Device()->CreateSamplerState(&m_samplerStateDesc, &m_samplerState)
        );
    }

    void Texture::freeD3D11Resources() {
        if (m_shaderResourceView) {
            m_shaderResourceView->Release();
            m_shaderResourceView = nullptr;
        }
        if (m_samplerState) {
            m_samplerState->Release();
            m_samplerState = nullptr;
        }
    }

    void Texture::initSingleton() {
        assert(s_textureMap.empty());
    }

    void Texture::freeSingleton() {
        for (auto& p : s_textureMap) {
            delete p.second;
        }
        s_textureMap.clear();
    }

    Texture* Texture::get(const std::string& name) {
        if (s_textureMap.count(name) == 0) {
            Texture* t = new Texture(name);
        }
        return s_textureMap[name];
    }

    Texture::Texture(const std::string& name)
        : m_name(name) {

        m_samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        m_samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        m_samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        m_samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        m_samplerStateDesc.MipLODBias = 0.0f;
        m_samplerStateDesc.MaxAnisotropy = 1;
        m_samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        m_samplerStateDesc.BorderColor[0] = 0;
        m_samplerStateDesc.BorderColor[1] = 0;
        m_samplerStateDesc.BorderColor[2] = 0;
        m_samplerStateDesc.BorderColor[3] = 0;
        m_samplerStateDesc.MinLOD = 0;
        m_samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

        s_textureMap[m_name] = this;
        initD3D11Resources();
    }

    Texture::~Texture() {
        freeD3D11Resources();
    }

    void Texture::setSamplerStateDesc(D3D11_FILTER filter,
                                      D3D11_TEXTURE_ADDRESS_MODE addressU,
                                      D3D11_TEXTURE_ADDRESS_MODE addressV) {
        m_samplerStateDesc.Filter = filter;
        m_samplerStateDesc.AddressU = addressU;
        m_samplerStateDesc.AddressV = addressV;

        if (m_samplerState) {
            m_samplerState->Release();
            m_samplerState = nullptr;
            ThrowIfFailed(
                g_app.getD3D11Device()->CreateSamplerState(&m_samplerStateDesc, &m_samplerState)
            );
        }
    }

    void Texture::applyTextureToD3D11(uint32_t stage) const {
        g_app.getD3D11DeviceContext()->PSSetShaderResources(stage, 1, &m_shaderResourceView);
        g_app.getD3D11DeviceContext()->PSSetSamplers(stage, 1, &m_samplerState);
    }
}
