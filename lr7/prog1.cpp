#include <windows.h>
#include <iostream>
#include <fstream>

using std::cout;

void CreateTextFiles() {
    // Create a folder (if it doesn't exist)
    CreateDirectoryA("C:\\C++\\OS\\lr7\\files", NULL);

    // Create an ASCII file
    std::ofstream asciiFile("C:\\C++\\OS\\lr7\\files\\afile.txt");
    if (asciiFile.is_open()) {
        asciiFile << "Hello World!\n";
        asciiFile << "ASCII file.";
        asciiFile.close();
        cout << "ASCII file created.\n";
    }

    // Create an UNICODE (UTF-16 LE) file with BOM
    std::ofstream unicodeFile(
        "C:\\C++\\OS\\lr7\\files\\ufile.txt",
        std::ios::binary
    );

    if (unicodeFile.is_open()) {
        // BOM for UTF-16 LE (0xFEFF)
        unsigned char bom[2] = { 0xFF, 0xFE };
        unicodeFile.write((char*)bom, 2);

        wchar_t line1[] = L"Привіт Світ!\n";
        wchar_t line2[] = L"UNICODE файл (UTF-16).";

        unicodeFile.write((char*)line1, wcslen(line1) * sizeof(wchar_t));
        unicodeFile.write((char*)line2, wcslen(line2) * sizeof(wchar_t));
        unicodeFile.close();
        cout << "Unicode file created.\n";
    }
}

int main() {
    cout << "Program 1 starting\n";
    
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi;

    // Start a standard notepad
    wchar_t cmd[] = L"notepad.exe";
    if (CreateProcess(
        NULL, // path to executable file
        cmd, // command line arguments to be executed
        NULL, // process gets a default security descriptor, handle cannot be inherited
        NULL, // thread gets a default security descriptor, handle cannot be inherited
        FALSE, // handles are not inherited
        0, // flags that control the priority class and the creation of the process
           // 0 – the process inherits both the error mode of the caller and the parent's console
        NULL, // new process copies the environment variables of the calling process
        NULL, // working folder = folder in which the program is running
        &si, // full path to the current directory for the process
        &pi // pointer to a PROCESS_INFORMATION structure
    )) {
        cout << "Notepad opened. Close it to continue file creation\n";
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // Generate files for the second program
    CreateTextFiles();
    
    cout << "Program 1 finished successfully.\n";
    return 0;
}