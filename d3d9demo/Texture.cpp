#include "BasicTypes.h"
#include "Material.h"
#include "Texture.h"

namespace eve {
	TextureMgr::Map* TextureMgr::s_map = 0;

	//
	// Texture Manager
	//
	Texture* TextureMgr::createDiffMap(const char* name) {
		Iterator i = s_map->find(name);
		Texture* t;
		if (i == s_map->end()) {
			t = new Texture(name, Texture::MAP_DIFF);
			if (t->m_texture) {
				return t;
			}
			t->rmvRef(); // failed to create d3d texture
			return 0;
		}
		i->second->addRef();
		return i->second;
	}
	Texture* TextureMgr::createNormMap(const char* name) {
		Iterator i = s_map->find(name);
		Texture* t;
		if (i == s_map->end()) {
			t = new Texture(name, Texture::MAP_NORM);
			if (t->m_texture) {
				return t;
			}
			t->rmvRef(); // failed to create d3d texture
			return 0;
		}
		i->second->addRef();
		return i->second;
	}
	bool TextureMgr::exists(const char* name) {
		if (s_map->find(name) == s_map->end()) {
			return false;
		}
		return true;
	}

	TextureMgr::TextureMgr() {
		EVE_ASSERT(s_map == 0);
		s_map = new Map;
	}
	TextureMgr::~TextureMgr() {
		EVE_ASSERT(s_map->empty());
		delete s_map;
		s_map = 0;
	}

	//
	// Texture
	//
	void Texture::setTextureStage(u32 stage) const {
		EVE_ASSERT(stage < Material::MAX_TEXTURE_STAGES);
		EVE_VALIDATE_HR(getDevice()->SetTexture(stage, m_texture));

		EVE_VALIDATE_HR(getDevice()->SetSamplerState(stage, D3DSAMP_ADDRESSU, m_addrModeU));
		EVE_VALIDATE_HR(getDevice()->SetSamplerState(stage, D3DSAMP_ADDRESSV, m_addrModeV));
		EVE_VALIDATE_HR(getDevice()->SetSamplerState(stage, D3DSAMP_MINFILTER, m_minFilter));
		EVE_VALIDATE_HR(getDevice()->SetSamplerState(stage, D3DSAMP_MAGFILTER, m_magFilter));
		EVE_VALIDATE_HR(getDevice()->SetSamplerState(stage, D3DSAMP_MIPFILTER, m_mipFilter));
	}
	void Texture::initD3DResources() {
		if (m_texture == 0) {
			D3DXCreateTextureFromFile(getDevice(), m_name.c_str(), &m_texture);

			if (m_texture != 0) {
				D3DSURFACE_DESC surface;
				m_texture->GetLevelDesc(0, &surface);
				switch (surface.Format) {
				case D3DFMT_DXT2:
				case D3DFMT_DXT3:
				case D3DFMT_DXT4:
				case D3DFMT_DXT5:
				case D3DFMT_A1R5G5B5:
				case D3DFMT_A8R8G8B8:
				case D3DFMT_A8B8G8R8:
					m_hasAlpha = true;
					break;
				default:
					m_hasAlpha = false;
					break;
				}
			}
		}
	}
	void Texture::freeD3DResources() {
		if (m_texture) {
			m_texture->Release();
			m_texture = 0;
		}
	}

	Texture::Texture(const char* name, MapType mapType) 
			: m_texture(0), m_name(name), m_mapType(mapType) {
		(*TextureMgr::s_map)[m_name] = this;
		m_addrModeU	= D3DTADDRESS_WRAP;
		m_addrModeV	= D3DTADDRESS_WRAP;
		m_minFilter = D3DTEXF_POINT;
		m_magFilter = D3DTEXF_POINT;
		m_mipFilter = D3DTEXF_NONE;
		initD3DResources();
	}
	Texture::~Texture() {
		TextureMgr::s_map->erase(m_name);
		freeD3DResources();
	}
}
