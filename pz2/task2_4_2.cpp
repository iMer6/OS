//
// Сформувати динамічний двовимірний масив,
// заповнити його випадковими числами і вивести на друк.
// Завдання: видалити рядок з номерок K.
//
// Для виділення пам'яті під масиви використовувати операцію new,
// для видалення масивів з пам'яті – операцію delete.
//
// Для виділення пам'яті, заповнення масивів, видалення елементів
// (рядків, стовпців) написати окремі функції.
// У функції main() повинні бути розміщені тільки опис змінних
// і звернення до відповідних функцій.
//
// Для реалізації інтерфейсу використовувати текстове меню.
//
// При видаленні елементів передбачити помилкові ситуації:
// . видалення елементу з порожнього масиву
// . кількість елементів, що видаляються > кількості наявних елементів.
// У цьому випадку вивести повідомлення про помилку.
// 
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib> // system("cls"), RAND_MAX
#include <random> // mt19937, uniform_int_distribution
#include <limits> // numeric_limits

using std::cout;
using std::cerr;
using std::cin;

/**
 * @brief Maximum array size. 
 */
const int MAX_SIZE = 10;
/**
 * @brief Currently number of rows in array.
 */
int n;
/**
 * @brief Currently number of columns in array.
 */
int m;

bool validN();
bool validM();
void wrongInput();
void invalidInput(std::string word);
inline bool isArrayExist(int** arr);
void textMenu();
char actionCheck(std::string action);
int** createTwoDArray(int** arr, int n, int m);
void fillTwoDArray(int** array, int n, int m);
void deleteRowByNumber(int** &arr, int &n);
void printTwoDArray(int** arr, int n, int m);
void deleteTwoDArray(int** &arr, int n);

int main() {
    system("cls");

    cout << "Введіть кількість рядків (n) масиву: ";
    if (!validN()) return 1;

    cout << "Введіть кількість стовпців (m) масиву: ";
    if (!validM()) return 1;

    int** arr = nullptr;

    std::string action;
    do {
        switch (actionCheck(action)) {
            case '1': arr = createTwoDArray(arr, n, m);
                break;
            case '2': fillTwoDArray(arr, n, m);
                break;
            case '3': deleteRowByNumber(arr, n);
                break;
            case '4': printTwoDArray(arr, n, m);
                break;
            case '5': deleteTwoDArray(arr, n);
                return 0;
                break;
            default: cout << "\nДії " << action << " не існує.";
        }
    } while (action != "5");
}

/**
 * @brief Check an input number of rows (n).
 * This value must be in range (0; MAX_SIZE].
 * @return true if n valid.
 * @return false if n incorrect or invalid.
 * @note Updates the global variable.
 * @attention If input is non-numeric, calls wrongInput().
 * @attention If n is out of range, calls invalidInput(). 
 */
bool validN() {
    if (!(cin >> n)) {
        wrongInput();
        return false;
    }
    if (n <= 0 || n > MAX_SIZE) {
        invalidInput("рядків");
        return false;
    }
    return true;
}

/**
 * @brief Check an input number of columns (m).
 * This value must be in range (0; MAX_SIZE].
 * @return true if m valid.
 * @return false if m incorrect or invalid.
 * @note Updates the global variable.
 * @attention If input is non-numeric, calls wrongInput().
 * @attention If m is out of range, calls invalidInput().
 */
bool validM() {
    if (!(cin >> m)) {
        wrongInput();
        return false;
    }
    if (m <= 0 || m > MAX_SIZE) {
        invalidInput("стовпців");
        return false;
    }
    return true;
}

/**
 * @brief Displays a message about error input.
 */
void wrongInput() {
    cerr << "\nНеправильний ввід.\n";
    cout << "Спробуйте ціле позитивне число.\n";
}

/**
 * @brief Displays a message about invalid input.
 */
void invalidInput(std::string word) {
    cerr << "\nМасив не може мати таку кількість " << word << ".\n";
    cout << "Спробуйте ввести ціле позитивне число.\n";
}

