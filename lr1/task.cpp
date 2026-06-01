/*
1. Створити консольний додаток для C++ програм.

2. Скласти оператори для програмної перевірки типу кодування,
заданого за замовчуванням (визначити довжину в байтах типу TCHAR).

3. Визначити тип кодування по заданих макросах в командному рядку.
Для цього визначити командний рядок для трансляції програми.

Усі змінні, які задаються при трансляції, задаються параметром \D.
Якщо визначено змінну UNICODE, то використовується режим UNICODE,
якщо така змінна не задана, то використовується режим ASCII.

4. Перемикнути режим задання символу на протилежний.
Для перемикання режиму з UNICODE в режим ASCII:
    Свойства конфигурации
        –> Общие
        –> Набор символов
        –> Использовать многобайтовую кодировку

Для перемикання з режиму ASCII в режим UNICODE:
    Свойства конфигурации
        –> Общие
        –> Набор символов
        –> Использовать набор символов Юникода

5. Після перемикання режиму знову перевірити тип символу за замовчуванням
та командний рядок.

Після трансляції

6. Задати ПІБ членів своєї сім'ї в ASCII та вивести задані значення.
Для виведення букв кирилиці необхідно встановити локальні режими.

Підключення додаткових файлів заголовків треба завжди виконувати після рядка
#include "pch.h"

В разі використання української мови буква І(і) виводиться некоректно, 
тому замість цієї букви треба задавати англійську букву I.

7. Перевести задані рядки в UNICODE за допомогою функції (MultiByteToWideChar).

8. Вивести отриманий масив.
Перевірити можливість виведення кожним з 3-х способів:
- функція _tprintf, якщо встановлено локальний режим;
- потік wcout, якщо встановлено локальний режим;
- функція MessageBox (незалежно від встановленого режима).

9. Виконати упорядкування масиву рядків, заданих в UNICODE.
Для сортування використовувати універсальну стандартну функцію qsort
та шаблонну функцію sort (для отримання найвищої оцінки).

10. Виконати зворотне перетворення масиву з UNICODE в ASCII.

11. Вивести отриманий результат.

12. Завдання для отримання найвищої оцінки:
Задано текстовий файл. Незалежно від способу кодування символів в цьому файлі,
переставити ці символи в зворотному порядку.
Для роботи з файлом можна використовувати довільні засоби роботи з файлами в C++,
які треба вивчити самостійно.
*/

// pch.h – Visual Stiduo internal technology
// g++ by default doesn't know what pch.h it is 
// #include "pch.h"


#include <iostream>
#include <tchar.h> // generic types and macros  
#include <locale.h> // localize console
#include <windows.h> // IsTextUnicode, MultiByteToWideChar, MessageBox
// #include <winbase.h> // IsTextUnicode (include windows.h)
// #include <stringapiset.h> // MultiByteToWideChar (include windows.h)
// #include <winuser.h> // MessageBox (include windows.h)
#include <cstdlib> // qsort
#include <algorithm> // std::sort, std::reverse
#include <cstdio> // fopen, fread, fwrite

using std::cout;
using std::wcout;

constexpr size_t MAX_FILE_SIZE = 65536; // max static file buffer size
constexpr size_t MAX_PIB_LENGTH = 100;

