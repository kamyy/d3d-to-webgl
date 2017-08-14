#pragma once
#ifndef StdAfx_h
#define StdAfx_h

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#endif

#define _WIN32_WINNT _WIN32_WINNT_MAXVER

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <cassert>
#include <climits>
#include <cstring>
#include <cstdint>
#include <cmath>

#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

namespace d3d11demo {
    using namespace std;

    inline void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)) {
            throw std::exception();
        }
    }
}

#endif
