//
// Знайти мін. парне число головної діагоналі матриці.
// Формування, друк і обробка двовимірних масивів.
// Реалізувати масиви як псевдодінамічні.
// Формування масивів виконати з використанням ГСЧ.
// В масиви записувати і +, і - числа.
//
#include <iostream>
#include <iomanip> // setw()
#include <cstdlib> // system("cls"), RAND_MAX 
#include <random>

using namespace std;

int wrongInput();
int invalidInput(char* word);
int** createMatrix(int n, int m);
void fillMatrix(int** matrix, int n, int m);
void minEvenValueMainD(int** matrix, int n);
void printMatrix(int** matrix, int n, int m);
void deleteMatrix(int** matrix, int n);

int main() {
    system("cls");

    cout << "Введіть кількість рядків (n) матриці: ";
    int n; // rows
    if (!(cin >> n)) return wrongInput();
    if (n <= 0) return invalidInput("рядків");

    cout << "Введіть кількість рядків (m) матриці: ";
    int m; // columns
    if (!(cin >> m)) return wrongInput();
    if (m <= 0) return invalidInput("стовпців");

    int **matrix = createMatrix(n, m);
    
    fillMatrix(matrix, n, m);

    minEvenValueMainD(matrix, min(n, m));

    printMatrix(matrix, n, m);

    deleteMatrix(matrix, n);
    return 0;
}

/**
 * @brief Displays a message about error input.
 * @return Error code (1).
 */
int wrongInput() {
    cerr << "\nНеправильний ввід.\n";
    cout << "Спробуйте ціле позитивне число.\n";
    return 1;
}

/**
 * @brief Displays a message about invalid input.
 * @return Error code (1).
 */
int invalidInput(char* word) {
    cerr << "\nМатриця не може мати таку кількість "<< word <<".\n";
    cout << "Спробуйте ввести ціле позитивне число.\n";
    return 1;
}

/**
 * @brief Allocates memory for a 2D dynamic array (matrix).
 * @param n number of rows in the matrix.
 * @param m number of columns in the matrix.
 * @return Pointer to the newly allocated 2D array of integers.
 * @note Allocated memory should be deallocated using deleteMatrix().
 */
int** createMatrix(int n, int m) {
    int **matrix = new int *[n];
    for (int i = 0; i < n; i++) matrix[i] = new int[m];
    return matrix;
}

/**
 * @brief Fills the matrix with random integers from -RAND_MAX to RAND_MAX.
 * @param matrix pointer to a 2D dynamic array of integers.
 * @param n number of rows in the matrix.
 * @param m number of columns in the matrix.
 * @return Void.
 * @note Uses the Mersenne Twister (mt19937) algorithm for random number generation.
 * https://en.wikipedia.org/wiki/Mersenne_Twister.
 * @note The generator is static to ensure program performance by being created once during the first call.
 */
void fillMatrix(int** matrix, int n, int m) {
    //
    // {} – create the object
    // () – calls object
    //
    static mt19937 gen(random_device{}());
    uniform_int_distribution<> distrib(-RAND_MAX, RAND_MAX);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = distrib(gen);
        }
    }
}

/**
 * @brief Find the minimal even value on the matrix main diagonal.
 * @param matrix pointer to a 2D dynamic array of integers.
 * @param n number of elements of main diagonal (dimension).
 * @return Void. The result is printed to the console.
 * @note If no even numbers are found, a corresponding message is displayed.
 */
void minEvenValueMainD(int** matrix, int n) {
    int minEl = RAND_MAX + 1;
    for (int i = 0; i < n; i++) {
        if (minEl > matrix[i][i] && matrix[i][i] % 2 == 0) {
            minEl = matrix[i][i];
        }
    }
    if (minEl != RAND_MAX + 1) {
        cout << "\nМінімальне парне число головної діагоналі: " 
            << minEl << "\n";
    } else { cout << "\nНа головній діагоналі немає парних чисел.\n"; }

}

/**
 * @brief Printing the 2D array algorithm.
 * @param matrix pointer to a 2D dynamic array of integers.
 * @param n number of rows in the matrix.
 * @param m number of columns in the matrix.
 * @return Void.
 */
void printMatrix(int** matrix, int n, int m) {
    cout << "\n";
    for (int i = 0; i < n; i++) {
        cout << "| ";
        for (int j = 0; j < m; j++) {
            cout << setw(6) << matrix[i][j] << " ";
        }
        cout << "|\n";
    }
    cout << "\n";
}

/**
 * @brief Deallocates memory used by the 2D array.
 * @param matrix pointer to a 2D dynamic array of integers.
 * @param n number of rows in the matrix.
 */
void deleteMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) delete[] matrix[i];
    delete[] matrix;

    cout << "\nМатрицю видалено.\n";
}