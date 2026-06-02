#define BUILDING_RSA_DLL
#include "rsa.h"

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

bool generateKeys(uint64_t p, uint64_t q, uint64_t &e, uint64_t &d, uint64_t &n) {
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