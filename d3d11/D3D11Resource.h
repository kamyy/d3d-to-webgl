#pragma once
#ifndef D3D11Resource_h
#define D3D11Resource_h

namespace d3d11demo {

    using D3D11ResourceSet = std::set<class D3D11Resource*>;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class D3D11Resource {
        static D3D11ResourceSet s_resources;

        virtual void initD3D11Resources() = 0;
        virtual void freeD3D11Resources() = 0;

    public:
        static void initAllD3D11Resources();
        static void freeAllD3D11Resources();

        D3D11Resource();
       ~D3D11Resource();
    };

}

#endif
