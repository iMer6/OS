#include <windows.h>
#include <iostream>
#include "createProc.h"

using std::cout;

int main() {
    cout << "Program 3 started.\n";

    PROCESS_INFORMATION pi1;
    cout << "Program 3 launching program 1\n";
    wchar_t cmd1[] = L"prog1.exe";
    LAUNCH_PROCESS_NOW(
        NULL, // file path (from command line)
        cmd1, // command line arguments 
        pi1 // where Windows will write info about the created process
    );
    
    if (pi1.hProcess != NULL) {
        // Wait for the complete on the first program
        WaitForSingleObject(pi1.hProcess, INFINITE);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        cout << "Program 1 finished execution.\n\n";
    } else {
        std::cerr << "Error: failed to launch program 1.\n";
        return 1;
    }

    PROCESS_INFORMATION pi2;
    cout << "Program 3 launching program 2 with BELOW_NORMAL_PRIORITY_CLASS\n";
    wchar_t cmd2[] = L"prog2.exe";
    LAUNCH_PROCESS_CUSTOM(
        NULL, // file path (from command line)
        cmd2, // command line arguments
        BELOW_NORMAL_PRIORITY_CLASS, // flags
        pi2 // where Windows will write info about the created process
    );

    if (pi2.hProcess != NULL) {
        // Wait for the complete on the second program
        WaitForSingleObject(pi2.hProcess, INFINITE);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
        cout << "Program 2 finished execution.\n";
    } else {
        std::cerr << "Error: failed to launch program 2.\n";
        return 1;
    }

    cout << "All operations completed.\n";
    return 0;
}