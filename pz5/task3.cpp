/*
Рядки тексту упорядкувати в порядку зростання.
Тип кодування невідомий.
*/
#include <iostream>
#include <fstream>
#include <tchar.h>

using std::cout;

const char* FILENAME = "UnknownEncoding.txt";
const size_t MAX_COUNT = 10; // max number of rows
const size_t MAX_LENGTH = 100; // max length of row

/**
 * @brief Ascending order sort
 */
void SortStrings(TCHAR* arr[], size_t count) {
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            // ASCII – strcmp
            // Unicode – wcscmp
            if (_tcscmp(arr[j], arr[j + 1]) > 0) {
                TCHAR* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    TCHAR textBuf[MAX_COUNT][MAX_LENGTH];

    TCHAR* ptrs[MAX_COUNT];
    for (size_t i = 0; i < MAX_COUNT; i++) ptrs[i] = textBuf[i];

    // ifstream works with char (ASCII) characters.
    // wifstream works with wchar_t (Unicode) characters.
    // ASCII: TCHAR – char, basic_ifstream<TCHAR> – basic_ifstream<char> == ifstream
    // Unicode: TCHAR – wchar_t, basic_ifstream<TCHAR> – basic_ifstream<wchar_t> == 
    std::basic_ifstream<TCHAR> fl(FILENAME);
    if (!fl.is_open()) {
        _tprintf(_TEXT("Помилка: Не вдалося відкрити файл %s\n"), FILENAME);
        return 1;
    }

    // Numbers of readed lines
    size_t actualCount = 0;

    while (actualCount < MAX_COUNT && fl.getline(ptrs[actualCount], MAX_LENGTH)) {
        if (_tcslen(ptrs[actualCount]) > 0) actualCount++;
    }
    fl.close();

    if (actualCount == 0) {
        _tprintf(_TEXT("Помилка: Файл порожній або не містить даних.\n"));
        return 1;
    }
    fl.close();

    cout << "Рядки з файлу:\n";
    for (size_t i = 0; i < MAX_COUNT; i++) _tprintf(_TEXT("%s\n"), ptrs[i]);

    SortStrings(ptrs, actualCount);

    cout << "Відсортовані рядки:\n";
    for (size_t i = 0; i < MAX_COUNT; i++) _tprintf(_TEXT("%s\n"), ptrs[i]);
    return 0;
}