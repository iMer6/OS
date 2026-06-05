#include <iostream>
#include <windows.h>

using std::cout;

void printSystemInfo() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    cout << "System info:\n";
    cout << "Page size: " << si.dwPageSize << " bytes\n";
    cout << "Minimum app address: " << si.lpMinimumApplicationAddress << "\n";
    cout << "Maximum app address: " << si.lpMaximumApplicationAddress << "\n";
}

void printMemoryStatus() {
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatusEx(&ms);

    cout << "\nMemory status:\n";
    cout << "Memory load: " << ms.dwMemoryLoad << "%\n";
    cout << "Total physical: " << ms.ullTotalPhys / 1048576 << " MB\n";
    cout << "Available physical: " << ms.ullAvailPhys / 1048576 << " MB\n";
    cout << "Total virtual: " << ms.ullTotalVirtual / 1048576 << " MB\n";
    cout << "Available virtual: " << ms.ullAvailVirtual / 1048576 << " MB\n";
}

int main() {
    printSystemInfo();
    printMemoryStatus();

    cout << "Address of start of main function: " << (void*)&main << "\n";
    
    system("pause"); 
    return 0;
}