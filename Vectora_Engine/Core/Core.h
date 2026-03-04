#pragma once
#include <stdint.h>
#include <memory>

#ifdef VE_PLATFORM_WINDOWS
    #if defined(VE_BUILD_STATIC)
        #define VECTORA_API
    #elif defined(VE_BUILD_DLL)
        #define VECTORA_API __declspec(dllexport)
    #else
        #define VECTORA_API __declspec(dllimport)
    #endif
#else
    #define VECTORA_API
#endif
#define BIT(x) (1 << x)

#define VE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef VE_ENABLE_ASSERTS
#define VE_ASSERT(x, ...) { if(!(x)) { VE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define VE_CORE_ASSERT(x, ...) { if(!(x)) { VE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define VE_ASSERT(x, ...)
#define VE_CORE_ASSERT(x, ...)
#endif

namespace Vectora {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#define VE_UINT uint64_t
#define VE_UINT8 uint8_t
#define VE_UINT16 uint16_t
#define VE_UINT32 uint32_t
#define VE_UINT64 uint64_t
#define VE_INT int64_t
#define VE_INT8 int8_t
#define VE_INT16 int16_t
#define VE_INT32 int32_t
#define VE_INT64 int64_t
#define VE_FLOAT32 float
#define VE_FLOAT64 double
#define VE_CHAR char
#define VE_BOOL bool
#define VE_SIZE size_t
#define VE_WCHAR wchar_t
#define VE_NULLPTR nullptr
#define VE_STRING std::string
#define VE_WSTRING std::wstring