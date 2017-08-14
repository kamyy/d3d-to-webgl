#include "BasicTypes.h"
#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"

namespace {
	using namespace eve;
	const f32 NORMAL_SCALE = 0.25f;
	const f32 TANGENT_SCALE = 0.25f;
	const f32 BINORMAL_SCALE = 0.25f;

	vector<Vtx_P3C4> s_lineVtxs;
	Material* s_lineMtl;
	struct QPkg {
		f32 m_d;
		RefFrame* m_frm;
		Drawable::Pkg* m_pkg;
	};
	list<QPkg> s_alphaQ;

	bool cmpFunc(QPkg& a, QPkg& b) {
		return a.m_d > b.m_d;
	}
}

namespace eve {
	//
	// Drawable
	//
	Drawable::Drawable() {
		if (MaterialMgr::exists("LineMtl")) {
			s_lineMtl->addRef();
		} else {
			Shader* shader = ShaderMgr::create("P3C4");
			s_lineMtl = MaterialMgr::create("LineMtl");
			s_lineMtl->setShader(shader);
			shader->rmvRef();
		}
	}
	Drawable::~Drawable() {
		for (u32 i = 0; i < m_pkgs.size(); ++i) {
			delete m_pkgs[i];
		}
		s_lineMtl->rmvRef();
	}

