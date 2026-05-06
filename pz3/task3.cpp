/*
1. Сформувати динамічний масив з елементів структурного типу.
Структурний тип визначений у варіанті.
2. Роздрукувати сформований масив.
3. Виконати пошук елементів у масиві, що задовольняють заданій у варіанті умові
і сформувати з них новий масив.
4. Роздрукувати отриманий масив.
5. Сформувати динамічний масив, що складається з динамічних рядків.
6. Роздрукувати сформований масив.
7. Виконати обробки цього масиву.
8. Роздрукувати отриманий масив.

> Критерій для пошуку в масиві структур: вік менше 18.
> Завдання для обробки масиву рядків: видалити рядок з номерок K.

Для виділення пам'яті під масиви використовувати операцію new,
для видалення масивів з пам'яті – операцію delete.

Для формування і друку структур написати окремі функції.

Для виділення пам'яті, заповнення масивів, пошуку заданих елементів
написати окремі функції.
У функції main повинні бути розміщені тільки описи змінних
і звернення до відповідних функцій.

Якщо в масиві відсутні елементи, відповідні критерію пошуку,
то повинно бути виведено повідомлення про те, що необхідні елементи не знайдені.

При видалення рядків передбачити ситуації, в яких буде виконуватися
спроба видалення рядка з пустого масиву або кількість елементів, що видаляються
буде перевищувати кількість наявних рядків.
У цьому випадку повинно бути виведено повідомлення про помилку.
*/
#include <iostream>
#include <cstdlib> // system("cls")
#include <iomanip> // setw()
#include <random> // mt19937, uniform_int_distribution
#include <limits> // std::numeric_limits
#include <string.h> // strcpy()

using std::cin;
using std::cout;
using std::cerr;
using std::setw;

struct Person {
    char* name;
    unsigned int age;
};

constexpr unsigned int MAX_NAME_LENGTH = 20;
constexpr unsigned int MAX_AGE = 200;
constexpr unsigned int PERSON_COUNT = 3;
constexpr unsigned int ARRAY_SIZE = 5;

Person makePerson();
Person* filterPeople(Person*, unsigned int, unsigned int&);
void printHeader();
void printPerson(const Person&);
void deletePerson(Person*&);
int** createArray(size_t);
void removeRowByNumber(int**&, size_t&, size_t&);
void fillArray(int**, int, int);
void printArray(int**, size_t, size_t);
void deleteArray(int**&, size_t);

int main() {
    // 1.
    Person* prsn = new Person[PERSON_COUNT]();
    for (size_t i = 0; i < PERSON_COUNT; i++) prsn[i] = makePerson();
    system("cls");
    // 2.
    cout << "Динамічний масив структурного типу:\n";
    printHeader();
    for (size_t i = 0; i < PERSON_COUNT; i++) printPerson(prsn[i]);
    // 3.
    unsigned int underEighteenCount = 0;
    Person* filteredPrsn = filterPeople(prsn, PERSON_COUNT, underEighteenCount);
    // 4.
    cout << "\nВідфільтрований масив:\n";
    printHeader();
    for (size_t i = 0; i < underEighteenCount; i++) printPerson(filteredPrsn[i]);
    
    deletePerson(prsn);
    deletePerson(filteredPrsn);
    //
    cout << "\n\nНатисніть Enter, щоб продовжити...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    system("cls");
    // 5.
    size_t ARRAY_SIZE = 5;
    int** arr = createArray(ARRAY_SIZE);
    fillArray(arr, ARRAY_SIZE, ARRAY_SIZE);
    // 6.
    cout << "Динамічний масив, що складається з динамічних рядків:\n";
    printArray(arr, ARRAY_SIZE, ARRAY_SIZE);
    // 7.
    size_t k = 0;
    removeRowByNumber(arr, ARRAY_SIZE, k);
    // 8.
    cout << "\nОброблений масив (видалено рядок " << k + 1 << "):\n";
    printArray(arr, ARRAY_SIZE, ARRAY_SIZE + 1);

    deleteArray(arr, ARRAY_SIZE);
    return 0;
}

/**
 * @brief Prompts the user for Person's name and age, validate it 
 * and create object of Person.
 * @return Created object of struct Person with entered data.
 * @note Uses function @a strcpy(). 
 * Needs to include library @a <string.h>.
 * @note Uses function @a std::string::c_str(). 
 * Needs to include library @a <string>.
 * @note Name of a Person must be no longer than 20 characters.
 * @note Age of a Person must be no more than 200. People don't live that long.
 */
