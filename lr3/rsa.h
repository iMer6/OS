#pragma once

#include <cstdint>

// Macros for automatically export/import functions in DLL
#ifdef BUILDING_RSA_DLL
    #define RSA_API __declspec(dllexport)
#else
    #define RSA_API __declspec(dllimport)
#endif

extern "C" {
    RSA_API bool __cdecl verifyLibraryIntegrity(HMODULE hModule);

    // Fast exponentiation by modulus (binary algorithm)
    RSA_API uint64_t __cdecl modularPow(
        uint64_t base,
        uint64_t exp,
        uint64_t mod
    );

    // RSA key generation
    RSA_API bool generateKeys(
        uint64_t p,
        uint64_t q,
        uint64_t &e,
        uint64_t &d,
        uint64_t &n
    );

    RSA_API uint64_t __cdecl encryptRSA(
        uint64_t message,
        uint64_t e,
        uint64_t n
    );
    RSA_API uint64_t __cdecl decryptRSA(
        uint64_t ciphertext,
        uint64_t d,
        uint64_t n
    );
}