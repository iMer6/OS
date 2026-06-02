#include <iostream>
#include "rsa.h"

using std::cerr;
using std::cout;

int main() {
    // Prime numbers (to 2^32 - 1)
    uint64_t p0 = 61;
    uint64_t q0 = 53;
    
    uint64_t d0;
    uint64_t e0;
    uint64_t n0;

    // Generating the first key pair {d0, e0, n0}
    if (!generateKeys(p0, q0, e0, d0, n0)) {
        cerr << "Error generating the first key pair!\n";
        return 1;
    }
    cout << "Pait 1: e0=" << e0 << ", d0=" << d0 << ", n0=" << n0 << "\n";

    uint64_t p1 = 79;
    uint64_t q1 = 71;

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