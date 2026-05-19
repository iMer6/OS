#include <iostream>
#include <windef.h> // HMODULE, NULL, FARPROC
#include <libloaderapi.h> // GetModuleHandleA, LoadLibraryA, GetProcAddress, FreeLibrary

using std::cout;
using std::cerr;

typedef bool (*SafeOpFunc)(long long, long long, long long*);

int main() {
    GetModuleHandleA("math64.dll") == NULL
        ? cout << "Динамічне завантаження бібліотеки.\n"
        : cout << "Статичне завантаження бібліотеки.\n";

    HMODULE hDll = LoadLibraryA("math64.dll");
    if (!hDll) {
        cerr << "Помилка завантаження бібліотеки!\n";
        return 1;
    }

    SafeOpFunc add = (SafeOpFunc)(void*)GetProcAddress(hDll, "Add");
    SafeOpFunc sub = (SafeOpFunc)(void*)GetProcAddress(hDll, "Sub");
    SafeOpFunc mul = (SafeOpFunc)(void*)GetProcAddress(hDll, "Mul");
    SafeOpFunc div = (SafeOpFunc)(void*)GetProcAddress(hDll, "Div");

    if (!add || !sub || !mul || !div) {
        cerr << "Помилка! Функції не знайдені!\n";
        FreeLibrary(hDll);
        return 1;
    }

    long long a = 30;
    long long b = 10;
    long long res;

    add(a, b, &res)
        ? cout << a << " + " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення!\n";
    
    sub(a, b, &res)
        ? cout << a << " - " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення!\n";
    
    mul(a, b, &res)
        ? cout << a << " * " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення!\n";

    div(a, b, &res)
        ? cout << a << " / " << b << " = " << res << "\n"
        : cout << "Помилка! Переповнення або ділення на 0!\n";

    FreeLibrary(hDll);
    return 0;
}