Person makePerson() {
    std::string name;
    cout << "Ім'я: ";
    while(!(cin >> name) || name.length() == 0 || name.length() > 20) {
        cerr << "Помилка! Введіть ім'я до " << MAX_NAME_LENGTH << " символів: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    unsigned int age;
    cout << "Вік: ";
    while (!(cin >> age) || age > MAX_AGE) {
        cerr << "\nПомилка! Введіть ціле число від 0 до " << MAX_AGE << ": ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    Person p;
    p.name = new char[name.length() + 1];
    strcpy(p.name, name.std::string::c_str());
    p.age = age;
    return p;
}

/**
 * @brief Filters the array of struct Person where field "Age" < 18.
 * @param pArray pointer to an array of Person's.
 * @param size length of @a pArray.
 * @param underEighteenCount count of objects in an array with age < 18.
 * @return Pointer to the newly allocated array of Persons.
 * @note If memory for an array has not yet been allocated, returns nullptr.
 */
Person* filterPeople(
    Person* pArray, 
    unsigned int size, 
    unsigned int& underEighteenCount
) {
    if (pArray == nullptr) {
        cout << "\nМасиву структурного типу не існує.\n";
        return nullptr;
    }

    for (size_t i = 0; i < size; i++) {
        if (pArray[i].age < 18) underEighteenCount++;
    }

    Person* filteredPeople = new Person[underEighteenCount];
    for (size_t i = 0, j = 0; i < size; i++) {
        if (pArray[i].age < 18) filteredPeople[j++] = pArray[i];
    }

    return filteredPeople;
}

/**
 * @brief Prints the header of the table with columns "Name" and "Age".
 */
void printHeader() {
    cout << "Name" << setw(19) << "| " << "Age" << setw(3) << "\n";
    cout << std::string(20, '-') << "-|----\n";
}

/**
 * @brief Prints one row of the table with values "Name" and "Age".
 */
void printPerson(const Person& p) {
    cout << setw(20) << p.name << " | " << setw(3) << p.age << "\n";
}

/**
 * @brief Deallocates memory used by the array of the struct Person.
 * @param pArray pointer reference to the dynamic array of struct Person.
 */
void deletePerson(Person* &pArray) {
    if (pArray == nullptr) {
        cout << "\nМасиву Person не існує.";
        return;
    }
    delete[] pArray;
    pArray = nullptr;
    cout << "\nМасив структурного типу видалено.";
}

/**
 * @brief Allocates memory for a 2D dynamic array of integers.
 * @param size number of rows and columns in the array (square matrix).
 * @return Pointer to the newly allocated 2D array of integers.
 * @note Allocated memory should be deallocated using deleteArray().
 */
int** createArray(size_t size) {
    int** array = new int*[size];
    for (size_t i = 0; i < size; i++) array[i] = new int[size]();
    return array;
}

/**
 * @brief Prompts user for row number to remove, validates the input and
 * deletes a specific row in the array.
 * @param arr pointer reference to a 2D dynamic array of integers.
 * @param size reference number of rows in the array.
 * @param k row number in the array to remove.
 * @note Number of row must be in range [1, size]. 
 */
void removeRowByNumber(int** &arr, size_t &size, size_t &k) {
    cout << "Введіть номер рядку, який потрібно видалити: ";
    while (!(cin >> k) || k == 0 || k > size) {
        cerr << "Помилка! Введіть ціле число від 1 до " << size << ": ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   
    }

    delete[] arr[--k];
    for (size_t i = k; i < size; i++) arr[i] = arr[i + 1];
    arr[--size] = nullptr; 
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
void fillArray(int** arr, int n, int m) {
    if (!arr) return;

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(-RAND_MAX, RAND_MAX);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = distrib(gen);
        }
    }
}

/**
 * @brief Prints the 2D array.
 * @param arr pointer to a 2D dynamic array of integers.
 * @param n number of rows in the array.
 * @param m number of columns in the array.
 */
void printArray(int** arr, size_t n, size_t m) {
    if (!arr) return;

    for (size_t i = 0; i < n; i++) {
        cout << "| ";
        for (size_t j = 0; j < m; j++) {
            cout << setw(6) << arr[i][j] << " ";
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
    if (!arr) return;

    for (size_t i = 0; i < n; i++) delete[] arr[i];
    delete[] arr;
    arr = nullptr;

    cout << "\nДвовимірний динамічний масив видалено.\n";
}