#include <iostream>
#include "rsa.h"
#include <random>

using std::cerr;
using std::cout;

bool isPrime(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int main() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<uint64_t> distr(100, 1000); 

    uint64_t p0, q0;

    do { p0 = distr(gen); }
    while (!isPrime(p0));

    do { q0 = distr(gen); }
    while (!isPrime(q0) || q0 == p0);
    
    uint64_t d0;
    uint64_t e0;
    uint64_t n0;

    // Generating the first key pair {d0, e0, n0}
    if (!generateKeys(p0, q0, e0, d0, n0)) {
        cerr << "Error generating the first key pair!\n";
        return 1;
    }
    cout << "Pait 1: e0=" << e0 << ", d0=" << d0 << ", n0=" << n0 << "\n";

    uint64_t p1, q1;

    do { p1 = distr(gen); }
    while (!isPrime(p1));

    do { q1 = distr(gen); }
    while (!isPrime(q1) || q1 == p1);

    uint64_t d1;
    uint64_t e1;
    uint64_t n1;

    // Generating the second key pair {d1, e1, n1}
    if (!generateKeys(p1, q1, e1, d1, n1)) {
        cerr << "Error generating second key pair!\n";
        return 1;
    }
    cout << "Pair 2: e1=" << e1 << ", d1=" << d1 << ", n1=" << n1 << "\n\n";

    // values ​​must be < n0 and < n1
    uint64_t t[] = {12, 42, 105, 255, 7};
    size_t t_size = sizeof(t) / sizeof(t[0]);

    cout << "Encryption/decryption cycle\n";
    for (size_t i = 0; i < t_size; ++i) {
        cout << "\nOriginal number t[i] = " << t[i] << "\n";

        uint64_t e1t = encryptRSA(t[i], e1, n1);
        cout << "==> {e1, n1}: e1t = " << e1t << "\n";

        uint64_t d1e1t = decryptRSA(e1t, d1, n1);
        cout << "<== {d1, n1}: d1e1t = " << d1e1t << "\n";

        if (d1e1t != t[i]) {
            cerr << "Error: d1e1t != t[i]!\n";
            return 1;
        }

        uint64_t e0d1e1t = encryptRSA(d1e1t, e0, n0);
        cout << "==> {e0, n0}: e0d1e1t = " << e0d1e1t << "\n";

        uint64_t d0e0d1e1t = decryptRSA(e0d1e1t, d0, n0);
        cout << "<== {d0, n0}: d0e0d1e1t = " << d0e0d1e1t << "\n";

        if (d0e0d1e1t != t[i]) {
            cerr << "Error: d0e0d1e1t != t[i]!\n";
            return 1;
        }
    }
    return 0;
}