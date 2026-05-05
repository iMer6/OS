/*
Ввести з клавіатури рядок символів.
Обробити його у відповідності з варіантом.
Створити програмні обробки рядка.

Завдання: ввести символьну матрицю (двовимірний масив символів).
    Інвертувати її відносно середини рядку.
    Перетворити матрицю у string.
    Знайти довжину string.
*/
#include <iostream>
#include <iomanip>
#include <cstdlib> // system("cls")
#include <limits> // numeric_limits

using std::cin;
using std::cout;
using std::cerr;

/**
 * @brief Maximum matrix rows or columns size. 
 */
const int MAX_SIZE = 10;
/**
 * @brief Currently number of rows in matrix.
 */
int n;
/**
 * @brief Currently number of columns in matrix.
 */
int m;

bool validN();
bool validM();
void wrongInput();
void invalidInput(std::string word);
void fillCharMatrix(char** chMatrix);
std::string charMatrixToString (char** chMatrix);
std::string reverseStringByCenter(std::string str);
void printCharMatrix(char** chMatrix);
void deleteCharMatrix(char** &chMatrix);

int main() {
    system("cls");

    cout << "Введіть кількість рядків масиву: ";
    if (!validN()) return 1;

    cout << "Введіть кількість стовпців масиву: ";
    if (!validM()) return 1;

    char** charMatrix = new char*[n]();
    for (int i = 0; i < n; i++) charMatrix[i] = new char[m]();

    fillCharMatrix(charMatrix);

    cout << "\nМатриця:\n";
    printCharMatrix(charMatrix);

    std::string matrixInStr = charMatrixToString(charMatrix);
    cout << "\nСимвольна матриця у string: " << matrixInStr << "\n";
    cout << "Довжина матриці у string: " << n * m << "\n";

    std::string reversedMatrixInString = reverseStringByCenter(matrixInStr);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            charMatrix[i][j] = reversedMatrixInString[i * m + j];
        }
    }
    cout << "\nІнвертована матриця:\n";
    printCharMatrix(charMatrix);

    deleteCharMatrix(charMatrix);

    return 0;
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
    cerr << "\nМатриця не може мати таку кількість " << word << ".\n";
    cout << "Спробуйте ввести ціле позитивне число.\n";
}

/**
 * @brief Fills the character matrix with characters entered by the user.
 * @param chMatrix pointer to a 2D dynamic array of characters.
 * @return Void.
 */
void fillCharMatrix(char** chMatrix) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            cout << "Введіть [" << i + 1 << ", " << j + 1 << "] елемент: ";

            std::string input = "";
            while (!(cin >> input) || input.length() != 1) {
                cerr << "Помилка вводу. Спробуйте ще раз: ";
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                input = "";
            };
            chMatrix[i][j] = input[0];
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

/**
 * @brief Transform character matrix to string.
 * @param chMatrix pointer to a 2D dynamic array of characters.
 * @return String. Transformed matrix.
 */
std::string charMatrixToString (char** chMatrix) {
    std::string str = "";
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            str += chMatrix[i][j];
        }
    }
    return str;
}

/**
 * @brief Reverse string.
 * @param str input string to be reversed.
 * @return String. Reversed string.
 */
std::string reverseStringByCenter(std::string str) {
    std::string reversedString = "";
    for (int i = str.length() - 1; i >= 0; i--) reversedString += str[i];
    return reversedString;
}

/**
 * @brief Printing the character matrix algorithm.
 * @param chMatrix pointer to a 2D dynamic array of characters.
 * @return Void.
 */
void printCharMatrix(char** chMatrix) {
    for (size_t i = 0; i < n; i++) {
        cout << "| ";
        for (size_t j = 0; j < m; j++) {
            cout << chMatrix[i][j] << " ";
        }
        cout << "|\n";
    }   
}

/**
 * @brief Deallocates memory used by the matrix.
 * @param chMatrix pointer to a 2D dynamic array of characters.
 * @return Void.
 */
void deleteCharMatrix(char** &chMatrix) {
    for (int i = 0; i < n; i++) delete[] chMatrix[i];
    delete[] chMatrix;
    chMatrix = nullptr;

    cout << "\nСимвольну матрицю видалено.\n";
}