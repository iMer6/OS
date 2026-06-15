#include <iostream>
#include <windows.h>
#include <fstream>
#include <atomic>

using std::cout;

// WinAPI synchronization object
// Needed so that threads don't write to the log file at the same time
CRITICAL_SECTION ConsoleCS;
std::atomic<int> globalCallCount(0); // func call counter
 
unsigned long WINAPI ThreadProc(void* lpParam) {
    [[maybe_unused]] unsigned int threadNum = *static_cast<unsigned int*>(lpParam);
    
    // Memory was allocated in main via 'new' operator must be deallocated
    delete static_cast<unsigned int*>(lpParam);

    globalCallCount++; // func call

    // Logging about start work in DEBUG mode
#ifdef _DEBUG
    // Thread that got here first locks other access to file
    // 'til it writes its line
    EnterCriticalSection(&ConsoleCS); // lock file
    std::ofstream logFile("debugLogs.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "Thread " << threadNum << " started working.\n";
        logFile.close();
    }
    LeaveCriticalSection(&ConsoleCS); // unlock file
#endif

    // Heavy cycle to load the CPU
    // `volatile` prevents the compiler from optimizing this code
    volatile size_t value = 0;
    for (size_t i = 0; i < 50000000; i++) {
        value++;
    }

    // Logging about end of work in DEBUG mode
#ifdef _DEBUG
    EnterCriticalSection(&ConsoleCS); // lock file
    logFile.open("debugLogs.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "Thread " << threadNum << " finished work.\n";
        logFile.close();
    }
    LeaveCriticalSection(&ConsoleCS); // unlock file
#endif

    return 0;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    // Initialization of synchronization object before use
    InitializeCriticalSection(&ConsoleCS);

    constexpr size_t numThreads = 10;
    HANDLE threads[numThreads];

    for (size_t i = 0; i < numThreads; i++) {
        unsigned int* param = new unsigned int(i + 1); // sequence number for thread
        threads[i] = CreateThread(
            NULL, // secure attributes
                  // NULL – default security descriptor, the handle cannot be inherited 
            0, // the initial size of the stack (bytes)
            ThreadProc, // pointer to the function to be executed by the thread
            param, // pointer to a variable to be passed to the thread
            0, // the flags that control the creation of the thread
               // 0 – the thread runs immediately after creation
            NULL // pointer to a variable that receives the thread ID
                 // NULL – the thread ID is not returned
        ); // return value is a handle to the new thread.
    }

    // Waiting for at least one thread to finish
    // Forces `main` to wait for events from child threads
    DWORD waitResult = WaitForMultipleObjects(
        numThreads, // the number of object handles in the array (threads)
        threads, // an array of object handles
        FALSE, // continue work as soon as at least one thread ends
        INFINITE // wait 'til the thread ends
    ); // returns `WAIT_OBJECT_0 + thread index` if success
    size_t finishedIndex = waitResult - WAIT_OBJECT_0; // first-finished thread index
    
    cout << "Thread " << (finishedIndex + 1) << " finished execution first\n";
    cout << "Number of thread func calls: " << globalCallCount.load() << "\n";

    // Waiting for the remaining threads to close the handles
    WaitForMultipleObjects(
        numThreads, // the number of object handles in the array (threads)
        threads, // an array of objects handles
        TRUE, // wait for all threads
        INFINITE // wait 'til the thread ends
    );

    // Memory deallocation
    for (size_t i = 0; i < numThreads; i++) {
        CloseHandle(threads[i]);
    }
    DeleteCriticalSection(&ConsoleCS);
    return 0;
}