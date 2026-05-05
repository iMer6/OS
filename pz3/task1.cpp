/*
Дано натуральне число n. Серед чисел 1,2,...,n знайти всі ті,
які можна представити у вигляді суми квадратів двох натуральних чисел.

Визначити функцію, що дозволяє розпізнати повні квадрати.

Введення даних здійснюється з клавіатури.
Масиви при вирішенні завдання не використовуються.
При вирішення задачі доцільно використовувати цикл з параметром, 
    тому що відомо кількість елементів послідовності.
*/
#include <iostream>
#include <limits>
#include <cmath>

using std::cin;
using std::cout;
using std::cerr;

const size_t MAX_SIZE = 10000;

bool isPerfectSquare(size_t);

int main() {
    size_t n;
    cout << "Введіть число n: ";
    while (!(cin >> n) || n <= 0 || n > MAX_SIZE) {
        cerr << "Помилка вводу.\n";
        cout << "Введіть ціле число від 0 до " << MAX_SIZE << ": ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    cout << "Числа від 1 до " << n << ", які є сумою квадратів двох натуральних чисел:\n";

    // Smallest natural number is 1. 1^2 + 1^2 = 2
    for (size_t i = 2; i <= n; i++) {
        for (size_t a = 1; a < sqrt(i); a++) {
            // i = a^2 + b^2  =>  b^2 = i - a^2
            size_t b_squared = i - a * a;
            if (isPerfectSquare(b_squared)) {
                cout << i << " = " << a << "^2 + " << sqrt(b_squared) << "^2\n";
                break;
            }
        }
    }
    return 0;
}

/**
 * @brief Checks if a number is a perfect square.
 * @param n number to check.
 * @return Bool. Is the number a perfect square?
 */
bool isPerfectSquare(size_t n) {
    size_t root = static_cast<size_t>(round(sqrt(n)));
    return root * root == n;
}