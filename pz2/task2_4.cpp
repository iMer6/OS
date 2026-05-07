//
// Сформувати динамічний одновимірний масив,
// заповнити його випадковими числами і вивести на друк.
// Завдання: додати елемент в початок масиву.
//
// Для виділення пам'яті під масиви використовувати операцію new,
// для видалення масивів з пам'яті – операцію delete.
//
// Для виділення пам'яті, заповнення масивів і додавання елементів
// (рядків, стовпців) написати окремі функції.
// У функції main() повинні бути розміщені тільки опис змінних
// і звернення до відповідних функцій.
//
// Для реалізації інтерфейсу використовувати текстове меню.
// 
#include <iostream>
#include <string>
#include <cstdlib> // system("cls"), RAND_MAX
#include <random> // mt19937, uniform_int_distribution
#include <limits> // numeric_limits

using std::cout;
using std::cin;
using std::cerr;

/**
 * @brief Maximum array size. 
 */
const int MAX_SIZE = 100;
/**
 * @brief Currently array size.
 */
int size;

int* createOneDArray();
void textMenu();
char actionCheck(std::string action);
void fillOneDArray(int* &arr);
void addElementAtStart(int* &arr);
void printOneDArray(int* arr);
void deleteOneDArray(int* &arr);

int main() {
    system("cls");

    int* arr;

    std::string action;
    do {
        switch (actionCheck(action)) {
            case '1': arr = createOneDArray();
                break;
            case '2': fillOneDArray(arr);
                break;
            case '3': addElementAtStart(arr);
                break;
            case '4': printOneDArray(arr);
                break;
            case '5': deleteOneDArray(arr);
                return 0;
                break;
            default: cout << "\nДії " << action << " не існує.";
        }
    } while (action != "5");
}

/**
 * @brief Allocates memory for a one-dimensional dynamic array. User enters array size. 
 * @return Pointer to the newly allocated 1D array of integers.
 * @note Allocated memory should be deallocated using deleteOneDArray().
 */
int* createOneDArray() {
    while (true) {
        cout << "\nВведіть розмір масиву від 1 до " << MAX_SIZE << ": ";
        if (!(cin >> size) || size <= 0 || size > MAX_SIZE) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cerr << "\nВведено некоректне значення для розміру масиву.\n";
            continue;
        }
        break;
    }
    cout << "Одновимірний динамічний масив створено.\n";
    return new int[size]();
}

/**
 * @brief Display menu of possible actions.
 */
void textMenu() {
    cout << "\nМожливі дії:\n";
    cout << "1. Формування масиву.\n";
    cout << "2. Заповнення масиву випадковими числами.\n";
    cout << "3. Додавання в масив.\n";
    cout << "4. Друк масиву.\n";
    cout << "5. Вихід.\n";
    cout << "\nВиберіть дію з масивом: ";
}

/**
 * @brief Validates action input from the user.
 * @param action user input in string via cin.
 * @return Action number in char. First char in action.
 */
char actionCheck(std::string action) {
    while(true) {
        textMenu();

        cin >> action;
        if (action.length() == 1 && action[0] >= '1' && action[0] <= '5') {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("cls");
            return action[0];
        }
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        system("cls");
        cerr << "\nНевірна дія.\n";
    }
}

/**
 * @brief Fills the array with random integers from -RAND_MAX to RAND_MAX.
 * @param &arr pointer link to a 1D dynamic array of integers.
 * @return Void.
 * @note Uses the Mersenne Twister (mt19937) algorithm for random number generation.
 * https://en.wikipedia.org/wiki/Mersenne_Twister.
 * @note The generator is static to ensure program performance by being created once during the first call.
 */
void fillOneDArray(int* &arr) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(-RAND_MAX, RAND_MAX);

    for (int i = 0; i < size; i++) { arr[i] = distrib(gen); }

    printOneDArray(arr);
}

/**
 * @brief Adds an integer number to the start of an array.
 * @param &arr pointer link to a 1D dynamic array of integers.
 * @return Void.
 * @note The number must be in [-RAND_MAX; RAND_MAX] range.
 */
void addElementAtStart(int* &arr) {
    cout << "Число повинно бути цілим в діапазоні від " 
        << -RAND_MAX << " до " << RAND_MAX << "\n";
    cout << "Введіть число, яке потрібно додати в початок масиву: ";

    int newEl;
    if (!(cin >> newEl) || newEl < -RAND_MAX || newEl > RAND_MAX) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::clog << "Число не було додано: ";
        cerr << "\ndведено некоректне значення.\n";
        cout << "Число повинно бути цілим в діапазоні від " 
            << -RAND_MAX << " до " << RAND_MAX << "\n";
        return;
    }

    int newSize = size + 1;
    int* tempArr = new int[newSize]();

    tempArr[0] = newEl;
    for (int i = 1; i < newSize; i++) tempArr[i] = arr[i - 1];

    delete[] arr;
    arr = tempArr;
    size++;

    std::clog << "\nЕлементи успішно додано.\n";
}

/**
 * @brief Printing the one-dimensional array algorithm.
 * @param arr pointer to a 1D dynamic array of integers.
 * @return Void.
 */
void printOneDArray(int* arr) {
    if (size == 0) {
        cout << "\nМасив порожній.\n";
        return;
    }
    cout << "\n";
    for (int i = 0; i < size; i++) { cout << *(arr + i) << " "; }
    cout << "\n";
}

/**
 * @brief Deallocates memory used by the 1D array.
 * @param arr pointer link to a 1D dynamic array of integers.
 * @return Void.
 */
void deleteOneDArray(int* &arr) {
    if (arr != nullptr) {
        delete[] arr;
        arr = nullptr;
        cout << "Одновимірний динамічний масив видалено.\n";
        return;
    }
    cout << "Масиву не існує";
}