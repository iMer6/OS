#include <iostream>
#include <windows.h>
#include <iomanip>
#include <cmath>

using namespace std;

float F(float x);

int main() {
    SetConsoleOutputCP(65001);

    cout << "+" << setfill('-') << setw(6) << "+" << setw(11) << "+" << endl;
    cout << "|  x  |   F(x)   |" << endl;
    cout << "+" << setfill('-') << setw(6) << "+" << setw(11) << "+" << endl;

    float a = 1.5, b = 5.0, h = 0.3;
    //
    // Do While
    //
    // do {
    //     cout << "| " << fixed << setprecision(1) << a << " | " 
    //         << fixed << setprecision(6) << F(a) << " |" << endl;
    //     a += h;
    // } while (a <= b);

    //
    // While
    //
    // while (a <= b) {
    //     cout << "| " << fixed << setprecision(1) << a << " | " 
    //         << fixed << setprecision(6) << F(a) << " |" << endl;
    //     a += h;
    // }

    //
    // For
    //
    for (; a <= b; a += h) {
        cout << "| " << fixed << setprecision(1) << a << " | " 
            << fixed << setprecision(6) << F(a) << " |" << endl;
    }

    cout << "+" << setfill('-') << setw(6) << "+" << setw(11) << "+" << endl;

    return 0;
}

float F(float x) { return sqrt(x) * pow(cos(x), 2); }