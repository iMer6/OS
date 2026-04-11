#include <iostream>
#include <windows.h>
#include <string>
#include <limits>

using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    
    cout << "Введіть кількість елементів: ";
    inputN:
    int n;
    
    if(!(cin >> n)) {
        cout << "Помилка! Введіть ціле число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        goto inputN;
    }

    if (n < 0) {
        cout << "Кількість елементів не може бути від'ємною." << endl;
        return 1;
    }
    if (n == 0) {
        cout << "Послідовність порожня." << endl;
        return 0;
    }

    float min = 0;
    float max = 0;

    int i = 1;
    //
    // Do While
    //
    // do {
    //     cout << "Введіть " << i << " число: ";
    //     inputElement:
    //     float temp;
    //     if(!(cin >> temp)) {
    //         cout << "Помилка! Введіть число: ";
    //         cin.clear();
    //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //         goto inputElement;
    //     }
    //     if (temp < min) { min = temp; }
    //     if (temp > max) { max = temp; }
    // } while (++i <= n);

    //
    // While
    //
    // while (i <= n) {
    //     cout << "Введіть " << i << " число: ";
    //     inputElement:
    //     float temp;
    //     if(!(cin >> temp)) {
    //         cout << "Помилка! Введіть число: ";
    //         cin.clear();
    //         cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //         goto inputElement;
    //     }
    //     if (temp < min) { min = temp; }
    //     if (temp > max) { max = temp; }
    //     i++;
    // }

    //
    // For
    //
    for (int i = 1; i <= n; i++) {
        cout << "Введіть " << i << " число: ";
        inputElement:
        float temp;
        if(!(cin >> temp)) {
            cout << "Помилка! Введіть число: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            goto inputElement;
        }
        if (temp < min) { min = temp; }
        if (temp > max) { max = temp; }
    }

    cout << "\nМаксимальне число: " << max << endl;
    cout << "Мінімальне число: " << min << endl;
    cout << "Різниця мінімального та максимального: " << max - min << endl;
    
    return 0;
}