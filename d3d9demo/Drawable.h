#pragma once
#ifndef Drawable_h
#define Drawable_h

#include "Material.h"
#include "D3DResourceUser.h"

namespace eve {
	using namespace std;

	class Drawable : public RefCountedObject, public D3DResourceUser {
	public:
		template<class T> void addPkg(Material* mtl, const vector<u16>& idxs, const vector<T>& vtxs);
		void draw() const;
		void drawAtOnce() const;
		void drawNormals() const;
		Drawable();
	~	Drawable();
		static void setupAlphaQ();
		static void flushAlphaQ();

		struct Pkg {
			virtual ~Pkg() = 0 {}
			virtual void initD3DResources() = 0;
			virtual void draw(bool instant) = 0;
			void freeD3DResources();
			void drawNormals();
			void addToAlphaQ();

			IDirect3DVertexBuffer9*	m_vtxBuf;
			IDirect3DIndexBuffer9*	m_idxBuf;
			vector<u16> m_idxs;
			Material* m_mtl;
			Vector1x4 m_min;
			Vector1x4 m_max;
		};
		template<class T> struct PkgT : public Pkg {
			vector<T> m_vtxs;
			void initD3DResources();
			void draw(bool instant);
			PkgT(Material* mtl, const vector<u16>& idxs, const vector<T>& vtxs);
		~	PkgT();
		};

	private:
		// override D3DResource pure virtuals
		void initD3DResources();
		void freeD3DResources();
	protected:
		vector<Pkg*> m_pkgs;
	};
}

template<class T>
	void eve::Drawable::addPkg(Material* mtl, const vector<u16>& idxs, const vector<T>& vtxs) {
		m_pkgs.push_back(new PkgT<T>(mtl, idxs, vtxs));
	}

template<class T> 
	void eve::Drawable::PkgT<T>::initD3DResources() {
		IDirect3DDevice9* dev = D3DResourceUser::getDevice();

		if (m_idxBuf == 0 && m_vtxBuf == 0) {
			EVE_VALIDATE_HR(dev->CreateIndexBuffer((UINT)m_idxs.size() * sizeof(u16),
				D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_idxBuf, 0));

			EVE_VALIDATE_HR(dev->CreateVertexBuffer((UINT)m_vtxs.size() * sizeof(T), 
				D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vtxBuf, 0));

			T* v; u16* x;

			EVE_VALIDATE_HR(m_idxBuf->Lock(0, 0, (void**)&x, 0));
			memcpy(x, &m_idxs[0], m_idxs.size() * sizeof(u16));
			m_idxBuf->Unlock();

			EVE_VALIDATE_HR(m_vtxBuf->Lock(0, 0, (void**)&v, 0));
			memcpy(v, &m_vtxs[0], m_vtxs.size() * sizeof(T));
			m_vtxBuf->Unlock();
		}
	}

template<class T> 
	void eve::Drawable::PkgT<T>::draw(bool instant) {
		if (instant || !m_mtl->isTranslucent()) {
			m_mtl->setDevice();
			EVE_VALIDATE_HR(getDevice()->SetIndices(m_idxBuf));
			EVE_VALIDATE_HR(getDevice()->SetStreamSource(0, m_vtxBuf, 0, sizeof(T)));
			EVE_VALIDATE_HR(getDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, (UINT)m_vtxs.size(), 0, (UINT)m_idxs.size() / 3));
		} else {
			addToAlphaQ();
		}
	}
	
template<class T>
	eve::Drawable::PkgT<T>::PkgT(Material* mtl, const vector<u16>& idxs, const vector<T>& vtxs) {
		EVE_ASSERT(!vtxs.empty());
		EVE_ASSERT(!idxs.empty());
		m_mtl = mtl;
		m_mtl->addRef();

		m_idxs.resize(idxs.size());
		m_vtxs.resize(vtxs.size());
		memcpy(&m_vtxs[0], &vtxs[0], vtxs.size() * sizeof(T));
		memcpy(&m_idxs[0], &idxs[0], idxs.size() * sizeof(u16));

		if (mtl->isTranslucent()) {
			m_min.m_x = m_max.m_x = vtxs[0].m_x;
			m_min.m_y = m_max.m_y = vtxs[0].m_y;
			m_min.m_z = m_max.m_z = vtxs[0].m_z;
			for (u32 i = 1; i < vtxs.size(); ++i) {
				if (vtxs[i].m_x < m_min.m_x) 
					m_min.m_x = vtxs[i].m_x; 
				if (vtxs[i].m_x > m_max.m_x) 
					m_max.m_x = vtxs[i].m_x;

				if (vtxs[i].m_y < m_min.m_y) 
					m_min.m_y = vtxs[i].m_y; 
				if (vtxs[i].m_y > m_max.m_y) 
					m_max.m_y = vtxs[i].m_y;

				if (vtxs[i].m_z < m_min.m_z) 
					m_min.m_z = vtxs[i].m_z; 
				if (vtxs[i].m_z > m_max.m_z) 
					m_max.m_z = vtxs[i].m_z;
			}
		}
		m_idxBuf = 0;
		m_vtxBuf = 0;
		initD3DResources();
	}
template<class T> 
	eve::Drawable::PkgT<T>::~PkgT() {
		freeD3DResources();	
		m_mtl->rmvRef();
	}

#endif
