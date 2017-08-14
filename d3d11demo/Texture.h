#pragma once
#ifndef Texture_h
#define Texture_h

#include "D3D11Resource.h"

namespace d3d11demo {

    using TextureMap = std::map<std::string, class Texture*>;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Texture : public D3D11Resource {
        static TextureMap s_textureMap;

        const std::string m_name;

        ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
        ID3D11SamplerState* m_samplerState = nullptr;
        D3D11_SAMPLER_DESC  m_samplerStateDesc;
        bool m_hasAlpha = false;

        virtual void initD3D11Resources() override;
        virtual void freeD3D11Resources() override;

    public:
        static void initSingleton();
        static void freeSingleton();
        static Texture* get(const std::string& name);

        Texture(const std::string& name);
       ~Texture();

        void setSamplerStateDesc(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV);
        void applyTextureToD3D11(uint32_t stage) const;

        bool hasAlpha() const {
            return m_hasAlpha;
        }
    };

}

#endif
