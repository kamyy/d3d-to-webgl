#include "BasicTypes.h"
#include "Shader.h"
#include "ASE.h"

namespace eve {
	ASE::Mtl::Mtl() {
		m_mtl = 0;
		m_uOffset = 0.f;
		m_vOffset = 0.f;
		m_uTiling = 1.f;
		m_vTiling = 1.f;
	}
	ASE::Mtl::~Mtl() {
		ASE::MtlArray::iterator i;
		for (i = m_mtls.begin(); i < m_mtls.end(); ++i) {
			delete (*i);
		}
		if (m_mtl) {
			m_mtl->rmvRef();
		}
	}
	bool ASE::Mtl::read(istream& is, const string& parentName) {
		char *p, *q, buf[512];

		Class clss = CLASS_STANDARD;
		Color ambi;
		Color diff;
		Color spec;
		f32 shineExponent = 0.0f;
		f32 shineStrength = 0.0f;
		DiffMap diffMap;
		NormMap bumpMap;
		string mtlName;

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;

			if (!strcmp(p, "*MATERIAL_NAME")) {
				mtlName = strtok(strchr(q, '"'), "\"");
				mtlName = mtlName + parentName;
			} else if (!strcmp(p, "*MATERIAL_CLASS")) {
				if (strstr(q, "Multi/Sub-Object")) {
					clss = CLASS_MULTISUB;
				} else if (strstr(q, "Standard" )) {
					clss = CLASS_STANDARD;
				} else {
					EVE_ASSERT(!"Bad mtl class");
				}
			} else if (!strcmp(p, "*MATERIAL_AMBIENT" )) {
				if (sscanf(q, "%f %f %f", &ambi.m_r, &ambi.m_g, &ambi.m_b) != 3)
					return false;
			} else if (!strcmp(p, "*MATERIAL_DIFFUSE" )) {
				if (sscanf(q, "%f %f %f", &diff.m_r, &diff.m_g, &diff.m_b) != 3)
					return false;
			} else if (!strcmp(p, "*MATERIAL_SPECULAR")) {
				if (sscanf(q, "%f %f %f", &spec.m_r, &spec.m_g, &spec.m_b) != 3)
					return false;
			} else if (!strcmp(p, "*MATERIAL_SHINESTRENGTH"	)) {
				if (sscanf(q, "%f", &shineStrength) != 1)
					return false;
			} else if (!strcmp(p, "*MATERIAL_SHINE"			)) {
				if (sscanf(q, "%f", &shineExponent) != 1)
					return false;
			} else if (!strcmp(p, "*SUBMATERIAL")) {
				Mtl* mtl = new Mtl;
				if (!mtl->read(is, mtlName)) {
					delete mtl;
					return false;
				}
				m_mtls.push_back(mtl);
			} else if (!strcmp(p, "*MAP_DIFFUSE")) {
				if (!diffMap.read(is, this)) {
					return false;
				}
			} else if (!strcmp(p, "*MAP_BUMP")) {
				if (!bumpMap.read(is)) {
					return false;
				}
			} else if (strchr(p, '}')) {
				break;
			} else if (strchr(q, '{')) {
				if (!skipBlock(is))
					return false;
			}
			is.getline(buf, sizeof(buf));
		}

