/*
Використовуючи функції сформувати за допомогою ДСЧ
двовимірний масив і вивести його на друк.

ДСЧ (джерело спеціальних чисел) = ГВЧ (генератор випадкових чисел)

2D: перевернути всі парні рядки матриці.

Введення даних здійснюється з клавіатури.

1. Формування, друк і обробку масиву оформити у вигляді функції.
Масив передавати як параметр функцій.
2. Реалізувати масив як псевдодинамічний, його розмірності
передавати як параметри функцій.
3. Формування масиву виконати з використанням ДСЧ.
В масив записувати і + і - числа.
4. Функція main повинна містити тільки опис масиву і
виклики функцій для формування, друку та обробки масиву.
*/
#include <iostream>
#include <iomanip> // setw()
#include <cstdlib> // system("cls"), RAND_MAX
#include <random> // 

using std::cin;
using std::cout;
using std::cerr;

const size_t MAX_SIZE = 10;

bool validN(size_t&);
bool validM(size_t&);
void wrongInput();
void invalidInput(std::string);
inline bool isArrayExist(int**);
int** createArray(size_t, size_t);
void fillArray(int**, size_t, size_t);
void reverseEvenRowsInArray(int**, size_t, size_t);
void printArray(int**, size_t, size_t);
void deleteArray(int**&, size_t);

int main() {
    system("cls");

    size_t n = 0;
    cout << "Введіть кількість рядків (n) масиву: ";
    if (!validN(n)) return 1;

    size_t m = 0;
    cout << "Введіть кількість стовпців (m) масиву: ";
    if (!validM(m)) return 1;

    int** arr = createArray(n, m);

    fillArray(arr, n, m);
    cout << "\nДвовимірний масив випадкових значень:";
    printArray(arr, n, m);

    reverseEvenRowsInArray(arr, n, m);
    cout << "Оброблений масив:";
    printArray(arr, n, m);

    deleteArray(arr, n);
    return 0;
}

/**
 * @brief Check an input number of rows (n).
 * This value must be in range (0; MAX_SIZE].
 * @return True if n valid.
 * @return False if n incorrect or invalid.
 * @note If input is non-numeric, calls wrongInput().
 * @note If n is out of range, calls invalidInput(). 
 */
bool validN(size_t& n) {
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
 * @return True if m valid.
 * @return False if m incorrect or invalid.
 * @note If input is non-numeric, calls wrongInput().
 * @note If m is out of range, calls invalidInput().
 */
bool validM(size_t& m) {
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
 * @return Bool. Is the array pointer is not null?
 * @note This is an inline function to reduce call overhead.
 */
inline bool isArrayExist(int** arr) {
    if (arr == nullptr) { cout << "Масиву не існує.\n"; }
    return arr != nullptr;
}

/**
 * @brief Allocates memory for a 2D dynamic array of integers.
 * @param n number of rows in the array.
 * @param m number of columns in the array.
 * @return Pointer to the newly allocated 2D array of integers.
 * @note Allocated memory should be deallocated using deleteArray().
 */
int** createArray(size_t n, size_t m) {    
    int** newArr = new int* [n]();
    for (size_t i = 0; i < n; i++) newArr[i] = new int[m]();
    return newArr;
}

/**
 * @brief Fills the array with random integers from -RAND_MAX to RAND_MAX.
 * @param arr pointer to a 2D dynamic array of integers.
 * @param n number of rows in the array.
 * @param m number of columns in the array.
 * @note Uses the Mersenne Twister (mt19937) algorithm for random number generation.
 * https://en.wikipedia.org/wiki/Mersenne_Twister.
 * @note The generator is static to ensure program performance by being created once during the first call.
 */
void fillArray(int** arr, size_t n, size_t m) {
    if (!isArrayExist(arr)) return;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(-RAND_MAX, RAND_MAX);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            arr[i][j] = distrib(gen);
        }
    }
}

/**
 * @brief Reverses elements in rows with even indexes (1, 3, 5, ...).
 * @param arr pointer to a 2D dymanic array of integers.
 * @param n number of rows in the array.
 * @param m number of columns in the array.
 */
void reverseEvenRowsInArray(int** arr, size_t n, size_t m) {
    if (!isArrayExist(arr)) return;
    
    for (size_t i = 1; i < n; i += 2) {
        for (size_t j = 0; j < m / 2; j++) {
            int temp = arr[i][j];
            arr[i][j] = arr[i][m - 1 - j];
            arr[i][m - 1 - j] = temp;
        }
    }
    return;
}

/**
 * @brief Printing the 2D array algorithm.
 * @param arr pointer to a 2D dynamic array of integers.
 * @param n number of rows in the array.
 * @param m number of columns in the array.
 */
void printArray(int** arr, size_t n, size_t m) {
    if (!isArrayExist(arr)) return;

    cout << "\n";
    for (size_t i = 0; i < n; i++) {
        cout << "| ";
        for (size_t j = 0; j < m; j++) {
            cout << std::setw(6) << arr[i][j] << " ";
        }
        cout << "|\n";
    }
    cout << "\n";
}

/**
 * @brief Deallocates memory used by the 2D array.
 * @param arr pointer reference to a 2D dynamic array of integers.
 * @param n number of rows in the array.
 */
void deleteArray(int** &arr, size_t n) {
    if (!isArrayExist(arr)) return;

    for (size_t i = 0; i < n; i++) delete[] arr[i];
    delete[] arr;
    arr = nullptr;

    cout << "\nДвовимірний динамічний масив видалено.\n";
}