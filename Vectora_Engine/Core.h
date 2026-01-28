#pragma once
#include <stdint.h>

#ifdef VE_PLATFORM_WINDOWS
    #ifdef VE_BUILD_DLL
        #define VECTORA_API __declspec(dllexport)
    #else
        #define VECTORA_API __declspec(dllimport)
    #endif
#else
    #define VECTORA_API
#endif
#define BIT(x) (1 << x)

#ifdef VE_ENABLE_ASSERTS
#define VE_ASSERT(x, ...) { if(!(x)) { VE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define VE_CORE_ASSERT(x, ...) { if(!(x)) { VE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define VE_ASSERT(x, ...)
#define VE_CORE_ASSERT(x, ...)
#endif

#define VE_UINT uint64_t
#define VE_UINT8 uint8_t
#define VE_UINT16 uint16_t
#define VE_UINT32 uint32_t
#define VE_UINT64 uint64_t
#define VE_FLOAT32 float
#define VE_FLOAT64 double
#define VE_CHAR char
#define VE_BOOL bool
#define VE_SIZE size_t
#define VE_WCHAR wchar_t
#define VE_NULLPTR nullptr
#define VE_STRING std::string
#define VE_WSTRING std::wstring