	void Drawable::setupAlphaQ() {
		if (!s_alphaQ.empty()) {
			s_alphaQ.clear();
		}
	}
	void Drawable::flushAlphaQ() {
		if (!s_alphaQ.empty()) {
			DWORD dw1, dw2, dw3;
			s_alphaQ.sort(cmpFunc);

			getDevice()->GetRenderState(D3DRS_SRCBLEND, &dw1);
			getDevice()->GetRenderState(D3DRS_DESTBLEND, &dw2);
			getDevice()->GetRenderState(D3DRS_ALPHABLENDENABLE, &dw3);
			getDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			getDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			getDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			list<QPkg>::iterator i = s_alphaQ.begin();
			list<QPkg>::iterator j = s_alphaQ.end();
			while (i != j) {
				Shader::setRefFrame(i->m_frm);
				i->m_pkg->draw(true);
				i++;
			}

			getDevice()->SetRenderState(D3DRS_SRCBLEND, dw1);
			getDevice()->SetRenderState(D3DRS_DESTBLEND, dw2);
			getDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, dw3);
		}
	}

	void Drawable::draw() const {
		for (u32 i = 0; i < m_pkgs.size(); ++i) {
			m_pkgs[i]->draw(false);
		}
	}
	void Drawable::drawAtOnce() const {
		for (u32 i = 0; i < m_pkgs.size(); ++i) {
			m_pkgs[i]->draw(true);
		}
	}
	void Drawable::drawNormals() const {
		for (u32 i = 0; i < m_pkgs.size(); ++i) {
			m_pkgs[i]->drawNormals();
		}
	}

	void Drawable::initD3DResources() {
		for (u32 i = 0; i < m_pkgs.size(); ++i) {
			m_pkgs[i]->initD3DResources();
		}
	}
	void Drawable::freeD3DResources() {
		for (u32 i = 0; i < m_pkgs.size(); ++i) {
			m_pkgs[i]->freeD3DResources();
		}
	}

	//
	// Drawable Package
	//
	void eve::Drawable::Pkg::freeD3DResources() {
		if (m_idxBuf) {
			m_idxBuf->Release();
			m_idxBuf = 0;
		}
		if (m_vtxBuf) {
			m_vtxBuf->Release();
			m_vtxBuf = 0;
		}
	}
	void Drawable::Pkg::drawNormals() {
		// this function is slow because it uses drawPrimitiveUP and it also generates
		// vertices on the fly but this is ok as this function is purely for debugging
		switch (m_mtl->getShader()->getDeclIdentifier()) {
			case VtxMgr::DI_P3N3: {
				PkgT<Vtx_P3N3>* pkg = (PkgT<Vtx_P3N3>*)this;
				vector<Vtx_P3N3>::iterator i = pkg->m_vtxs.begin();

				if (s_lineVtxs.size() < pkg->m_vtxs.size() * 2) {
					s_lineVtxs.resize(pkg->m_vtxs.size() * 2);
				}
				vector<Vtx_P3C4>::iterator j = s_lineVtxs.begin();
				while (i < pkg->m_vtxs.end()) {
					j->m_x = i->m_x;
					j->m_y = i->m_y;
					j->m_z = i->m_z;
					j->m_r = 0.f;
					j->m_g = 0.f;
					j->m_b = 1.f;
					j->m_a = 1.f;
					++j;
					j->m_x = i->m_x + i->m_nx * NORMAL_SCALE;
					j->m_y = i->m_y + i->m_ny * NORMAL_SCALE;
					j->m_z = i->m_z + i->m_nz * NORMAL_SCALE;
					j->m_r = 0.f;
					j->m_g = 0.f;
					j->m_b = 1.f;
					j->m_a = 1.f;
					++j;

					++i;
				}
				s_lineMtl->setDevice();
				EVE_VALIDATE_HR(getDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 
								(UINT)pkg->m_vtxs.size(), &s_lineVtxs[0], sizeof(Vtx_P3C4)));
			}
			break;

			case VtxMgr::DI_P3N3T2: {
				PkgT<Vtx_P3N3T2>* pkg = (PkgT<Vtx_P3N3T2>*)this;
				vector<Vtx_P3N3T2>::iterator i = pkg->m_vtxs.begin();

				if (s_lineVtxs.size() < pkg->m_vtxs.size() * 2) {
					s_lineVtxs.resize(pkg->m_vtxs.size() * 2);
				}
				vector<Vtx_P3C4>::iterator j = s_lineVtxs.begin();
				while (i < pkg->m_vtxs.end()) {
					j->m_x = i->m_x;
					j->m_y = i->m_y;
					j->m_z = i->m_z;
					j->m_r = 0.f;
					j->m_g = 0.f;
					j->m_b = 1.f;
					j->m_a = 1.f;
					++j;
					j->m_x = i->m_x + i->m_nx * NORMAL_SCALE;
					j->m_y = i->m_y + i->m_ny * NORMAL_SCALE;
					j->m_z = i->m_z + i->m_nz * NORMAL_SCALE;
					j->m_r = 0.f;
					j->m_g = 0.f;
					j->m_b = 1.f;
					j->m_a = 1.f;
					++j;

					++i;
				}
				s_lineMtl->setDevice();
				EVE_VALIDATE_HR(getDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 
								(UINT)pkg->m_vtxs.size(), &s_lineVtxs[0], sizeof(Vtx_P3C4)));
			}
			break;

			case VtxMgr::DI_P3N3B3T2: {
				PkgT<Vtx_P3N3B3T2>* pkg = (PkgT<Vtx_P3N3B3T2>*)this;
				vector<Vtx_P3N3B3T2>::iterator i = pkg->m_vtxs.begin();

				if (s_lineVtxs.size() < pkg->m_vtxs.size() * 6) {
					s_lineVtxs.resize(pkg->m_vtxs.size() * 6);
				}
				vector<Vtx_P3C4>::iterator j = s_lineVtxs.begin();
				while (i < pkg->m_vtxs.end()) {
					j->m_x = i->m_x;
					j->m_y = i->m_y;
					j->m_z = i->m_z;
					j->m_r = 0.f;
					j->m_g = 0.f;
					j->m_b = 1.f;
					j->m_a = 1.f;
					++j;
					j->m_x = i->m_x + i->m_nx * NORMAL_SCALE;
					j->m_y = i->m_y + i->m_ny * NORMAL_SCALE;
					j->m_z = i->m_z + i->m_nz * NORMAL_SCALE;
					j->m_r = 0.f;
					j->m_g = 0.f;
					j->m_b = 1.f;
					j->m_a = 1.f;
					++j;

					j->m_x = i->m_x;
					j->m_y = i->m_y;
					j->m_z = i->m_z;
					j->m_r = 0.f;
					j->m_g = 1.f;
					j->m_b = 0.f;
					j->m_a = 1.f;
					++j;
					j->m_x = i->m_x + i->m_bx * BINORMAL_SCALE;
					j->m_y = i->m_y + i->m_by * BINORMAL_SCALE;
					j->m_z = i->m_z + i->m_bz * BINORMAL_SCALE;
					j->m_r = 0.f;
					j->m_g = 1.f;
					j->m_b = 0.f;
					j->m_a = 1.f;
					++j;

					j->m_x = i->m_x;
					j->m_y = i->m_y;
					j->m_z = i->m_z;
					j->m_r = 1.f;
					j->m_g = 0.f;
					j->m_b = 0.f;
					j->m_a = 1.f;
					++j;
					// cross binormal and normal for tangent vector
					j->m_x = i->m_x + (i->m_by * i->m_nz - i->m_bz * i->m_ny) * TANGENT_SCALE;
					j->m_y = i->m_y + (i->m_bz * i->m_nx - i->m_bx * i->m_nz) * TANGENT_SCALE;
					j->m_z = i->m_z + (i->m_bx * i->m_ny - i->m_by * i->m_nx) * TANGENT_SCALE;
					j->m_r = 1.f;
					j->m_g = 0.f;
					j->m_b = 0.f;
					j->m_a = 1.f;
					++j;

					++i;
				}
				s_lineMtl->setDevice();
				EVE_VALIDATE_HR(getDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 
								(UINT)pkg->m_vtxs.size() * 3, &s_lineVtxs[0], sizeof(Vtx_P3C4)));
			}
			break;
		}
	}
	void Drawable::Pkg::addToAlphaQ() {
		// material is translucent so delay rendering it until the end of the frame
		// by adding it to a queue to be later sorted by y in view space
		QPkg q;
		Matrix4x4 m = Shader::getCamera()->getViewMatrix();

		// min x min y min z corner of bbox in model space
		Vector1x4 p(m_min.m_x,
					m_min.m_y,
					m_min.m_z, 1.f);
		q.m_d = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		// max x min y min z corner of bbox in model space
		p.m_x = m_max.m_x;
		p.m_y = m_min.m_y;
		p.m_z = m_min.m_z;
		p.m_w = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		q.m_d = (q.m_d > p.m_w) ? q.m_d : p.m_w;
		// max x min y max z corner of bbox in model space
		p.m_x = m_max.m_x;
		p.m_y = m_min.m_y;
		p.m_z = m_max.m_z;
		p.m_w = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		q.m_d = (q.m_d > p.m_w) ? q.m_d : p.m_w;
		// min x min y max z corner of bbox in model space
		p.m_x = m_min.m_x;
		p.m_y = m_min.m_y;
		p.m_z = m_max.m_z;
		p.m_w = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		q.m_d = (q.m_d > p.m_w) ? q.m_d : p.m_w;

		// min x max y min z corner of bbox in model space
		p.m_x = m_min.m_x;
		p.m_y = m_max.m_y;
		p.m_z = m_min.m_z;
		p.m_w = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		// max x max y min z corner of bbox in model space
		p.m_x = m_max.m_x;
		p.m_y = m_max.m_y;
		p.m_z = m_min.m_z;
		p.m_w = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		q.m_d = (q.m_d > p.m_w) ? q.m_d : p.m_w;
		// max x max y max z corner of bbox in model space
		p.m_x = m_max.m_x;
		p.m_y = m_max.m_y;
		p.m_z = m_max.m_z;
		p.m_w = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		q.m_d = (q.m_d > p.m_w) ? q.m_d : p.m_w;
		// min x max y max z corner of bbox in model space
		p.m_x = m_min.m_x;
		p.m_y = m_max.m_y;
		p.m_z = m_max.m_z;
		p.m_w = (p.m_x * m.m_12) + (p.m_y * m.m_22) + (p.m_z * m.m_32) + m.m_42;
		q.m_d = (q.m_d > p.m_w) ? q.m_d : p.m_w;

		q.m_frm = Shader::getRefFrame();
		q.m_pkg = this;
		s_alphaQ.push_back(q);
	}
}