/**
 * @brief Checks if a dynamic 2D array exists.
 * @param arr A pointer to a pointer of integers (int**) to be validated.
 * @note This is an inline function to reduce call overhead.
 * @return true if the array pointer is not null.
 * @return false if the array pointer is null.
 */
inline bool isArrayExist(int** arr) {
    if (arr == nullptr) { cout << "Масиву не існує.\n"; }
    return arr != nullptr;
}

/**
 * @brief Display menu of possible actions.
 */
void textMenu() {
    cout << "\nМожливі дії:\n";
    cout << "1. Формування масиву.\n";
    cout << "2. Заповнення масиву випадковими числами.\n";
    cout << "3. Видалення рядка за номером.\n";
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
 * @brief Allocates memory for a 2D dynamic array (arr).
 * @param n number of rows in the arr.
 * @param m number of columns in the arr.
 * @return Pointer to the newly allocated 2D array of integers.
 * @note Allocated memory should be deallocated using deleteTwoDArray().
 */
int** createTwoDArray(int** arr, int n, int m) {
    if (isArrayExist(arr)) {
        cout << "Масив вже існує.\n";
        return arr;
    }
    
    int** newArr = new int* [n]();
    for (int i = 0; i < n; i++) newArr[i] = new int[m]();
    return newArr;
}

/**
 * @brief Fills the arr with random integers from -RAND_MAX to RAND_MAX.
 * @param arr pointer to a 2D dynamic array of integers.
 * @param n number of rows in the arr.
 * @param m number of columns in the arr.
 * @return Void.
 * @note Uses the Mersenne Twister (mt19937) algorithm for random number generation.
 * https://en.wikipedia.org/wiki/Mersenne_Twister.
 * @note The generator is static to ensure program performance by being created once during the first call.
 */
void fillTwoDArray(int** arr, int n, int m) {
    if (!isArrayExist(arr)) return;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(-RAND_MAX, RAND_MAX);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = distrib(gen);
        }
    }
    printTwoDArray(arr, n, m);
}

/**
 * @brief Deletes a row from a 2D dynamic array at the number entered by the user.
 * @param arr pointer link to a 2D dynamic array of integers.
 * @param n number of rows in the array (decremented on success).
 */
void deleteRowByNumber(int** &arr, int &n) {
    if (!isArrayExist(arr)) return;

    printTwoDArray(arr, n, m);

    cout << "\nВведіть номер рядку, який треба видалити: ";
    int rowIndex;
    if (!(cin >> rowIndex) || rowIndex <= 0 || rowIndex > n) {
        std::clog << "Рядок " << rowIndex << " не було видалено: ";
        cerr << "введено некоректне значення.\n";
        cout << "Число повинно бути цілим від 1 до " << n << ".\n";
        return;
    }

    delete[] arr[rowIndex - 1];
    for (int i = rowIndex - 1; i < n - 1; i++) { arr[i] = arr[i + 1]; }
    n--;

    std::clog << "Рядок " << rowIndex << " видалено.\n";
}

/**
 * @brief Printing the 2D array algorithm.
 * @param arr pointer to a 2D dynamic array of integers.
 * @param n number of rows in the arr.
 * @param m number of columns in the arr.
 * @return Void.
 */
void printTwoDArray(int** arr, int n, int m) {
    if (!isArrayExist(arr)) return;

    cout << "\n";
    for (int i = 0; i < n; i++) {
        cout << "| ";
        for (int j = 0; j < m; j++) {
            cout << std::setw(6) << arr[i][j] << " ";
        }
        cout << "|\n";
    }
    cout << "\n";
}

/**
 * @brief Deallocates memory used by the 2D array.
 * @param arr pointer to a 2D dynamic array of integers.
 * @param n number of rows in the arr.
 */
void deleteTwoDArray(int** &arr, int n) {
    if (!isArrayExist(arr)) return;

    for (int i = 0; i < n; i++) delete[] arr[i];
    delete[] arr;
    arr = nullptr;

    cout << "\nДвовимірний динамічний масив видалено.\n";
}