// File reverse on static arrays
void ReverseAnyFile(const TCHAR* inputPath, const TCHAR* outputPath) {
    // Open file in text mode ("rt") to read bytes
    // Universal _tfopen works everywhere
    // Windows automatically replace \r\n with \n
    FILE* inFile = _tfopen(inputPath, _T("rt"));
    if (inFile == NULL) {
        wcout << L"Помилка: Не вдалося відкрити вхiдний файл!\n";
        return;
    }

    // Static array (buffer) for reading file
    char staticBuffer[MAX_FILE_SIZE];

    // Read data in a static array
    size_t bytesRead = fread(
        staticBuffer, // data storage location
        1, // size of each object (bytes)
        MAX_FILE_SIZE - 2, // the number of the objects to be read
            // -2 for two bytes \0
        inFile // input file stream to read from (where to read)
    );
    fclose(inFile);

    if (bytesRead == 0) {
        wcout << L"Файл порожнiй або виникла помилка читання.\n";
        return;
    }

    // \0 for correct work of IsTextUnicode
    // Wide \0 (L'\0') consists of 2 bytes (0x00 0x00)
    staticBuffer[bytesRead] = '\0'; // first byte
    staticBuffer[bytesRead + 1] = '\0'; // second byte

    // Whether a buffer contains Unicode text
    bool isUnicode = IsTextUnicode(
        staticBuffer, // pointer to the input buffer to examine
        static_cast<int>(bytesRead), // size of the input buffer (bytes)
        nullptr // use all available tests to evaluate the buffer
    );

    // Open file in text mode ("wt") to write bytes
    // Windows automatically replace \n back to \r\n
    FILE* outFile = _tfopen(outputPath, _T("wt"));
    if (outFile == NULL) return;

    if (isUnicode) {
        wcout << L"Тип кодування файлу: Unicode\n";

        // Static buffer as wchar_t array (step by 2 bytes)
        wchar_t* wData = reinterpret_cast<wchar_t*>(staticBuffer);
        // In text mode, the number of characters is counted from the bytes actually read.
        size_t wCharsCount = bytesRead / sizeof(wchar_t);

        // Process BOM marker if it is at the beginning of file
        // BOM – Byte Order Mark
        size_t startIdx = 0;
        const wchar_t BOM = 0xFEFF;
        if (wData != nullptr && wCharsCount > 0 && wData[0] == BOM) {
            fwrite(
                &BOM, // pointer to the data to be written
                sizeof(wchar_t), // size of each object (bytes)
                1, // the number of the objects to be written
                outFile // output file stream to write to (where to write)
            );
            startIdx = 1; // skip BOM when reversing
        }

        // Manually reverse elements of a static wchar_t array
        std::reverse(wData + startIdx, wData + wCharsCount);

        // Write the result in a file
        fwrite(
            wData + startIdx, // pointer to the first object in the array to be written
            sizeof(wchar_t), // size of each object (bytes)
            wCharsCount - startIdx, // the number of the objects to be written
            outFile // output file stream to write to (where to write)
        );
    } else {
        wcout << L"Тип кодування файлу: ASCII\n";
        // Reverse all array of bytes
        std::reverse(staticBuffer, staticBuffer + bytesRead);

        // Write the result in a file
        fwrite(staticBuffer, 1, bytesRead, outFile);
    }
    fclose(outFile);
    wcout << L"Реверс файлу успiшно завершено!\n";
}

