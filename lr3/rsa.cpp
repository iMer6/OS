#define BUILDING_RSA_DLL
#include <windows.h>
#include <iostream>
#include "rsa.h"

using std::cout;
using std::cerr;

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// (e * d) % phi == 1
uint64_t modInverse(uint64_t e, uint64_t phi) {
    int64_t t = 0, newt = 1;
    int64_t r = phi, newr = e;

    while (newr != 0) {
        int64_t quotient = r / newr;
        int64_t temp = t;
        t = newt;
        newt = temp - quotient * newt;

        temp = r;
        r = newr;
        newr = temp - quotient * newr;
    }

    if (r > 1) return 0; // the inverted element doesn't exist
    if (t < 0) t += phi;
    return static_cast<uint64_t>(t);
}

// Binary exponentiation algorithm by modulus
uint64_t modularPow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            // Preventing overflow
            // __int128 when multiplying 64-bit numbers
            result = (static_cast<unsigned __int128>(result) * base) % mod;
        }
        exp = exp >> 1; // division by 2
        base = (static_cast<unsigned __int128>(base) * base) % mod;
    }
    return result;
}

bool generateKeys(
    uint64_t p,
    uint64_t q,
    uint64_t &e,
    uint64_t &d,
    uint64_t &n
) {
    if (p == q) return false; // p must != q

    n = p * q; // modulus
    uint64_t phi = (p - 1) * (q - 1); // Euler function

    e = 3; // open key (comprime with phi)
    while (gcd(e, phi) != 1 && e < phi) {
        e += 2; // skip even numbers
        // even number cannot be coprime with phi
    }

    if (e >= phi) return false;

    d = modInverse(e, phi); // secret key
    return (d != 0);
}

uint64_t encryptRSA(uint64_t message, uint64_t e, uint64_t n) {
    return modularPow(message, e, n); // M^E mod n
}

uint64_t decryptRSA(uint64_t ciphertext, uint64_t d, uint64_t n) {
    return modularPow(ciphertext, d, n); // C^D mod n
}

bool VerifyIntegrity(HMODULE hModule) {
    wchar_t filePath[MAX_PATH]; // 260
    // Full path to the current DLL
    // Error ==> 0
    if (0 == GetModuleFileNameW(
            hModule, // a handle to the loaded module (whose path?)
            filePath, // a pointer to a buffer (where to write the result?)
            MAX_PATH // the size of the buffer (WCHARs)
        )
    ) return false;

    // Oped file via WinAPI
    HANDLE hFile = CreateFileW(
        filePath, // file path
        GENERIC_READ, // file opening mode (read)
        FILE_SHARE_READ, // sharing mode (read)
            // Other processes can read the file at the same time
        NULL, // the handle cannot be inherited (default)
        OPEN_EXISTING, // opens the file only if it exists
            // If the file doesn't exist ==> error
        FILE_ATTRIBUTE_NORMAL, // the file has no other attributes
        NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
    );
    if (hFile == INVALID_HANDLE_VALUE) return false;

    DWORD fileSize = GetFileSize(
        hFile, // a handle to the file (whose size?)
        NULL // where to write a tail? (if file > 4 Gb)
    );
    if (fileSize < sizeof(unsigned int)) {
        CloseHandle(hFile);
        return false;
    }

    DWORD dataSize = fileSize - sizeof(unsigned int);

    DWORD bytesRead;
    unsigned int savedChecksum = 0;
    SetFilePointer(
        hFile, // file handle (where need to change position)
        dataSize, // how many bytes the pointer needs to be moved?
        NULL, // required for files > 4 GB
            // NULL – 64-bit shift is not needed
        FILE_BEGIN // countdown starts from the beginning of the file
    );

    ReadFile(
        hFile, // where to read data from?
        &savedChecksum, // where to write the read data?
        sizeof(savedChecksum), // buffer size (how many bytes need to be read)
        &bytesRead, // where to record the number of characters read?
        NULL // synchronous mode
    );

    SetFilePointer(
        hFile, // file handle (where need to change position)
        0, // how many bytes the pointer needs to be moved?
        NULL, // required for files > 4 GB
            // NULL – 64-bit shift is not needed
        FILE_BEGIN // countdown starts from the beginning of the file
    );
    unsigned int calculatedChecksum = 0;

    const DWORD BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    DWORD remaining = dataSize;

    while (remaining > 0) {
        DWORD toRead = remaining > BUFFER_SIZE
            ? BUFFER_SIZE
            : remaining;
        
        if (
            !ReadFile(
                hFile, // where to read data from?
                buffer, // where to write the read data?
                toRead, // buffer size (how many bytes need to be read?)
                &bytesRead, // where to record the number of characters read?
                NULL // synchronous mode
            )
            || bytesRead == 0
        ) {
            CloseHandle(hFile);
            return false;
        }

        for (DWORD i = 0; i < bytesRead; ++i) {
            calculatedChecksum += static_cast<unsigned int>(buffer[i]);
        }
        remaining -= bytesRead;
    }

    CloseHandle(hFile);

    // If the checksum has not yet been recorded (first assembly),
    // return true so that the calculation utility can run
    if (savedChecksum == 0xCCCCCCCC || savedChecksum == 0) {
        return true;
    }

    return calculatedChecksum == savedChecksum;
}

RSA_API bool __cdecl verifyLibraryIntegrity(HMODULE hModule) {
    return VerifyIntegrity(hModule);
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD reason,
    LPVOID lpReserved
) {
    switch (reason) {
        case DLL_PROCESS_ATTACH: {
            char msg[128];
            // Writes formatted data to an ANSI string buffer
            wsprintfA(msg, "DLLMain ATTACH. lpReserver = %p (%s)\n",
                lpReserved,
                (lpReserved == NULL
                    ? "Dynamic Loading"
                    : "Static Loading"
                )
            );
            OutputDebugStringA(msg);
            cout << msg;

            // Integrity control during download
            if (!VerifyIntegrity(hModule)) {
                cout << "Integrity check failed (Checksum not written yet)\n";
            } else {
                cout << "Intergity check passed successfully\n";
            }
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}