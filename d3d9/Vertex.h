#pragma once
#ifndef Vertex_h
#define Vertex_h

namespace eve {
	struct Vtx_P3 {
		f32 m_x;
		f32 m_y;
		f32 m_z;
		bool operator<(const Vtx_P3& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) < 0; 
		}
		bool operator>(const Vtx_P3& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) > 0; 
		}
	};
	struct Vtx_P3C4 {
		f32 m_x;
		f32 m_y;
		f32 m_z;
		f32 m_r;
		f32 m_g;
		f32 m_b;
		f32 m_a;
		bool operator<(const Vtx_P3C4& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) < 0; 
		}
		bool operator>(const Vtx_P3C4& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) > 0; 
		}
	};
	struct Vtx_P3N3 {
		f32 m_x;
		f32 m_y;
		f32 m_z;
		f32 m_nx;
		f32 m_ny;
		f32 m_nz;
		bool operator<(const Vtx_P3N3& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) < 0; 
		}
		bool operator>(const Vtx_P3N3& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) > 0; 
		}
	};
	struct Vtx_P3N3T2 {
		f32 m_x;
		f32 m_y;
		f32 m_z;
		f32 m_nx;
		f32 m_ny;
		f32 m_nz;
		f32 m_u0;
		f32 m_v0;
		bool operator<(const Vtx_P3N3T2& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) < 0; 
		}
		bool operator>(const Vtx_P3N3T2& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) > 0; 
		}
	};
	struct Vtx_P3N3B3T2 {
		f32 m_x;
		f32 m_y;
		f32 m_z;
		f32 m_nx;
		f32 m_ny;
		f32 m_nz;
		f32 m_bx; 
		f32 m_by;
		f32 m_bz;
		f32 m_u0;
		f32 m_v0;
		bool operator<(const Vtx_P3N3B3T2& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) < 0; 
		}
		bool operator>(const Vtx_P3N3B3T2& rhs) const { 
			return memcmp(this, &rhs, sizeof(*this)) > 0; 
		}
	};

	//
	// Vertex Manager
	//
	class VtxMgr : public RefCountedObject, public D3DResourceUser {
	public:
		enum DeclIdentifier { 
			DI_P3, 
			DI_P3C4, 
			DI_P3N3, 
			DI_P3N3T2, 
			DI_P3N3B3T2,
			MAX_DECL_IDENTIFIERS 
		};
		static IDirect3DVertexDeclaration9* getVtxDeclIdent(DeclIdentifier i) {
			return s_decls[i];
		}
		VtxMgr();
	~	VtxMgr();
	private:
		static IDirect3DVertexDeclaration9* s_decls[MAX_DECL_IDENTIFIERS];
		void initD3DResources();
		void freeD3DResources();
	};
}

#endif
