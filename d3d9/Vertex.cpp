#include "BasicTypes.h"
#include "Vertex.h"

namespace eve {
	const D3DVERTEXELEMENT9 ve_P3[] = {
		{0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0},
		D3DDECL_END()
	};
	const D3DVERTEXELEMENT9 ve_P3C4[] = {
		{0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0},
		{0, 12,	D3DDECLTYPE_FLOAT4,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,		0},
		D3DDECL_END()
	};
	const D3DVERTEXELEMENT9 ve_P3N3[] = {
		{0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0},
		{0, 12,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_NORMAL,	0},
		D3DDECL_END()
	};
	const D3DVERTEXELEMENT9 ve_P3N3T2[] = {
		{0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0},
		{0, 12,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_NORMAL,	0},
		{0, 24, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	0},
		D3DDECL_END()
	};
	const D3DVERTEXELEMENT9 ve_P3N3B3T2[] = {
		{0,	0,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,	0},
		{0, 12,	D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_NORMAL,	0},
		{0, 24, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_BINORMAL,	0},
		{0, 36, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	0},
		D3DDECL_END()
	};

	IDirect3DVertexDeclaration9* VtxMgr::s_decls[MAX_DECL_IDENTIFIERS];

	VtxMgr::VtxMgr() {
		initD3DResources();
	}
	VtxMgr::~VtxMgr() {
		freeD3DResources();
	}
	void VtxMgr::initD3DResources() {
		if (s_decls[DI_P3] == 0)
			EVE_VALIDATE_HR(getDevice()->CreateVertexDeclaration(ve_P3, &s_decls[DI_P3]));

		if (s_decls[DI_P3C4] == 0)
			EVE_VALIDATE_HR(getDevice()->CreateVertexDeclaration(ve_P3C4, &s_decls[DI_P3C4]));

		if (s_decls[DI_P3N3] == 0)
			EVE_VALIDATE_HR(getDevice()->CreateVertexDeclaration(ve_P3N3, &s_decls[DI_P3N3]));

		if (s_decls[DI_P3N3T2] == 0)
			EVE_VALIDATE_HR(getDevice()->CreateVertexDeclaration(ve_P3N3T2, &s_decls[DI_P3N3T2]));

		if (s_decls[DI_P3N3B3T2] == 0)
			EVE_VALIDATE_HR(getDevice()->CreateVertexDeclaration(ve_P3N3B3T2, &s_decls[DI_P3N3B3T2]));
	}
	void VtxMgr::freeD3DResources() {
		for (i32 i = 0; i < MAX_DECL_IDENTIFIERS; ++i) {
			if (s_decls[i]) {
				s_decls[i]->Release(); s_decls[i] = 0;
			}
		}
	}
}