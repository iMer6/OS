#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);

    if (argc != 3) {
        cerr << "Правильні аргументи: <одиниця_виміру> <довжина>." << endl;
        cerr << "Одиниці виміру: 1 - мм, 2 - см, 3 - м, 4 - дм, 5 - км." << endl;
        return 1;
    }

    int measure;
    float length;

    try {
        measure = stoi(argv[1]);
        length = stof(argv[2]);

        if (measure < 1 || measure > 5) {
            cerr << "Неправильна одиниця виміру. Введіть число від 1 до 5." << endl;
            cerr << "Одиниці виміру: 1 - мм, 2 - см, 3 - м, 4 - дм, 5 - км." << endl;
            return 1;
        }
        if (length < 0) {
            cerr << "Довжина не може бути від'ємною." << endl;
            return 1;
        }
    } catch (const invalid_argument &e) {
        cerr << "Помилка! Введіть правильні аргументи: <одиниця виміру> <довжина>" << endl;
        return 1;
    } catch (const out_of_range &e) {
        cerr << "Помилка введення! Ви ввели число, яке не поміщається в діапазоні." << endl;
        return 1;
    }

    float length_in_meters;

    switch (measure) {
        case 1:
            length_in_meters = length / 1000;
            break;
        case 2:
            length_in_meters = length / 100;
            break;
        case 3:
            length_in_meters = length;
            break;
        case 4:
            length_in_meters = length / 10;
            break;
        case 5:
            length_in_meters = length * 1000;
            break;
        default:
            cerr << "Неправильна одиниця виміру. Введіть число від 1 до 5." << endl;
            cerr << "Одиниці виміру: 1 - мм, 2 - см, 3 - м, 4 - дм, 5 - км." << endl;
            return 1;
    }

    cout << "Довжина відрізка в метрах: " << fixed << length_in_meters << " м" << endl;
    return 0;
}