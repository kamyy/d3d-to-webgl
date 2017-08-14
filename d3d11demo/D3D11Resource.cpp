#include "StdAfx.h"
#include "D3D11Resource.h"

namespace d3d11demo {

    D3D11ResourceSet D3D11Resource::s_resources;

    void D3D11Resource::initAllD3D11Resources() {
        for (auto p : s_resources) {
            p->initD3D11Resources();
        }
    }

    void D3D11Resource::freeAllD3D11Resources() {
        for (auto p : s_resources) {
            p->freeD3D11Resources();
        }
    }

    D3D11Resource::D3D11Resource() {
        assert(s_resources.find(this) == s_resources.end());
        s_resources.insert(this);
    }

    D3D11Resource::~D3D11Resource() {
        assert(s_resources.find(this) != s_resources.end());
        s_resources.erase(this);
    }

}
