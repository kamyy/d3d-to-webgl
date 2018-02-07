#pragma once
#ifndef Material_h
#define Material_h

#include "Texture.h"
#include "Vertex.h"
#include "Color.h"

namespace eve {
	class Shader;
	//
	// Material manager
	//
	class MaterialMgr : public RefCountedObject {
		friend class Material;
	public:
		static Material* create(const char* name);
		static bool exists(const char* name);
		MaterialMgr();
	~	MaterialMgr();
	private:
		typedef std::map<std::string, Material*>::iterator Iterator;
		typedef std::map<std::string, Material*> Map;
		static Map* s_map;
	};

	//
	// Material
	//
	class Material : public RefCountedObject {
		friend class MaterialMgr;
	public:
		enum {
			MAX_TEXTURE_STAGES = 8
		};
		bool isTranslucent() const { return m_translucent; }

		const Color& getDiff() const { return m_diff; }
		const Color& getSpec() const { return m_spec; }
		const f32& getShininess() const { return m_shininess; }
		void setDiff(const Color& c) { m_diff = c; }
		void setSpec(const Color& c) { m_spec = c; }
		void setShininess(f32 p) { m_shininess = p; }

		void setTexture(u32 stage, Texture* texture);
		Texture* getTexture(u32 stage);

		void setShader(Shader* shader);
		Shader* getShader();

		void setDevice() const;

	private:	
		Material(const char* name);
	~	Material();

		const	std::string m_name;
		Color	m_diff;
		Color	m_spec;
		f32		m_shininess;
		bool	m_translucent;
		Texture*	m_textures[MAX_TEXTURE_STAGES];
		Shader*		m_shader;
	};
}

#endif