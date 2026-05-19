#pragma once

#ifdef MATH64_EXPORTS
    #define MATH64_API __declspec(dllexport)
#else
    #define MATH64_API __declspec(dllimport)
#endif

extern "C" {
    MATH64_API bool Add(long long a, long long b, long long* res);
    MATH64_API bool Sub(long long a, long long b, long long* res);
    MATH64_API bool Mul(long long a, long long b, long long* res);
    MATH64_API bool Div(long long a, long long b, long long* res);
}