#include "BasicTypes.h"
#include "RefFrame.h"
#include "Shader.h"
#include "ASE.h"

namespace eve {
	ASE::ASE() {
	}

	ASE::~ASE() {
		for (GeoArray::iterator i = m_geos.begin(); i < m_geos.end(); ++i)
			delete (*i);
		for (MtlArray::iterator j = m_mtls.begin(); j < m_mtls.end(); ++j)
			delete (*j);
		for (CamArray::iterator k = m_cams.begin(); k < m_cams.end(); ++k)
			delete (*k);
	}
	bool ASE::skipBlock(istream& is) {
		char buf[512];
		u32 block = 1;
		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			if (strchr(buf, '}')) {
				if (--block == 0) {
					break;
				}
			} else if (strchr(buf, '{')) {
				++block;
			}
			is.getline(buf, sizeof(buf));
		}
		return !is.fail();
	}

	void ASE::createCameras(CameraArray& cameras, CamTgtArray& camTgts, RefFrame* parent) const {
		EVE_ASSERT(cameras.empty());
		EVE_ASSERT(camTgts.empty());
		RefFrame* tgt;
		Camera* cam;

		for (CamArray::const_iterator i = m_cams.begin(); i < m_cams.end(); ++i) {
			tgt = new RefFrame(parent);
			cam = new Camera(	(*i)->m_fov, 1.333f, 
								(*i)->m_clipN, 
								(*i)->m_clipF, tgt);

			tgt->setWorldMatrix((*i)->m_tgtMtx);
			cam->setWorldMatrix((*i)->m_camMtx);
			cameras.push_back(cam);
			camTgts.push_back(tgt);
		}
	}

	void ASE::createActors(ActorList* actors, RefFrame* parent) const {
		EVE_ASSERT(actors->empty());

		for (u32 i = 0; i < m_geos.size(); ++i) {
			const string& myNameStr = m_geos[i]->m_myName;
			const string& parentStr = m_geos[i]->m_parent;
			if (parentStr.empty()) { // must be a root object
				Actor* actor = new Actor(myNameStr.c_str());
				addDrawable(actor, parent, m_geos[i]);
				actors->push_back(actor);
			} else { // look for the parent node
				for (u32 j = 0; j < m_geos.size(); ++j) {
					if (j != i && m_geos[j]->m_myName == parentStr) {
						break;
					}
				}
				if (j == m_geos.size()) { // no parent node found
					Actor* actor = new Actor(myNameStr.c_str());
					addDrawable(actor, parent, m_geos[i]);
					actors->push_back(actor);
				}
			}
		}
	}

	bool ASE::read(const char* filename) {
		char *p, *q, buf[512];
		ifstream is(filename);

		is.getline(buf, sizeof(buf));
		if (is.fail() || !strstr(buf, "*3DSMAX_ASCIIEXPORT"	))
			return false;
		is.getline(buf, sizeof(buf));
		if (is.fail() || !strstr(buf, "*COMMENT"			))
			return false;
		is.getline(buf, sizeof(buf));
		if (is.fail() || !strstr(buf, "*SCENE"				))
			return false;

		if (!m_scn.read(is)) {
			return false;
		}

		is.getline(buf, sizeof(buf));
		if (is.fail() || !strstr(buf, "*MATERIAL_LIST"		))
			return false;

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;
			if (!strcmp(p, "*MATERIAL")) {
				Mtl* mtl = new Mtl;
				if (!mtl->read(is)) {
					delete mtl;
					return false;
				}
				m_mtls.push_back(mtl);
			} else if (strchr(p, '}')) {
				break;
			} else if (strchr(q, '{')) {
				if (!skipBlock(is))
					return false;
			}
			is.getline(buf, sizeof(buf));
		}

		return read(is);
	}
	bool ASE::read(istream& is) {
		char *p, *q, buf[512];

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;

			if (!strcmp(p, "*GROUP")) {
				if (!read(is)) {
					return false;
				}
			} else if (!strcmp(p,	"*GEOMOBJECT"	)) {
				Geo* geo = new Geo;
				if (!geo->read(is)) {
					delete geo;
					return false;
				}
				m_geos.push_back(geo);
			} else if (!strcmp(p,	"*CAMERAOBJECT"	)) {
				Cam* cam = new Cam;
				if (!cam->read(is)) {
					delete cam;
					return false;
				}
				m_cams.push_back(cam);
			} else if (strchr(p, '}')) {
				break;
			} else if (strchr(q, '{')) {
				if (!skipBlock(is))
					return false;
			}
			is.getline(buf, sizeof(buf));
		}
		return true;
	}

	void ASE::addDrawable(Actor* actor, RefFrame* parent, const Geo* geo) const {
		RefFrame* frm = new RefFrame(parent);
		Drawable* drw = new Drawable;

		for (u32 i = 0; i < m_mtls.size(); ++i) {
			if (geo->m_mtlID == i) {
				if (m_mtls[i]->m_mtls.empty()) {
					geo->addStdMtlTris(drw, m_mtls[i]); // triangles reference one material
				} else {
					geo->addMulMtlTris(drw, m_mtls[i]); // triangles grouped by material
				}
				break;
			}
		}
		if (i == m_mtls.size())
			geo->addTrisTypeA(drw, 0, UINT_MAX);
		frm->setWorldMatrix(geo->m_mtx);
		actor->add(frm, drw);
		frm->rmvRef();
		drw->rmvRef();

		for (u32 i = 0; i < m_geos.size(); ++i) {
			// search for and add any child geometry
			const string& s = m_geos[i]->m_parent;
			if (!s.empty() && s == geo->m_myName) {
				addDrawable(actor, frm, m_geos[i]);
			}
		}
	}
}