// Організувати введення з клавіатури дробового числа.
// Вивести отримане число з двома знаками після коми

#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>
using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    string input;
    while (true) {
        cout << "Введіть дробове число (або натисніть [Y] для виходу): ";
        cin >> input;

        if (input == "Y" || input == "y") { break; }

        try {
            size_t numbers_in_input = 0;
            float input_number = stof(input, &numbers_in_input);

            if (numbers_in_input != input.length()) {
                throw invalid_argument("Помилка! Введіть дробове число.");
            }
            
            cout << "Ваше число " << fixed << setprecision(2) << input_number << endl;
        }
        catch (const invalid_argument& e) {
            cout << "Помилка! Введіть дробове число." << endl;
        }
    }
    return 0;
}