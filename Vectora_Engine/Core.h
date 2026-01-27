#pragma once

#ifdef V_PLATFORM_WINDOWS
    #ifdef V_BUILD_DLL
        #define VECTORA_API __declspec(dllexport)
    #else
        #define VECTORA_API __declspec(dllimport)
    #endif
#else
    #define VECTORA_API
#endif
#define BIT(x) (1 << x)