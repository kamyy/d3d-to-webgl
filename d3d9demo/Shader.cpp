#include "BasicTypes.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

namespace eve {
	ShaderMgr::Map* ShaderMgr::s_map = 0;
	//
	// Shader Manager
	//
	Shader* ShaderMgr::create(const char* name) {
		Iterator i = s_map->find(name);
		if (i == s_map->end()) {
			if (strcmp(name, P3C4::getName()) == 0)
				return new P3C4;
			if (strcmp(name, P3N3::getName()) == 0)
				return new P3N3;
			if (strcmp(name, P3N3T2::getName()) == 0)
				return new P3N3T2;
			if (strcmp(name, P3N3B3T2::getName()) == 0)
				return new P3N3B3T2;

			EVE_ASSERT(!"Unable to create shader!");
			return 0;
		}
		i->second->addRef();
		return i->second;
	}
	bool ShaderMgr::exists(const char* name) {
		if (s_map->find(name) == s_map->end()) {
			return false;
		}
		return true;
	}
	ShaderMgr::ShaderMgr() {
		EVE_ASSERT(s_map == 0);
		s_map = new Map;
	}
	ShaderMgr::~ShaderMgr() {
		Shader::setRefFrame(0);
		Shader::setAmbientLS(0);
		Shader::setOmniLS(0);
		Shader::setCamera(0);
		EVE_ASSERT(s_map->empty());
		delete s_map;
		s_map = 0;
	}

	RefFrame* Shader::s_frame = 0;
	AmbientLS* Shader::s_ambientLS = 0;
	OmniLS* Shader::s_omniLS = 0;
	Camera* Shader::s_camera = 0;
	//
	// Shader
	//
	Shader::Shader(const char* name, VtxMgr::DeclIdentifier di, const DWORD* vs, const DWORD* ps) 
	: m_name(name), m_di(di), m_vsBin(vs), m_psBin(ps), m_vs(0), m_ps(0) {
		(*ShaderMgr::s_map)[m_name] = this;
		initD3DResources();
	}
	Shader::~Shader() {
		freeD3DResources();
		ShaderMgr::s_map->erase(m_name);
	}

	void Shader::setShaderCode() const {
		EVE_VALIDATE_HR(getDevice()->SetVertexDeclaration(VtxMgr::getVtxDeclIdent(m_di)));
		if (m_vs) 
			EVE_VALIDATE_HR(getDevice()->SetVertexShader(m_vs));
		if (m_ps) 
			EVE_VALIDATE_HR(getDevice()->SetPixelShader(m_ps));
	}

	void Shader::initD3DResources() {
		if (m_vsBin && m_vs == 0)
			EVE_VALIDATE_HR(getDevice()->CreateVertexShader(m_vsBin, &m_vs));
		if (m_vsBin && m_ps == 0)
			EVE_VALIDATE_HR(getDevice()->CreatePixelShader(m_psBin, &m_ps));
	}
	void Shader::freeD3DResources() {
		if (m_vs)
			m_vs->Release();
		if (m_ps)
			m_ps->Release();		
		m_ps = 0;
		m_vs = 0;
	}

	void Shader::setVSConstReg(i32 a, i32 b, const f32* v) { 
		EVE_VALIDATE_HR(getDevice()->SetVertexShaderConstantF(a, v, b)); 
	}
	void Shader::setPSConstReg(i32 a, i32 b, const f32* v) { 
		EVE_VALIDATE_HR(getDevice()->SetPixelShaderConstantF(a, v, b));
	}
	void Shader::setRefFrame(RefFrame* frm) {
		if (s_frame) {
			s_frame->rmvRef();
			s_frame = 0;
		}
		if (frm) {
			s_frame = frm;
			s_frame->addRef();
		}
	}
	void Shader::setAmbientLS(AmbientLS* ls) {
		if (s_ambientLS) {
			s_ambientLS->rmvRef();
			s_ambientLS = 0;
		}
		if (ls) {
			s_ambientLS = ls;
			s_ambientLS->addRef();
		}
	}
	void Shader::setOmniLS(OmniLS* ls) {
		if (s_omniLS) {
			s_omniLS->rmvRef();
			s_omniLS = 0;
		}
		if (ls) {
			s_omniLS = ls;
			s_omniLS->addRef();
		}
	}
	void Shader::setCamera(Camera* cam) {
		if (s_camera) {
			s_camera->rmvRef();
			s_camera = 0;
		}
		if (cam) {
			s_camera = cam;
			s_camera->addRef();
		}
	}
}
