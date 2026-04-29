// Дана послідовність цілих позитичних чисел.
// Утворити нову послідовність, викинувши з вихідного ті члени,
// які дорівнюють (однакові).
#include <iostream>
#include <limits>

using std::cin;
using std::cout;
using std::cerr;

const int MAX_SIZE = 100;
int size = 0;

inline bool isEmpty();
void fillArray(int* arr);
int* uniqueElements(int* arr);
void printArray(int* arr);
void deleteArray(int* &arr);

int main() {
    cout << "\nВведіть розмір масиву від 1 до " << MAX_SIZE << ": ";
    if (!(cin >> size) || size <= 0 || size > MAX_SIZE)
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        cerr << "\nВведено некоректне значення для розміру масиву.\n";
        return 1;
    }
    int* arr = new int[size]();

    fillArray(arr);
    printArray(arr);  

    arr = uniqueElements(arr);
    printArray(arr);

    deleteArray(arr);
    return 0;
}

inline bool isEmpty() { return size == 0; }

void fillArray(int* arr) {
    cout << "\nВсі числа в масиві повинні бути цілими позитивними.\n";
    for (int i = 0; i < size; i++) {
        cout <<"Введіть " << i + 1 << " число: ";
        int newEl;
        if (!(cin >> newEl) || newEl <= 0) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            --i;
            cerr << "Введено некоректне значення елементу масиву.\n";
            continue;
        }
        arr[i] = newEl;
    }
}

int* uniqueElements(int* arr) {
    if (arr) {
        cout << "\nПослідовності не існує.\n";
        return;
    }
    if (isEmpty()) {
        cout << "\nПослідовність порожня.\n";
        return;
    }

    int uniqueCount = 0;
    for (int i = 0; i < size; i++) {
        int count = 0;

        for (int j = 0; j < size; j++) {
            if (arr[i] == arr[j]) { count++; }
        }
        if (count == 1) { arr[uniqueCount++] = arr[i]; }
    }

    int* newArr = new int[uniqueCount]();
    for (int i = 0; i < uniqueCount; i++) { newArr[i] = arr[i]; }
    size = uniqueCount;

    return newArr;
}

/**
 * @brief Printing the one-dimensional array algorithm.
 * @param arr pointer to a 1D dynamic array of integers.
 * @return Void.
 */
void printArray(int* arr) {
    if (arr) {
        cout << "\nПослідовності не існує.\n";
        return;
    }
    if (isEmpty()) {
        cout << "\nПослідовність порожня.\n";
        return;
    }
    cout << "\n";
    for (int i = 0; i < size; i++) { cout << arr[i] << " "; }
    cout << "\n";
}

/**
 * @brief Deallocates memory used by the 1D array.
 * @param arr pointer link to a 1D dynamic array of integers.
 * @return Void.
 */
void deleteArray(int* &arr) {
    if (arr) {
        delete[] arr;
        arr = nullptr;
        size = 0;

        cout << "\nПослідовність видалено.\n";
        return;
    }
    cout << "\nПослідовності цілих позитивних чисел не існує.\n";
}