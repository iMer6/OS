/*
Скласти функція для порівняння двох каталогів.
Каталоги регістронезалежні і можуть закінчуватися символом \.
Цей символ може бути відсутнім.
*/
#include <iostream>
#include <cstring> // strcpy, strlen, _stricmp
#include <windows.h> // MAX_PATH

using std::cout;

int CompareDirectories(const char*, const char*);
void printResult(const char*, const char*, int);

int main() {
    const char* dirA = "C:\\Program Files";
    const char* dirB = "c:\\program files\\";
    const char* dirC = "C:\\Windows";

    int res1 = CompareDirectories(dirA, dirB);
    printResult(dirA, dirB, res1);

    int res2 = CompareDirectories(dirA, dirC);
    printResult(dirA, dirC, res2);

    return 0;
}

int CompareDirectories(const char* path1, const char* path2) {
    if (!path1 || !path2) return -1; // Param error

    // MAX_PATH = 260 characters
    char buf1[MAX_PATH];
    char buf2[MAX_PATH];

    // strcpy_s – copy path
    strcpy_s(buf1, path1);
    strcpy_s(buf2, path2);
    
    size_t len1 = strlen(buf1);
    size_t len2 = strlen(buf2);

    // Replace \ at the end of path with \0
    if (len1 > 0 && buf1[len1 - 1] == '\\') buf1[len1 - 1] = '\0';
    if (len2 > 0 && buf2[len2 - 1] == '\\') buf2[len2 - 1] = '\0';
    
    // _stricmp – case-insensitive comparison
    if (_stricmp(buf1, buf2) == 0) return 1; // Equal
    return 0; // Not equal
}

void printResult(const char* path1, const char* path2, int res) {
    if (res == -1) {
        cout << "Помилка! Неправильний(-і) параметр(-и)\n";
    } else {
        cout << "Каталоги \"" << path1 << "\" та \"" << path2 << "\" "
            << (res ? "однакові" : "різні") << "\n";
    }
}