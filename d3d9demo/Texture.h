#pragma once
#ifndef Texture_h
#define Texture_h

namespace eve {
	class TextureMgr : public RefCountedObject {
		friend class Texture;
	public:
		static Texture* createDiffMap(const char* name);
		static Texture* createNormMap(const char* name);
		static bool exists(const char* name);
		TextureMgr();
	~	TextureMgr();
	private:
		typedef std::map<std::string, Texture*>::iterator Iterator;
		typedef std::map<std::string, Texture*> Map;
		static Map* s_map;
	};

	class Texture : public RefCountedObject, public D3DResourceUser {
		friend class TextureMgr;
	public:
		enum MapType {
			MAP_DIFF, // diffuse colour map
			MAP_NORM  // for dot3 bump map
		};
		void setTextureStage(u32 stage) const;
		void setAddrModeU(D3DTEXTUREADDRESS a) { m_addrModeU = a; }
		void setAddrModeV(D3DTEXTUREADDRESS a) { m_addrModeV = a; }
		void setMinFilter(D3DTEXTUREFILTERTYPE f) { m_minFilter = f; }
		void setMagFilter(D3DTEXTUREFILTERTYPE f) { m_magFilter = f; }
		void setMipFilter(D3DTEXTUREFILTERTYPE f) { m_mipFilter = f; }
		const char* getName() const { return m_name.c_str(); }
		bool hasAlpha() const { return m_hasAlpha; }

	private:
		D3DTEXTUREADDRESS m_addrModeU; // default is wrap
		D3DTEXTUREADDRESS m_addrModeV; // default is wrap
		D3DTEXTUREFILTERTYPE m_minFilter; // default is nearest neighbour (point sampling)
		D3DTEXTUREFILTERTYPE m_magFilter; // default is nearest neighbour (point sampling)
		D3DTEXTUREFILTERTYPE m_mipFilter; // default is none
		LPDIRECT3DTEXTURE9 m_texture;
		const std::string m_name;
		MapType m_mapType;
		bool m_hasAlpha;
		void initD3DResources();
		void freeD3DResources();
		Texture(const char* name, MapType mapType);
	~	Texture();		
	};
}

#endif