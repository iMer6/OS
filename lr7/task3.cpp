#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::wcout;

void PrintProcessModules(DWORD pid) {
    // Create a snapshot of modules (DLL) for a specific process
    HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, // flags
            // What will be included in the snapshot?
            // TH32CS_SNAPMODULE – all process modules
            // TH32CS_SNAPMODULE32 – all 32-bit process modules (for 64-bit OS)
        pid // Process ID
    );
    
    if (hModuleSnapshot == INVALID_HANDLE_VALUE) {
        cout << "\tAccess Denied / No Modules\n";
        return;
    }

    MODULEENTRY32 me32; // struct for data about each DLL found
    me32.dwSize = sizeof(MODULEENTRY32); // size of the struct

    if (Module32First(hModuleSnapshot, &me32)) {
        cout << "\tUsed DLLs:\n";
        do {
            // me32.szModule – DLL name
            wcout << L"\t  ==> " << me32.szModule << L"\n";
        } while (Module32Next(hModuleSnapshot, &me32));
    }
    CloseHandle(hModuleSnapshot);
}

int main() {
    // Create a snapshot descriptor of all processes in the system
    HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS, // flags
            // TH32CS_SNAPPROCESS – all processes in the system
        0 // Process ID
          // 0 – system-wide snapshot
    );

    if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create process snapshot.\n";
        return 1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Get info about the first process
    if (!Process32First(hProcessSnapshot, &pe32)) {
        cerr << "Failed to get first process.\n";
        CloseHandle(hProcessSnapshot);
        return 1;
    }

    cout << "System processes report\n";
    
    // Limit the output to the first 5 processes
    int limit = 0; 
    do {
        limit++;

        wcout << L"\n" << limit << "\n";
        wcout << L"Process Name: " << pe32.szExeFile << L"\n";
        cout << "Process ID:   " << pe32.th32ProcessID << "\n";
        cout << "Threads Count: " << pe32.cntThreads << "\n";

        // Get the DLL for this process
        PrintProcessModules(pe32.th32ProcessID);
        
    } while (Process32Next(hProcessSnapshot, &pe32) && limit < 10);

    CloseHandle(hProcessSnapshot); // memory deallocation
    return 0;
}