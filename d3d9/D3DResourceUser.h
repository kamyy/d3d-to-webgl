#pragma once
#ifndef D3DResourceUser_h
#define D3DResourceUser_h

#include "RefCountedObject.h"

namespace eve {
	class D3DResourceUserMgr : public RefCountedObject {
		friend class D3DResourceUser;
	public:
		D3DResourceUserMgr(IDirect3DDevice9* device);
	~	D3DResourceUserMgr();
		static void initD3DResources();
		static void freeD3DResources();
	private:
		typedef std::set<D3DResourceUser*>::iterator SetIterator;
		typedef std::set<D3DResourceUser*> Set;
		static IDirect3DDevice9* s_device;
		static Set* s_set;
	};

	class D3DResourceUser {
		friend class D3DResourceUserMgr;
	public:
		static IDirect3DDevice9* getDevice() {
			return D3DResourceUserMgr::s_device;
		}
		D3DResourceUser();
	~	D3DResourceUser();
	private:
		virtual void initD3DResources() = 0;
		virtual void freeD3DResources() = 0;
	};
}

#endif