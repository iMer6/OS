#define MATH64_EXPORTS
#include "math64.h"
#include <climits> // LLONG_MAX, LLONG_MIN

extern "C" {
    MATH64_API bool Add(long long a, long long b, long long* result) {
        if (b > 0 && a > LLONG_MAX - b) return false; // Overflow up
        if (b < 0 && a < LLONG_MIN - b) return false; // Overflow down
        
        *result = a + b;
        return true;
    }

    MATH64_API bool Sub(long long a, long long b, long long* result) {
        if (b > 0 && a < LLONG_MIN + b) return false; // Overflow up
        if (b < 0 && a > LLONG_MAX + b) return false; // Overflow down
        
        *result = a - b;
        return true;
    }

    MATH64_API bool Mul(long long a, long long b, long long* result) {
        if (a == 0 || b == 0) {
            *result = 0;
            return true;
        }
        if (a > 0) {
            if (b > 0 && a > LLONG_MAX / b) return false;
            if (b < 0 && b < LLONG_MIN / a) return false;
        } else {
            if (b > 0 && a < LLONG_MIN / b) return false;
            if (b < 0 && b < LLONG_MAX / a) return false;
        }
        
        *result = a * b;
        return true;
    }
    
    MATH64_API bool Div(long long a, long long b, long long* result) {
        if (b == 0) return false; // Division by 0
        
        if (a == LLONG_MIN && b == -1) return false; 
        
        *result = a / b;
        return true;
    }
}