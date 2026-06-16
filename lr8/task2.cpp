#include <iostream>
#include <windows.h>
#include <vector>
#include <atomic>

std::atomic<int> globalCallCount(0); // func call counter

unsigned long WINAPI ThreadProc([[maybe_unused]] void* lpParam) {
    globalCallCount++; // func call
    Sleep(10); // wait for other threads to start
    return 0;
}

int main() {
    unsigned int threadCount = 2; // initial quantity

    while (threadCount <= 65536) {
        globalCallCount = 0;
        std::vector<HANDLE> threads;
        bool isCreated = true;

        for (unsigned int i = 0; i < threadCount; i++) {
            HANDLE hThread = CreateThread(
                NULL, // secure attributes
                      // NULL – default security descriptor, the handle cannot be inherited
                0, // the initial size of the stack (bytes)
                ThreadProc, // pointer to the func to be executed by the thread
                NULL, // pointer to a variable to be passed to the thread
                0, // the flags that control the creation of the thread
                   // 0 – the thread runs immediately after creation
                NULL // pointer to a variable that receives the thread ID
                     // NULL – the thread ID is not returned
            ); // return value is a handle to the new thread
            if (hThread == NULL) {
                isCreated = false;
                break; // failed to create thread
            }
            threads.push_back(hThread); // append a new thread to the end of a vector
        }

        // Waiting for current threads to complete
        if (!threads.empty()) {
            for (auto h : threads) {
                WaitForSingleObject(
                    h, // the thread
                    INFINITE // wait 'til the thread ends
                );
                CloseHandle(h);
            }
        }

        std::cout << "Trying to run " << threadCount
                  << " threads. Successful: " << globalCallCount.load() << "\n";

        // Exit if the number of calls != the number of threads
        if (!isCreated
            || static_cast<unsigned int>(globalCallCount.load()) != threadCount) {
            std::cout << "Maximum number of threads that can be run: "
                      << globalCallCount.load() << "\n";
            break;
        }

        threadCount *= 2;
    }

    return 0;
}