int main() {
    // Set console output encoding for UTF-8 (for Ciryllic)
    SetConsoleOutputCP(65001);
    _wsetlocale(LC_ALL, L".65001");
    
    // Check default encoding type (TCHAR)
    _tprintf(_T("Розмiр типу TCHAR у байтах: %d\n"), (int)sizeof(TCHAR));
#ifdef UNICODE
    _tprintf(_T("Режим: UNICODE (wchar_t)\n\n"));
    // TCHAR ==> wchar_t (2 bytes)
    // _tprintf ==> wprintf 
#else
    _tprintf(_T("Режим: ASCII (char)\n\n"));
    // TCHAR ==> char (1 byte)
    // _tprintf ==> printf
#endif

    // Full name in ASCII (char)
    constexpr size_t COUNT = 2;
    const char* pibASCII[COUNT] = {
        "Мерщанський Олег Сергiйович",
        "Мерщанський Iгор Сергiйович"
    };

    cout << "ASCII array:\n";
    for (size_t i = 0; i < COUNT; i++) {
        cout << "[" << i << "]: " << pibASCII[i] << "\n";
    }

    // Convert string to Unicode using MultiByteToWideChar
    _tprintf(_T("\nПереведення в Unicode та виведення 3 способами\n"));

    // Two static 2D arrays of wchar_t
    wchar_t pibUnicode[COUNT][MAX_PIB_LENGTH];
    wchar_t* pibPointers[COUNT]; // array of pointers required for qsort to work

    for (size_t i = 0; i < COUNT; i++) {
        // Convert data directly into a static array
        MultiByteToWideChar(
            CP_UTF8, // UTF-8
            0, // without additional checks. "Broken" byte ==> default character
            pibASCII[i], // pointer to the character string to convert
            -1, // size (bytes) of the input string (pibASCII[i]). -1 – entire input string
            pibUnicode[i], // pointer to a buffer that receives the result
            MAX_PIB_LENGTH // size (characters) of the buffer (pibUnicode[i])
        );
        pibPointers[i] = pibUnicode[i]; // pointer to the start of the string
    }
    
    // 3 output methods
    // 1: function _tprintf
    _tprintf(_T("_tprintf:\n"));
    for (unsigned int i = 0; i < COUNT; i++) {
        _tprintf(_T("[%u]: %ls\n"), i, pibPointers[i]); // %ls for explicit output wchar_t
    }

    // 2: stream wcout
    wcout << L"wcout:\n";
    for (unsigned int i = 0; i < COUNT; i++) {
        wcout << L"[" << i << L"]: " << pibPointers[i] << L"\n";
    }

    // 3: MessageBox (Windows graphic window)
    // Always working with Unicode regardless of the project
    for (size_t i = 0; i < COUNT; i++) {
        MessageBoxW(
            NULL, // the message box has no owner window
            pibPointers[i], // the message to be displayed
            L"MessageBox", // dialog box title
            MB_OK // the message box contains one push button OK. Default
        );
    }

    // Sorting an array of strings in Unicode using qsort
    _tprintf(_T("\nСортування масиву (qsort)\n"));
    qsort(
        pibPointers, // pointer to the array to sort
        COUNT, // number of elements in the array
        sizeof(wchar_t*), // size of each element in the array (bytes)
        [](const void* a, const void* b) -> int {
            // Double cast to sort an array of pointers to wchar_t
            return wcscmp(*(const wchar_t**)a, *(const wchar_t**)b);
        } // comparison lambda-function (sorting Unicode strings)

        // [](const void* a, const void* b) – callback function
    );

    wcout << L"Відсортований масив (qsort):\n";
    for (size_t i = 0; i < COUNT; i++) {
        wcout << L"[" << i << L"]: " << pibPointers[i] << L"\n";
    }

    // Варіант Б (для найвищої оцінки): за допомогою шаблонної std::sort
    std::sort(pibPointers, pibPointers + COUNT, [](const wchar_t* a, const wchar_t* b) {
        return wcscmp(a, b) < 0;
    });
    wcout << L"Вiдсортовано через std::sort:\n";
    for (size_t i = 0; i < COUNT; i++) wcout << L"[" << i << L"]: " << pibPointers[i] << L"\n";

    // Convert Unicode ==> ANSI (ASCII)
    _tprintf(_T("\nЗворотне перетворення в ASCII\n"));
    char resASCII[COUNT][MAX_PIB_LENGTH]; // static array for result
    for (size_t i = 0; i < COUNT; i++) {
        WideCharToMultiByte(
            CP_UTF8, // UTF-8
            0, // flag indicating the UTF-8 conversion type
            pibPointers[i], // pointer to the Unicode string to convert (where to read)
            -1, // size of the string indicated by pibPointers[i] (characters).
                // -1 – the entire input string, including the \0 char
            resASCII[i], // pointer to a buffer that receives the converted string (where to write)
            MAX_PIB_LENGTH, // size of the buffer indicated by resASCII[i] (bytes)
            NULL, // default char for a broken char. Must be NULL for UTF-8
            NULL // flag: was the default char used? Must be NULL for UTF-8
        );
        cout << "[" << i << "]: " << resASCII[i] << "\n";
    }

    // File reverse
    _tprintf(_T("\nРеверс файлу\n"));
    ReverseAnyFile(_T("input.txt"), _T("output.txt"));

    return 0;
}