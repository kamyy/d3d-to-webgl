#include "BasicTypes.h"

namespace eve {
	D3DResourceUserMgr::Set* D3DResourceUserMgr::s_set = 0;
	IDirect3DDevice9* D3DResourceUserMgr::s_device = 0;

	//
	// D3D Resource User Manager
	//	
	D3DResourceUserMgr::D3DResourceUserMgr(IDirect3DDevice9* device) {
		EVE_ASSERT(s_device == 0);
		EVE_ASSERT(s_set == 0);
		s_device = device;
		device->AddRef();
		s_set = new Set;
	}
	D3DResourceUserMgr::~D3DResourceUserMgr() {
		EVE_ASSERT(s_set->empty());
		EVE_ASSERT(s_device != 0);
		s_device->Release();
		s_device = 0;
		delete s_set;
		s_set = 0;
	}
	void D3DResourceUserMgr::initD3DResources() {
		SetIterator i = s_set->begin();
		SetIterator j = s_set->end();
		while (i != j) {
			(*i++)->initD3DResources();
		}
	}
	void D3DResourceUserMgr::freeD3DResources() {
		SetIterator i = s_set->begin();
		SetIterator j = s_set->end();
		while (i != j) {
			(*i++)->freeD3DResources();
		}
	}

	//
	// D3D Resource User
	//	
	D3DResourceUser::D3DResourceUser() {
		EVE_ASSERT(D3DResourceUserMgr::s_set->find(this) == D3DResourceUserMgr::s_set->end());
		D3DResourceUserMgr::s_set->insert(this);
	}
	D3DResourceUser::~D3DResourceUser() {
		EVE_ASSERT(D3DResourceUserMgr::s_set->find(this) != D3DResourceUserMgr::s_set->end());
		D3DResourceUserMgr::s_set->erase(this);
	}
}