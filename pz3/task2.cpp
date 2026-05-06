/*
Використовуючи функція сформувати за допомогою ДСЧ
одновимірний масив і вивести його на друк.

ДСЧ (джерело спеціальних чисел) = ГВЧ (генератор випадкових чисел)

1D: створити новий масив з номерів елементів, значення яких дорівнюють 0,
використовуючи функції, результат вивести на друк.

1. Формування, друк і обробку масиву оформити у вигляді функції.
Масив передавати як параметр функцій.
2. Реалізувати масив як псевдодинамічний, його розмірність
передавати як параметр функцій.
3. Формування масиву виконати з використанням ДСЧ.
В масив записувати і + і - числа.
4. Функція main повинна містити тільки опис масиву і
виклики функцій для формування, друку та обробки масиву.
*/
#include <iostream>
#include <cstdlib> // RAND_MAX
#include <random> // mt19937, uniform_int_distribution
#include <limits> // numeric_limits

using std::cin;
using std::cout;
using std::cerr;

const size_t MAX_SIZE = 100;

void validSize(size_t&);
void fillArray(int*, size_t);
int* zeroIndexesInArray(int*, size_t, size_t&);
void printArray(int*, size_t);
void deleteArray(int*&);

int main() {
    size_t size;
    validSize(size);

    int* arr = new int[size]();

    fillArray(arr, size);

    cout << "Одновимірний масив випадкових значень:";
    printArray(arr, size);

    size_t zeroValuesCount = 0;
    int* zeroValueIndexes = zeroIndexesInArray(arr, size, zeroValuesCount);
    if (zeroValueIndexes != nullptr) {
        cout << "Номери елементів, значення яких дорівнює 0:";
        printArray(zeroValueIndexes, zeroValuesCount);
        deleteArray(zeroValueIndexes);
    }

    deleteArray(arr);
    return 0;
}

/**
 * @brief Prompts user for array size and validates the input.
 * @param[out] size reference to size_t variable to store the validated size.
 * @note Input is restricted th the range [1, MAX_SIZE].
 */
void validSize(size_t &size) {
    while (true) {
        cout << "\nВведіть розмір масиву від 1 до " << MAX_SIZE << ": ";
        if (!(cin >> size) || size <= 0 || size > MAX_SIZE) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cerr << "Введено некоректне значення для розміру масиву.\n";
            continue;
        }
        break;
    }
}

/**
 * @brief Fills the array with random integers from -RAND_MAX to RAND_MAX.
 * @param arr pointer to a 1D dynamic array of integers.
 * @param size array length.
 * @return Void.
 * @note Uses the Mersenne Twister (mt19937) algorithm for random number generation.
 * https://en.wikipedia.org/wiki/Mersenne_Twister.
 * @note The generator is static to ensure program performance by being created once during the first call.
 */
void fillArray(int* arr, size_t size) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(-RAND_MAX, RAND_MAX);

    for (size_t i = 0; i < size; i++) arr[i] = distrib(gen);
}

/**
 * @brief Finds indexes of all zero-value elements in the array
 * and returns them in a new array.
 * @param arr pointer to the source 1D array of integers.
 * @param size the length of the source array.
 * @param zeroValuesCount count of zero-values in source array.
 * Length of the returned array.
 * @return Pointer to the newly allocated 1D array of integers.
 * @note If the source array have no zero-value elements, nullptr is returned.
 * @note Allocated memory should be deallocated using deleteArray().
 */
int* zeroIndexesInArray(int* arr, size_t size, size_t &zeroValuesCount) {
    size_t newSize = 0;
    for (size_t i = 0; i < size; i++ ) if (arr[i] == 0) newSize++;
    
    if (newSize != 0) {
        cout << "\nКількість елементів, значення яких становить 0: " 
            << newSize << ".\n";
    }
    else {
        cout << "\nЕлементів, значення яких дорівнює 0 в масиві немає.\n";
        return nullptr;
    }

    int* newArray = new int[newSize]();
    for (size_t i = 0, j = 0; i < size; i++) {
        if (arr[i] == 0) newArray[j++] = i;
    }
    
    zeroValuesCount = newSize;
    return newArray;
}

/**
 * @brief Printing the one-dimensional array.
 * @param arr pointer to a 1D dynamic array of integers.
 * @param size array length.
 * @return Void.
 */
void printArray(int* arr, size_t size) {
    if (arr == nullptr) {
        cout << "Масиву не існує.\n";
        return;
    }
    if (size == 0) {
        cout << "Масив порожній.\n";
        return;
    }
    cout << "\n";
    for (size_t i = 0; i < size; i++) cout << arr[i] << " ";
    cout << "\n";
}

/**
 * @brief Deallocates memory used by the 1D array.
 * @param arr pointer reference to a 1D dynamic array of integers.
 * @return Void.
 */
void deleteArray(int* &arr) {
    if (arr != nullptr) {
        delete[] arr;
        arr = nullptr;
        cout << "Одновимірний динамічний масив видалено.\n";
        return;
    }
    cout << "Масиву не існує\n";
}