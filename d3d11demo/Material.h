#pragma once
#ifndef Material_h
#define Material_h

#include "Color.h"

namespace d3d11demo {

    class Shader;
    class Texture;

    using MaterialMap = std::map<std::string, class Material*>;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Material {
        static const uint32_t MAX_TEXTURE_STAGES = 8;
        static MaterialMap s_materialMap;

        const   std::string m_name;
        Color   m_diff        = 0.5f;
        Color   m_spec        = 0.5f;
        float   m_shininess   = 1.0f;
        bool    m_translucent = false;

        Texture* m_textures[MAX_TEXTURE_STAGES] = { nullptr };
        Shader * m_shader = nullptr;

        explicit Material(const std::string& name);

    public:
        static void initSingleton();
        static void freeSingleton();
        static Material* get(const std::string& name);

        void applyMaterialToD3D11() const;

        void     setTexture(uint32_t stage, Texture* texture);
        Texture* getTexture(uint32_t stage);

        void    setShader(class Shader* shader);
        Shader* getShader();

        void         setDiff(const Color& c) { m_diff = c; }
        const Color& getDiff() const { return m_diff; }

        void         setSpec(const Color& c) { m_spec = c; }
        const Color& getSpec() const { return m_spec; }

        void         setShininess(float shininess) { m_shininess = shininess; }
        const float& getShininess() const { return m_shininess; }

        bool isTranslucent() const { return m_translucent; }
    };

}

#endif
