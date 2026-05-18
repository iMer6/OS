/*
Рядки тексту упорядкувати в порядку зростання.
Символи цих рядків задані в ASCII або в Unicode.
*/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cwchar>

using std::cout;

const size_t MAX_COUNT = 5; // max number of rows
const size_t MAX_LENGTH = 100; // max length of row

/**
 * @brief Ascending ASCII sort
 */
void SortStringsASCII(char* arr[], size_t count) {
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                char* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Ascending Unicode sort
 */
void SortStringsUnicode(wchar_t* arr[], size_t count) {
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            if (wcscmp(arr[j], arr[j + 1]) > 0) {
                wchar_t* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    system("cls");
    // ASCII
    std::ifstream aFile("ASCII.txt", std::ios::in);
    if (!aFile.is_open()) {
        std::cerr << "Не вдалося відкрити файл ASCII.txt\n";
        return 1;
    }

    char aBuf[MAX_COUNT][MAX_LENGTH];
    char* aPointers[MAX_COUNT];
    size_t aCount = 0;

    while (aCount < MAX_COUNT && aFile.getline(aBuf[aCount], MAX_LENGTH)) {
        aPointers[aCount] = aBuf[aCount];
        aCount++;
    }
    aFile.close();

    cout << "ASCII список з файлу:\n";
    for (size_t i = 0; i < aCount; i++) cout << aPointers[i] << "\n";

    SortStringsASCII(aPointers, aCount);

    cout << "\nВідсортований ASCII список:\n";
    for (size_t i = 0; i < aCount; i++) cout << aPointers[i] << "\n";

    cout << "\nНатисніть Enter...";
    getchar();
    system("cls");

    // Unicode
    std::ifstream uFile("Unicode.txt");
    if (!uFile.is_open()) {
        std::cerr << "Не вдалося відкрити файл unicode.txt\n";
        return 1;
    }

    // Unicode matrix
    wchar_t uBuf[MAX_COUNT][MAX_LENGTH];
    wchar_t* uPointers[MAX_COUNT];
    size_t uCount = 0;

    // byte buffer
    char tempCharBuf[MAX_LENGTH];

    while (uCount < MAX_COUNT && uFile.getline(tempCharBuf, MAX_LENGTH)) {
        // File UTF-8 (Unicode) –> tempCharBuf –> mbstowcs() –> uBuf
        // Sequence of multibyte characters –> Array of wide characters (wchar_t)
        // MultiByte String TO Wide Character String
        mbstowcs(uBuf[uCount], tempCharBuf, MAX_LENGTH);
        
        uPointers[uCount] = uBuf[uCount];
        uCount++;
    }
    uFile.close();

    cout << "Unicode список з файлу:\n";
    for (size_t i = 0; i < uCount; i++) std::wcout << uPointers[i] << "\n";

    SortStringsUnicode(uPointers, uCount);

    cout << "\nВідсортований Unicode список:\n";
    for (size_t i = 0; i < uCount; i++) std::wcout << uPointers[i] << "\n";

    return 0;
}