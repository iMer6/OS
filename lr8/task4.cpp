#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::to_string;

struct ThreadParams {
    unsigned int id;
    unsigned int role; // 1 – writer, 2 – reader
};

std::vector<string> newsList; 
CRITICAL_SECTION NewsCS; // list synchronization
CRITICAL_SECTION ConsoleCS; // synchronizing output to console

unsigned long WINAPI ThreadProc(void*);
void WriterThreadProc(unsigned int);
void ReaderThreadProc(unsigned int);

int main() {
    InitializeCriticalSection(&NewsCS);
    InitializeCriticalSection(&ConsoleCS);

    std::vector<HANDLE> threads;

    // Create 2 threads of writers
    for (int i = 1; i <= 2; i++) {
        threads.push_back(
            CreateThread(
                NULL, // secure attributes
                      // NULL – default security descriptor, the handle cannot be inherited
                0, // the initial size of the stack (bytes)
                ThreadProc, // pointer to the func to be executed by the thread
                new ThreadParams{(unsigned int)i, 1}, // pointer to a params to be passed to the thread
                0, // the flags that control the creation of the thread
                   // 0 – the thread runs immediately after creation
                NULL // pointer to a variable that receives the thread ID
                     // NULL – the thread ID is not returned
            )
        );
    }
    // Create 3 threads of readers
    for (int i = 1; i <= 3; i++) {
        threads.push_back(
            CreateThread(
                NULL, // secure attributes
                      // NULL – default security descriptor, the handle cannot be inherited
                0, // the initial size of the stack (bytes)
                ThreadProc, // pointer to the func to be executed by the thread
                new ThreadParams{(unsigned int)i, 2}, // pointer to a params to be passed to the thread
                0, // the flags that control the creation of the thread
                   // 0 – the thread runs immediately after creation
                NULL // pointer to a variable that receives the thread ID
                     // NULL – the thread ID is not returned
            )
        );
    }

    WaitForMultipleObjects(
        threads.size(), // the number of object handles in the array (threads)
        threads.data(), // an array of object handles
        TRUE, // wait for all threads
        INFINITE // wait 'til the thread ends
    );
    
    for (auto h : threads) CloseHandle(h);
    DeleteCriticalSection(&NewsCS);
    DeleteCriticalSection(&ConsoleCS);

    return 0;
}

unsigned long WINAPI ThreadProc(void* lpParam) {
    ThreadParams* params = static_cast<ThreadParams*>(lpParam);
    unsigned int id = params->id;
    unsigned int role = params->role;
    // Memory was allocated in main via 'new' operator must be deallocated
    delete params;

    if (role == 1) {
        WriterThreadProc(id);
    } else if (role == 2) {
        ReaderThreadProc(id);
    }
    return 0;
}

void WriterThreadProc(unsigned int id) {
    for (unsigned int i = 1; i <= 3; i++) {
        // Only one thread can working at the same time
        EnterCriticalSection(&NewsCS); // lock NewsCS
        string news = "News " + to_string(i) + " from writer " + to_string(id);
        newsList.push_back(news); // append to the end
        
        EnterCriticalSection(&ConsoleCS); // lock console
        cout << "Writer " << id << " wrote: " << news << "\n";
        LeaveCriticalSection(&ConsoleCS); // unlock console
        
        LeaveCriticalSection(&NewsCS); // unlock NewsCS
    }
}

void ReaderThreadProc(unsigned int id) {
    for (unsigned int i = 0; i < 4; i++) {
        // Only one thread can working at the same time
        EnterCriticalSection(&NewsCS); // lock NewsCS
        EnterCriticalSection(&ConsoleCS); // lock console
        
        if (!newsList.empty()) {
            cout << "Reader " << id << " read the latest news: " << newsList.back() << "\n";
        } else {
            cout << "Reader " << id << " has no news yet\n";
        }
        
        LeaveCriticalSection(&ConsoleCS); // unlock console
        LeaveCriticalSection(&NewsCS); // unlock NewsCS
    }
}