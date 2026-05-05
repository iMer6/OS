/*
Ввести з клавіатури рядок символів.
Обробити його у відповідності з варіантом.
Створити програмні обробки рядка.

Завдання: видалити кожне парне слово з рядка.
*/
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::cerr;

std::string substr(const std::string& str, size_t startIndex = 0, int n = -1);

int main() {
    // Reading input text.
    std::string txt;
    cout << "Введіть рядок символів: ";
    std::getline(cin, txt);

    if (txt.empty()) {
        cout << "\nРядок порожній.";
        return 0;
    }

    // Counting words and checking for non-space characters.
    size_t wordCount = 1;
    bool isSymbol = false;
    for (size_t i = 0; i < txt.length() - 1; i++) {
        if (txt[i] == ' ' && txt[i + 1] != ' ') wordCount++;
        if (txt[i] != ' ') isSymbol = true;
    }

    if (!isSymbol) {
        cout << "\nУ рядку немає символів.";
        return 0;
    }

    // Spliting the text into an array of words.
    std::string* words = new std::string[wordCount]();
    for (size_t i = 0, j = 0; i < txt.length(); i++) {
        if (txt[0] == ' ') {
            txt = substr(txt, 1, -1);
            i = 0;
            continue;
        }
        if (txt[i] == ' ' && i != 0) {
            words[j++] = substr(txt, 0, i);
            txt = substr(txt, i + 1, -1);
            i = 0;
        }
    }
    words[wordCount - 1] = txt;

    // Removing every even words in text. 
    txt = "";
    for (size_t i = 0; i < wordCount; i += 2) txt += words[i] + " ";
    
    // Output of the result.
    cout << "\n" << txt;

    delete[] words;
    words = nullptr;
    return 0;
}

/**
 * @brief Construst and return a new string.
 * @param str entry string.
 * @param startIndex index of first character in string. Must be in [0, string length).
 * @param n number of characters in substring. Must be in [0, remainder].
 * @return The new string.
 * @note Parameter startIndex optional. Default value is 0 (str start).
 * @note Parameter n optional. Default value is remainder.
*/
std::string substr(const std::string& str, size_t startIndex, int n) {
    size_t len = str.length();

    if (startIndex < 0 || startIndex >= len) {
        cerr << "Значення startIndex поза межами рядка.";
        return str;
    }

    if (n == -1 || startIndex + n > len) n = len - startIndex;
    if (n < 0) { 
        cerr << "Неправильна кількість символів.";
        return str;
    }
    
    std::string newStr;
    for (int i = 0; i < n; i++) newStr+= str[startIndex + i];
    return newStr;
}