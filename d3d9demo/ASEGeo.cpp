#include "BasicTypes.h"
#include "RefFrame.h"
#include "Shader.h"
#include "ASE.h"

namespace eve {
	bool ASE::Geo::read(istream& is) {
		char *p, *q, buf[512];

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;
			if (!strcmp(p, "*NODE_NAME")) {
				m_myName = strtok(strchr(q, '"'), "\"");
			} else if (!strcmp(p, "*NODE_PARENT" )) {
				m_parent = strtok(strchr(q, '"'), "\"");
			} else if (!strcmp(p, "*MATERIAL_REF")) {
				if (sscanf(q, "%d", &m_mtlID) < 1)
					return false;
			} else if (!strcmp(p, "*NODE_TM")) {
				if (!readNode(is))
					return false;
			} else if (!strcmp(p, "*MESH"	)) {
				if (!readMesh(is))
					return false;
			} else if (strchr(p, '}')) {
				break;
			} else if (strchr(q, '{')) {
				if (!skipBlock(is))
					return false;
			}
			is.getline(buf, sizeof(buf));
		}
		return !is.fail();
	}
	bool ASE::Geo::readNode(istream& is) {
		char *p, *q, buf[512];

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;
			if (!strcmp(p, "*TM_ROW0")) {
				if (sscanf(q, "%f %f %f", &m_mtx.m_11, &m_mtx.m_12, &m_mtx.m_13) != 3)
					return false;
			} else if (!strcmp(p, "*TM_ROW1")) {
				if (sscanf(q, "%f %f %f", &m_mtx.m_21, &m_mtx.m_22, &m_mtx.m_23) != 3)
					return false;
			} else if (!strcmp(p, "*TM_ROW2")) {
				if (sscanf(q, "%f %f %f", &m_mtx.m_31, &m_mtx.m_32, &m_mtx.m_33) != 3)
					return false;
			} else if (!strcmp(p, "*TM_ROW3")) {
				if (sscanf(q, "%f %f %f", &m_mtx.m_41, &m_mtx.m_42, &m_mtx.m_43) != 3)
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
		if (ok) { // remove any scaling in the matrix
			((Vector1x4*)&m_mtx.m_11)->normalize();
			((Vector1x4*)&m_mtx.m_21)->normalize();
			((Vector1x4*)&m_mtx.m_31)->normalize();
		}
		return ok;
	}
	bool ASE::Geo::readMesh(istream& is) {
		char *p, *q, buf[512];

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;
			if (!strcmp(p, "*MESH_VERTEX_LIST")) {
				if (!readMeshVtx(is)) // are in world space
					return false;
			} else if (!strcmp(p, "*MESH_FACE_LIST")) {
				if (!readMeshTri(is))
					return false;
			} else if (!strcmp(p, "*MESH_TVERTLIST")) {
				if (!readMeshVtxTUV(is))
					return false;
			} else if (!strcmp(p, "*MESH_TFACELIST")) {
				if (!readMeshTriTUV(is))
					return false;
			} else if (!strcmp(p, "*MESH_NORMALS")) {
				if (!readMeshNrm(is)) // are in model space
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
		if (ok) { // transform vertex positions from world space into local space
			Matrix4x4 inv = !m_mtx;
			for (u32 i = 0; i < m_vtxs.size(); ++i) {
				Vector1x4 pos(	m_vtxs[i].m_x,
								m_vtxs[i].m_y,
								m_vtxs[i].m_z, 1.f);
				pos *= inv;
				m_vtxs[i].m_x = pos.m_x;
				m_vtxs[i].m_y = pos.m_y;
				m_vtxs[i].m_z = pos.m_z;
			}
		}
		return ok;
	}
	bool ASE::Geo::readMeshVtx(istream& is) {
		Vtx v; char buf[512];

		is.getline(buf, sizeof(buf));
		while (!is.fail() && !strchr(buf, '}')) {
			if (sscanf(buf, "%*s %*d %f %f %f", &v.m_x, &v.m_y, &v.m_z) != 3) {
				return false;
			}
			is.getline(buf, sizeof(buf));
			m_vtxs.push_back(v);
		}
		return !is.fail();
	}
	bool ASE::Geo::readMeshTri(istream& is) {
		char* p, buf[512];
		u32 idx1;
		u32 idx2;
		u32 idx3;
		Tri t;

		is.getline(buf, sizeof(buf));
		while (!is.fail() && !strchr(buf, '}')) {
			if (sscanf(	buf, "%*s %*d %*s %*s%d %*s%d %*s%d", &idx1, &idx2, &idx3) != 3) {
				return false;
			}
			p = strstr(buf, "*MESH_MTLID");
			if (sscanf(p, "%*s %d", &t.m_mtlID) != 1) {
				return false;
			}
			t.m_vtx1 = &m_vtxs.at(idx1);
			t.m_vtx2 = &m_vtxs.at(idx2);
			t.m_vtx3 = &m_vtxs.at(idx3);
			is.getline(buf, sizeof(buf));
			m_tris.push_back(t);
		}
		return !is.fail();
	}
	bool ASE::Geo::readMeshVtxTUV(istream& is) {
		Tuv t; char buf[512];

		is.getline(buf, sizeof(buf));
		while (!is.fail() && !strchr(buf, '}')) {
			if (sscanf(buf, "%*s %*d %f %f %f", &t.m_u, &t.m_v, &t.m_w) != 3) {
				return false;
			}
			t.m_v = 1.f - t.m_v; // invert to flip texture about v
			is.getline(buf, sizeof(buf));
			m_tuvs.push_back(t);
		}
		return !is.fail();
	}
	bool ASE::Geo::readMeshTriTUV(istream& is) {
		u32 n, a, b, c; 
		char buf[512];

		is.getline(buf, sizeof(buf));
		while (!is.fail() && !strchr(buf, '}')) {
			if (sscanf(buf, "%*s %d %d %d %d", &n, &a, &b, &c) != 4) {
				return false;
			}
			Tri* tri = &m_tris.at(n);
			tri->m_tuv1 = &m_tuvs.at(a);
			tri->m_tuv2 = &m_tuvs.at(b);
			tri->m_tuv3 = &m_tuvs.at(c);
			is.getline(buf, sizeof(buf));
		}
		return !is.fail();
	}
	bool ASE::Geo::readMeshNrm(istream& is) {
		char buf[512];
		f32 x, y, z;
		u32 n;

		is.getline(buf, sizeof(buf));
		while (!is.fail() && !strchr(buf, '}')) {
			if (sscanf(buf, "%*s %d %f %f %f", &n, &x, &y, &z) != 4) {
				return false;
			}

			// note: normals are in model space
			Tri* tri = &m_tris.at(n);
			tri->m_nrmF.m_x = x;
			tri->m_nrmF.m_y = y;
			tri->m_nrmF.m_z = z;
			tri->m_nrmF.m_w = 1.f;
			Vector1x4* nrm1 = &tri->m_nrm1;
			Vector1x4* nrm2 = &tri->m_nrm2;
			Vector1x4* nrm3 = &tri->m_nrm3;
			nrm1->m_w = 1.f;
			nrm2->m_w = 1.f;
			nrm3->m_w = 1.f;

			is.getline(buf, sizeof(buf));
			if (is.fail() || sscanf(buf, "%*s %*d %f %f %f", &nrm1->m_x, &nrm1->m_y, &nrm1->m_z) != 3)
				return false;
			is.getline(buf, sizeof(buf));
			if (is.fail() || sscanf(buf, "%*s %*d %f %f %f", &nrm2->m_x, &nrm2->m_y, &nrm2->m_z) != 3)
				return false;
			is.getline(buf, sizeof(buf));
			if (is.fail() || sscanf(buf, "%*s %*d %f %f %f", &nrm3->m_x, &nrm3->m_y, &nrm3->m_z) != 3)
				return false;

			is.getline(buf, sizeof(buf));
		}
		return !is.fail();	
	}
	ASE::Geo::Geo() : m_mtx(Matrix4x4::IDENTITY) {
		m_mtlID = UINT_MAX;	// geometry has no material
	}
	ASE::Geo::~Geo() {
	}

	void ASE::Geo::calcBinormal(Vtx_P3N3B3T2& vtx1, const Vtx_P3N3B3T2& vtx2, const Vtx_P3N3B3T2& vtx3) const {
		Vector1x4 v12(vtx2.m_x - vtx1.m_x, vtx2.m_y - vtx1.m_y, vtx2.m_z - vtx1.m_z, 1.f); // edge v1 to v2
		Vector1x4 v13(vtx3.m_x - vtx1.m_x, vtx3.m_y - vtx1.m_y, vtx3.m_z - vtx1.m_z, 1.f); // edge v1 to v3
		Vector1x4 n(vtx1.m_nx, vtx1.m_ny, vtx2.m_nz, 1.f); // v1 normal
		Vector1x4 proj12 = v12 - ((n * v12) * n); // project v1 to v2 onto n and subtract from v1 to v2
		Vector1x4 proj13 = v13 - ((n * v13) * n); // project v1 to v3 onto n and subtract from v1 to v3

		f32 du12 = vtx2.m_u0 - vtx1.m_u0;
		f32 du13 = vtx3.m_u0 - vtx1.m_u0;
		f32 dv12 = vtx2.m_v0 - vtx1.m_v0;
		f32 dv13 = vtx3.m_v0 - vtx1.m_v0;

		if ((du13 * dv12) > (du12 * dv13)) {
			du13 = -du13;
			du12 = -du12;
		}

		Vector1x4 binormal =  proj12 * du13 - proj13 * du12;
		binormal.normalize(); // travels in dv direction
		vtx1.m_bx = binormal.m_x;
		vtx1.m_by = binormal.m_y;
		vtx1.m_bz = binormal.m_z;
	}

	template<class T> struct Less {
		bool operator()(const T* l, const T* r) const { return *l < *r; }
	};

	void ASE::Geo::addTrisTypeA(Drawable* Drawable, const ASE::Mtl* mtl, u32 mtlID) const {
		typedef Vtx_P3N3 Vtx;

		typedef map<Vtx*, size_t, Less<Vtx> > Map;
		vector<Vtx> vec;
		vector<u16>	idx;
		Map mpv;
		Vtx vtx;
		size_t j;

		pair<Map::iterator, bool> p;
		vec.reserve(m_tris.size() * 3); // musn't realloc during loop
		idx.reserve(m_tris.size() * 3);

		for (vector<Tri>::const_iterator i = m_tris.begin(); i < m_tris.end(); ++i) {
			if (mtlID == UINT_MAX || // all tris consist of this material or
					i->m_mtlID == mtlID) { // check to see if tri is of this material
				vtx.m_x = i->m_vtx1->m_x;
				vtx.m_y = i->m_vtx1->m_y;
				vtx.m_z = i->m_vtx1->m_z;
				vtx.m_nx = i->m_nrm1.m_x;
				vtx.m_ny = i->m_nrm1.m_y;
				vtx.m_nz = i->m_nrm1.m_z;

				vec.push_back(vtx);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);

				vtx.m_x = i->m_vtx2->m_x;
				vtx.m_y = i->m_vtx2->m_y;
				vtx.m_z = i->m_vtx2->m_z;
				vtx.m_nx = i->m_nrm2.m_x;
				vtx.m_ny = i->m_nrm2.m_y;
				vtx.m_nz = i->m_nrm2.m_z;

				vec.push_back(vtx);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);

				vtx.m_x = i->m_vtx3->m_x;
				vtx.m_y = i->m_vtx3->m_y;
				vtx.m_z = i->m_vtx3->m_z;
				vtx.m_nx = i->m_nrm3.m_x;
				vtx.m_ny = i->m_nrm3.m_y;
				vtx.m_nz = i->m_nrm3.m_z;

				vec.push_back(vtx);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);
			}
		}
		if (!vec.empty()) {
			if (mtl) {
				Drawable->addPkg(mtl->m_mtl, idx, vec);
			} else {
				Material* mat;
				if (MaterialMgr::exists("ASE default")) {
					mat = MaterialMgr::create("ASE Default"); // multiple refs possible
				} else {
					mat = MaterialMgr::create("ASE Default");
					Shader* s = ShaderMgr::create("P3N3");
					mat->setShader(s);
					s->rmvRef();
				}
				Drawable->addPkg(mat, idx, vec);
				mat->rmvRef();
			}
		}
	}
	void ASE::Geo::addTrisTypeB(Drawable* Drawable, const ASE::Mtl* mtl, u32 mtlID) const {
		typedef Vtx_P3N3T2 Vtx;

		typedef map<Vtx*, size_t, Less<Vtx> > Map;
		vector<Vtx> vec;
		vector<u16>	idx;
		Map mpv;
		Vtx vtx;
		size_t j;

		pair<Map::iterator, bool> p;
		vec.reserve(m_tris.size() * 3); // musn't realloc during loop
		idx.reserve(m_tris.size() * 3);

		for (vector<Tri>::const_iterator i = m_tris.begin(); i < m_tris.end(); ++i) {
			if (mtlID == UINT_MAX || // all tris consist of this material or
					i->m_mtlID == mtlID) { // check to see if tri is of this material
				vtx.m_x = i->m_vtx1->m_x;
				vtx.m_y = i->m_vtx1->m_y;
				vtx.m_z = i->m_vtx1->m_z;
				vtx.m_nx = i->m_nrm1.m_x;
				vtx.m_ny = i->m_nrm1.m_y;
				vtx.m_nz = i->m_nrm1.m_z;
				vtx.m_u0 = (i->m_tuv1->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
				vtx.m_v0 = (i->m_tuv1->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

				vec.push_back(vtx);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);

				vtx.m_x = i->m_vtx2->m_x;
				vtx.m_y = i->m_vtx2->m_y;
				vtx.m_z = i->m_vtx2->m_z;
				vtx.m_nx = i->m_nrm2.m_x;
				vtx.m_ny = i->m_nrm2.m_y;
				vtx.m_nz = i->m_nrm2.m_z;
				vtx.m_u0 = (i->m_tuv2->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
				vtx.m_v0 = (i->m_tuv2->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

				vec.push_back(vtx);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);

				vtx.m_x = i->m_vtx3->m_x;
				vtx.m_y = i->m_vtx3->m_y;
				vtx.m_z = i->m_vtx3->m_z;
				vtx.m_nx = i->m_nrm3.m_x;
				vtx.m_ny = i->m_nrm3.m_y;
				vtx.m_nz = i->m_nrm3.m_z;
				vtx.m_u0 = (i->m_tuv3->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
				vtx.m_v0 = (i->m_tuv3->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

				vec.push_back(vtx);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);
			}
		}
		if (!vec.empty()) {
			Drawable->addPkg(mtl->m_mtl, idx, vec);
		}
	}

	void ASE::Geo::addTrisTypeC(Drawable* Drawable, const ASE::Mtl* mtl, u32 mtlID) const {
		typedef Vtx_P3N3B3T2 Vtx;

		typedef map<Vtx*, size_t, Less<Vtx> > Map;
		vector<Vtx> vec;
		vector<u16>	idx;
		Vtx vtx1;
		Vtx vtx2;
		Vtx vtx3;
		size_t j;
		Map mpv;

		pair<Map::iterator, bool> p;
		vec.reserve(m_tris.size() * 3); // musn't realloc during loop
		idx.reserve(m_tris.size() * 3);

		for (vector<Tri>::const_iterator i = m_tris.begin(); i < m_tris.end(); ++i) {
			if (mtlID == UINT_MAX || // all tris consist of this material or
					i->m_mtlID == mtlID) { // check to see if tri is of this material
				vtx1.m_x = i->m_vtx1->m_x;
				vtx1.m_y = i->m_vtx1->m_y;
				vtx1.m_z = i->m_vtx1->m_z;
				vtx1.m_nx = i->m_nrm1.m_x;
				vtx1.m_ny = i->m_nrm1.m_y;
				vtx1.m_nz = i->m_nrm1.m_z;
				vtx1.m_u0 = (i->m_tuv1->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
				vtx1.m_v0 = (i->m_tuv1->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

				vtx2.m_x = i->m_vtx2->m_x;
				vtx2.m_y = i->m_vtx2->m_y;
				vtx2.m_z = i->m_vtx2->m_z;
				vtx2.m_nx = i->m_nrm2.m_x;
				vtx2.m_ny = i->m_nrm2.m_y;
				vtx2.m_nz = i->m_nrm2.m_z;
				vtx2.m_u0 = (i->m_tuv2->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
				vtx2.m_v0 = (i->m_tuv2->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

				vtx3.m_x = i->m_vtx3->m_x;
				vtx3.m_y = i->m_vtx3->m_y;
				vtx3.m_z = i->m_vtx3->m_z;
				vtx3.m_nx = i->m_nrm3.m_x;
				vtx3.m_ny = i->m_nrm3.m_y;
				vtx3.m_nz = i->m_nrm3.m_z;
				vtx3.m_u0 = (i->m_tuv3->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
				vtx3.m_v0 = (i->m_tuv3->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

				calcBinormal(vtx1, vtx2, vtx3);
				calcBinormal(vtx2, vtx3, vtx1);
				calcBinormal(vtx3, vtx1, vtx2);

				vec.push_back(vtx1);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);

				vec.push_back(vtx2);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);

				vec.push_back(vtx3);
				j = vec.size() - 1;
				p = mpv.insert(Map::value_type(&vec[j], j));
				if (!p.second) {
					vec.pop_back();
					j = p.first->second;
				}
				idx.push_back((u16)j);
			}
		}
		if (!vec.empty()) {
			Drawable->addPkg(mtl->m_mtl, idx, vec);
		}
	}
	void ASE::Geo::addStdMtlTris(Drawable* Drawable, const ASE::Mtl* mtl, u32 mtlID) const {
		if (mtl->m_mtl->getTexture(0)) { // has diffuse texture map
			if (mtl->m_mtl->getTexture(1)) { // has normal texture map
				addTrisTypeC(Drawable, mtl, mtlID);
			} else {
				addTrisTypeB(Drawable, mtl, mtlID);
			}
		} else {
			addTrisTypeA(Drawable, mtl, mtlID);
		}
	}
	void ASE::Geo::addMulMtlTris(Drawable* Drawable, const ASE::Mtl* mtl) const {
		EVE_ASSERT(!mtl->m_mtls.empty());
		for (u32 i = 0; i < mtl->m_mtls.size(); ++i) {	// for each sub material
			addStdMtlTris(Drawable, mtl->m_mtls[i], i); // collect relevant tris
		}
	}
}