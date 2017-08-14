#pragma once
#ifndef Shader_h
#define Shader_h

#include "Material.h"
#include "Vertex.h"
#include "Camera.h"
#include "Light.h"

namespace eve {
	class ShaderMgr : public RefCountedObject {
		friend class Shader;
	public:
		static Shader* create(const char* name);
		static bool exists(const char* name);
		ShaderMgr();
	~	ShaderMgr();
	private:
		typedef std::map<std::string, Shader*>::iterator Iterator;
		typedef std::map<std::string, Shader*> Map;
		static Map* s_map;
	};

	class Shader : public RefCountedObject, public D3DResourceUser {
	public:
		Shader(const char* name, VtxMgr::DeclIdentifier di, 
			const DWORD* vsBin, const DWORD* psBin);
		virtual ~Shader();

		static void setVSConstReg(i32 registerID, i32 registerTotal, const f32* v);
		static void setPSConstReg(i32 registerID, i32 registerTotal, const f32* v);

		VtxMgr::DeclIdentifier getDeclIdentifier() const {
			return m_di;
		}

		static RefFrame* getRefFrame() { return s_frame; }
		static AmbientLS* getAmbientLS() { return s_ambientLS; }
		static OmniLS* getOmniLS() { return s_omniLS; }
		static Camera* getCamera() { return s_camera; }

		static void setRefFrame(RefFrame* frame = 0);
		static void setAmbientLS(AmbientLS* ambientLS = 0);
		static void setOmniLS(OmniLS* omniLS = 0);
		static void setCamera(Camera* camera = 0);

		virtual void setVSConstData(const Material* m) const = 0;
		virtual void setPSConstData(const Material* m) const = 0;
		void setShaderCode() const;
	private:
		void initD3DResources();
		void freeD3DResources();
		IDirect3DVertexShader9* m_vs;
		IDirect3DPixelShader9*	m_ps;
		VtxMgr::DeclIdentifier m_di;
		const DWORD* m_vsBin;
		const DWORD* m_psBin;
		const char* m_name;
	protected:
		static RefFrame* s_frame;
		static AmbientLS* s_ambientLS;
		static OmniLS* s_omniLS;
		static Camera* s_camera;
	};

	class P3C4 : public Shader {
	public:
		virtual void setVSConstData(const Material* mat) const;
		virtual void setPSConstData(const Material* mat) const;
		static const char* getName() { return "P3C4"; }
	private:
		friend class ShaderMgr;
		P3C4();
	};

	class P3N3 : public Shader {
	public:
		virtual void setVSConstData(const Material* mat) const;
		virtual void setPSConstData(const Material* mat) const;
		static const char* getName() { return "P3N3"; }
	private:
		friend class ShaderMgr;
		P3N3();
	};

	class P3N3T2 : public Shader {
	public:
		virtual void setVSConstData(const Material* mat) const;
		virtual void setPSConstData(const Material* mat) const;
		static const char* getName() { return "P3N3T2"; }
	private:
		friend class ShaderMgr;
		P3N3T2();
	};

	class P3N3B3T2 : public Shader {
	public:
		virtual void setVSConstData(const Material* mat) const;
		virtual void setPSConstData(const Material* mat) const;
		static const char* getName() { return "P3N3B3T2"; }
	private:
		friend class ShaderMgr;
		P3N3B3T2();
	};
}

#endif
