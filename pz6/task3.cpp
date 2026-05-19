#include <iostream>
#include "math64.h"
#include <libloaderapi.h> // GetModuleHandleA

using std::cout;

int main() {
    GetModuleHandleA("math64.dll") == NULL
        ? cout << "Динамічне завантаження бібліотеки.\n"
        : cout << "Статичне завантаження бібліотеки.\n";

    long long a = 18;
    long long b = 10;
    long long res;

    Add(a, b, &res)
        ? cout << a << " + " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення!\n";

    Sub(a, b, &res)
        ? cout << a << " - " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення!\n";
    
    Mul(a, b, &res)
        ? cout << a << " * " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення!\n";
    
    Div(a, b, &res)
        ? cout << a << " / " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення або ділення на 0!\n";
    return 0;
}