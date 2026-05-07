#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;

const int MAX_SIZE = 100;

void deleteMinElement(int arr[], int &size);
void addElements(int arr[], int &size);
void swapElements(int arr[], int size);
void searchElByKey(int arr[], int &size);
void printArray(int arr[], int size);

int main() {
    cout << "\nВведіть розмір масиву від 1 до " << MAX_SIZE << ": ";
    int size = 0;
    if (!(cin >> size) || (size <= 0 || size > MAX_SIZE))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cerr << "\nВведено некоректне значення для розміру масиву.\n";
        return 1;
    }
    int arr[MAX_SIZE];
    //
    // Заповнення масиву псевдо-випадковими числами
    //
    for (int i = 0; i < size; i++) {
        arr[i] = rand() - RAND_MAX / 2;
    }
    //
    // Основний цикл
    //
    while (true) {
        string action;
        
        while (true) {
            cout << "\nМожливі дії:\n";
            cout << "1. Видалити мінімальний елемент.\n";
            cout << "2. Додати N елементів, починаючи з номеру K.\n";
            cout << "3. Поміняти місцями елементи з парними і непарними номерами.\n";
            cout << "4. Знайти елемент із заданим ключем (значенням).\n";
            cout << "5. Роздрукувати масив.\n";
            cout << "6. Вийти (завершити програму).\n";
            cout << "\nВиберіть дію з масивом: ";
            
            cin >> action;
            if (action.length() == 1 && action[0] >= '1' && action[0] <= '6') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                system("cls");
                cerr << "\nНевірна дія.\n";
            }
        }

        system("cls");

        switch (action[0]) {
            case '1':
                deleteMinElement(arr, size);
                break;
            case '2':
                addElements(arr, size);
                break;
            case '3':
                swapElements(arr, size);
                break;
            case '4':
                searchElByKey(arr, size);
                break;
            case '5':
                printArray(arr, size);
                break;
            case '6':
                return 0;
                break;
            default:
                cout << "\nДії " << action << " не існує.";
        }
    }
}

void deleteMinElement(int arr[], int &size) {
    if (size == 0) {
        cout << "\nМасив порожній, немає елементів для видалення.\n";
        return;
    }
    int minIndex = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[minIndex]) { minIndex = i; }
    }
    int minEl = arr[minIndex];
    //
    // Зсув елементів після видаленого
    //
    for (int i = minIndex; i < size - 1; i++) { arr[i] = arr[i + 1]; }
    size--;

    cout << "\nЕлемент " << minEl << " було видалено.\n";
}

void addElements(int arr[], int &size) {
    //
    // Введення кількості елементів та позиції K
    //
    int n;
    while (true) {
        printArray(arr, size);

        cout << "\nСкільки елементів потрібно додати? ";
        if (!(cin >> n) || n <= 0 || n + size > MAX_SIZE) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cerr << "\nВведено некоректне значення.\n";
            cerr << "Кількість елементів повинна бути в діапазоні від 1 до "
                << MAX_SIZE - size << ".\n";
        } else { break; }
    }

    int k;
    while (true) {
        cout << "\nВведіть позицію початку додавання елементів (від 0 до " 
            << size << "): ";
        if (!(cin >> k) || k < 0 || k > size) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cerr << "\nВведено некоректне значення.\n";
            cerr << "Позиція повинна бути від 0 до " << size << ".\n";
        } else { break; }
    }
    //
    // Додавання псевдо-випадкових елементів елементів
    //
    int newSize = size + n;
    int tempArr[newSize];

    for (int i = 0, j = 0; i < newSize; i++) {
        if (i < k) { tempArr[i] = arr[j++];
        }
        else if (i == k) {
            for (int ii = k; ii < k + n; ii++) { 
                tempArr[ii] = rand() - RAND_MAX / 2;
            }
            i += n - 1;
        }
        else { tempArr[i] = arr[j++]; }
    }
    //
    // Прохід по тимчасовому масиву (tempArr) для додавання в нього значень зі старого масиву (arr).
    // Якщо поточна ітерація не дійшла до позиції, з якої будуть додаватися нові елементи (i < k),
    //      то просто додаємо зі старого. tempArr[index] = arr[index].
    // Якщо ітерація дорівнює індексу, з якого будуть вставлятися елементи (i == k),
    //      то проходимося по потрібній кількості елементів для додавання, 
    //      після чого додаємо кількість доданих чисел до індексу ітерацій.
    // Якщо потрібні елементи додалися (i > k + n), продовжуємо додавати елементи зі старого масиву.
    //

    //
    // Зміна посилань (перепризначення вказівника)
    //
    arr = tempArr;
    size = newSize;

    clog << "\nЕлементи успішно додано.\n";
}

void swapElements(int arr[], int size) {
    if (size == 0) {
        cout << "\nМасив порожній. Переставляти нічого.\n";
        return;
    }
    //
    // XOR swap
    //
    for (int i = 0; i < size - 1; i += 2) {
        arr[i] ^= arr[i + 1];
        arr[i + 1] ^= arr[i];
        arr[i] ^= arr[i + 1];
    }

    cout << "\nПарні і непарні елементи масиву було змінено місцями.\n";
}

void searchElByKey(int arr[], int &size) {
    if (size == 0) {
        cout << "\nМасив порожній. Знайти елемент не можна.\n";
        return;
    }

    cout << "\nВведіть значення числа, яке потрібно знайти: ";
    int key;
    
    if (!(cin >> key) || key < 0 || key >= RAND_MAX) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cerr << "\nВведено некоректне значення. Спробуйте від " 
            << -RAND_MAX / 2 << " до " << RAND_MAX / 2 << "\n";

        return;
    }

    for (int i = 0; i < size; i++) {
        if (arr[i] == key) {
            cout << "\nЕлемент " << key << " знайдено на позиції " << i << ".\n";
            cout << "Виконано порівнянь: " << i + 1 << "\n";
            return;
        }
    }
    cout << "\nЕлемента " << key << " у масиві немає\n";
    cout << "Виконано порівнянь: " << size << "\n";
}

void printArray(int arr[], int size) {
    if (size == 0) {
        cout << "\nМасив порожній.\n";
        return;
    }
    cout << "\n";
    for (int i = 0; i < size; i++) { cout << arr[i] << " "; }
    cout << "\n";
}
