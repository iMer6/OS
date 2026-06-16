#include <iostream>
#include <windows.h>
#include <string>

using std::cout;
using std::wstring;

bool RunTaskProc(const wstring& applicationPath, int runNumber) {
    cout << "\nRun " << runNumber << "\n";

    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Create a copy of the path string, (CreateProcessW may modify this buffer)
    wstring cmdLine = applicationPath;

    if (!CreateProcessW(
        NULL, // path to executable file
              // NULL – path from command line (2nd parameter)
        &cmdLine[0], // command line arguments to be executed
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
        cout << "Error: failed to start process. " << GetLastError() << "\n";
        return false;
    }

    // Wait for task1.exe to finish running
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Completion code
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    cout << "Run " << runNumber << " completed with code " << exitCode << "\n";

    // Memory deallocation
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

int main() {
    wstring taskPath = L"task1.exe"; 
    constexpr unsigned int totalRuns = 3;

    for (unsigned int i = 1; i <= totalRuns; i++) {
        RunTaskProc(taskPath, i);

        // 10 second pause between starts except the last one
        if (i < totalRuns) {
            Sleep(10000);
        }
    }
    return 0;
}