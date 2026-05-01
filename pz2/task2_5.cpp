/*
Ввести з клавіатури рядок символів.
Обробити його у відповідності з варіантом.
Створити програмні обробки рядка.

Завдання 1: видалити кожне парне слово з рядка.
Завдання 2: ввести символьну матрицю (двовимірний масив символів).
    Інвертувати її відносно середини рядку.
    Перетворити матрицю у string.
    Знайти довжину string.
*/
#include <iostream>
#include <string>
#include <stdexcept>

using std::cin;
using std::cout;
using std::cerr;

std::string substr(const std::string& str, size_t startIndex, int n);

int main() {
    std::string txt;
    cout << "Введіть рядок символів: ";
    std::getline(cin, txt);
    int wordCount = 1;
    for (char ch : txt) { if (ch == ' ') wordCount++; }

    std::string* words = new std::string[wordCount]();
    for (int i = 0, j = 0; i < txt.length(); i++) {
        if (txt[i] == ' ' && i == 0) continue;
        if (txt[i] == ' ' && i != 0) {
            words[j++] = substr(txt, 0, i);
            txt = substr(txt, i + 1, -1);
            i = 0;
        }
    }
    words[wordCount - 1] = txt;

    for (int i = 0; i < wordCount; i++) { cout << words[i] << " "; }

    cout << "\nКількість слів у рядку: " << wordCount;

    return 0;
}

/**
 * @brief Construst and return a new string.
 * @param str – entry string.
 * @param startIndex – index of first character in string (default 0).
 * @param n – number of characters in substring (default remainder).
 * @return The new string.
 * @throw std::out_of_range if startIndex < 0 or startIndex >= string length.
 * @throw std::out_of_range if n < 0 or n >= string length - startIndex.
 * @note Parameter startIndex optional. Default value is 0 (str start).
 * @note Parameter n optional. Default value is remainder.
*/
std::string substr(const std::string& str, size_t startIndex = 0, int n = -1) {
    size_t len = str.length();

    if (startIndex < 0 || startIndex >= len) {
        throw std::out_of_range("Значення startIndex поза межами рядка.");
    }

    if (n == -1 || startIndex + n > len) n = len - startIndex;
    if (n < 0) throw std::out_of_range("Неправильна кількість символів.");
    
    std::string newStr;
    for (int i = 0; i < n; i++) newStr+= str[startIndex + i];
    return newStr;
}