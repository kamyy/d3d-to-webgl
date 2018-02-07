#pragma once
#ifndef Debugging_h
#define Debugging_h

#include <crtdbg.h>

#ifdef _DEBUG
#define EVE_ASSERT(expr) \
	do	{ \
		if (!(expr) && 1 == _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, NULL, #expr)) { \
			_CrtDbgBreak(); \
		} \
	}	while(0)
#else
	#define EVE_ASSERT(expr) do {} while(0)
#endif

#ifdef _DEBUG
#define EVE_VALIDATE(expr) \
	do	{ \
		if (!(expr) && 1 == _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, NULL, #expr)) { \
			_CrtDbgBreak(); \
		} \
	}	while(0)
#else
	#define EVE_VALIDATE(expr) (expr)
#endif

#ifdef _DEBUG
#define EVE_VALIDATE_HR(hr) \
	do	{ \
		HRESULT r = (hr); \
		if (FAILED(r) && 1 == _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, NULL, DXGetErrorString9(r))) { \
			_CrtDbgBreak(); \
		} \
	}	while(0)
#else
	#define EVE_VALIDATE_HR(hr) (hr)
#endif

#endif