		bool ok = !is.fail();
		if (ok) {
			EVE_ASSERT(!MaterialMgr::exists(mtlName.c_str()));
			m_mtl = MaterialMgr::create(mtlName.c_str());
			m_mtl->setDiff(Color(diff.m_r, diff.m_g, diff.m_b));
			m_mtl->setSpec(Color(	spec.m_r * shineStrength, 
									spec.m_g * shineStrength, 
									spec.m_b * shineStrength));
			m_mtl->setShininess(shineExponent * 100.0f);

			Shader* shaderA = ShaderMgr::create("P3N3B3T2");
			Shader* shaderB = ShaderMgr::create("P3N3T2");
			Shader* shaderC = ShaderMgr::create("P3N3");
			m_mtl->setShader(shaderC);

			if (diffMap.m_texture) {
				m_mtl->setTexture(0, diffMap.m_texture);
				m_mtl->setShader(shaderB);
			}
			if (bumpMap.m_texture) {
				m_mtl->setTexture(1, bumpMap.m_texture);
				m_mtl->setShader(shaderA);
			}
			shaderA->rmvRef();
			shaderB->rmvRef();
			shaderC->rmvRef();
		}
		return ok;
	}

	//
	// ASE Material Diffuse Map
	//
	ASE::Mtl::DiffMap::DiffMap() {
		m_texture = 0;
	}
	ASE::Mtl::DiffMap::~DiffMap() {
		if (m_texture) {
			m_texture->rmvRef();
		}
	}
	bool ASE::Mtl::DiffMap::read(istream& is, Mtl* mtl) {
		char *p, *q, buf[512];
		string file;

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;
			
			if (!strcmp(p, "*BITMAP")) {
				file = strtok(strchr(q, '"'), "\"");
			} else if (!strcmp(p, "*MAP_CLASS")) {
				EVE_ASSERT(strstr(q, "Bitmap")); 
			} else if (!strcmp(p, "*UVW_U_OFFSET")) {
				if (sscanf(q, "%f", &mtl->m_uOffset) != 1)
					return false;
			} else if (!strcmp(p, "*UVW_V_OFFSET")) {
				if (sscanf(q, "%f", &mtl->m_vOffset) != 1)
					return false;
			} else if (!strcmp(p, "*UVW_U_TILING")) {
				if (sscanf(q, "%f", &mtl->m_uTiling) != 1)
					return false;
			} else if (!strcmp(p, "*UVW_V_TILING")) {
				if (sscanf(q, "%f", &mtl->m_vTiling) != 1)
					return false;
			} else if (strchr(p, '}')) {
				break;
			} else if (strchr(q, '{')) {
				if (!skipBlock(is))
					return false;
			}
			is.getline(buf, sizeof(buf));
		}
		bool ok = !is.fail();
		if (ok) {
			if ((m_texture = TextureMgr::createDiffMap(file.c_str())) != 0) {
				m_texture->setMinFilter(D3DTEXF_ANISOTROPIC);
				m_texture->setMagFilter(D3DTEXF_ANISOTROPIC);
				m_texture->setAddrModeU(D3DTADDRESS_WRAP);
				m_texture->setAddrModeV(D3DTADDRESS_WRAP);
				m_texture->setMipFilter(D3DTEXF_LINEAR);
			}
		}
		return ok;
	}

	//
	// ASE Material Bump Map
	//
	ASE::Mtl::NormMap::NormMap() {
		m_texture = 0;
	}
	ASE::Mtl::NormMap::~NormMap() {
		if (m_texture) {
			m_texture->rmvRef();
		}
	}
	bool ASE::Mtl::NormMap::read(istream& is) {
		char *p, *q, buf[512];
		string file;

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;
			
			if (!strcmp(p, "*BITMAP")) {
				file = strtok(strchr(q, '"'), "\"");
			} else if (!strcmp(p, "*MAP_CLASS")) {
				EVE_ASSERT(strstr(q, "Bitmap")); 
			} else if (strchr(p, '}')) {
				break;
			} else if (strchr(q, '{')) {
				if (!skipBlock(is))
					return false;
			}
			is.getline(buf, sizeof(buf));
		}
		bool ok = !is.fail();
		if (ok) {
			if ((m_texture = TextureMgr::createNormMap(file.c_str())) != 0) {
				m_texture->setMinFilter(D3DTEXF_LINEAR);
				m_texture->setMagFilter(D3DTEXF_LINEAR);
				m_texture->setAddrModeU(D3DTADDRESS_WRAP);
				m_texture->setAddrModeV(D3DTADDRESS_WRAP);
				m_texture->setMipFilter(D3DTEXF_LINEAR);
			}
		}
		return ok;
	}
}