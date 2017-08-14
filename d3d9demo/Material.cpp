#include "BasicTypes.h"
#include "Material.h"
#include "Shader.h"

namespace eve {
	MaterialMgr::Map* MaterialMgr::s_map;
	//
	// Material Manager
	//
	Material* MaterialMgr::create(const char* name) {
		Iterator i = s_map->find(name);
		if (i == s_map->end())
			return new Material(name);
		i->second->addRef();
		return i->second;
	}
	bool MaterialMgr::exists(const char* name) {
		if (s_map->find(name) == s_map->end()) {
			return false;
		}
		return true;
	}

	MaterialMgr::MaterialMgr() {
		EVE_ASSERT(s_map == 0);
		s_map = new Map;
	}
	MaterialMgr::~MaterialMgr() {
		EVE_ASSERT(s_map->empty());
		delete s_map;
		s_map = 0;
	}

	//
	// Material
	//
	Material::Material(const char* name) : m_name(name) {
		(*MaterialMgr::s_map)[m_name] = this;

		m_diff.m_r = 0.5f;
		m_diff.m_g = 0.5f;
		m_diff.m_b = 0.5f;
		m_shininess = 1.0f;
		m_translucent = false;

		memset(m_textures, 0, sizeof(m_textures));
		m_shader = 0;
	}
	Material::~Material() {
		for (u32 i = 0; i < MAX_TEXTURE_STAGES; ++i) {
			setTexture(i, 0);
		}
		MaterialMgr::s_map->erase(m_name);
		setShader(0);
	}

	void Material::setTexture(u32 stage, Texture* texture) {
		EVE_ASSERT(stage < MAX_TEXTURE_STAGES);
		if (m_textures[stage]) {
			m_textures[stage]->rmvRef();
		}
		if (texture) {
			texture->addRef();
		}
		m_textures[stage] = texture;

		m_translucent = false;
		for (u32 i = 0; i < MAX_TEXTURE_STAGES; ++i) {
			if (m_textures[i] && m_textures[i]->hasAlpha()) {
				m_translucent = true;
			}
		}
	}
	Texture* Material::getTexture(u32 stage) {
		EVE_ASSERT(stage < MAX_TEXTURE_STAGES);
		return m_textures[stage];
	}
	void Material::setShader(Shader* shader) {
		if (m_shader) {
			m_shader->rmvRef();
		}
		if (shader) {
			shader->addRef();
		}
		m_shader = shader;
	}
	Shader* Material::getShader() {
		return m_shader;
	}

	void Material::setDevice() const {
		for (u32 i = 0; i < MAX_TEXTURE_STAGES; ++i) {
			if (m_textures[i]) {
				m_textures[i]->setTextureStage(i);
			}
		}
		EVE_ASSERT(m_shader != 0);
		m_shader->setShaderCode();
		m_shader->setVSConstData(this);
		m_shader->setPSConstData(this);
	}
}
