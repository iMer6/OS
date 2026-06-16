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

constexpr unsigned int TOTAL_PHILOSOPHERS = 3;
HANDLE forks[TOTAL_PHILOSOPHERS]; // each fork is a mutex

std::vector<string> newsList; 
CRITICAL_SECTION NewsCS; // list synchronization
CRITICAL_SECTION ConsoleCS; // synchronizing output to console

unsigned long WINAPI ThreadProc(void*);
unsigned long WINAPI PhilosopherThreadProc(void*);
void WriterThreadProc(unsigned int);
void ReaderThreadProc(unsigned int);
void logMessage(const string&);

int main() {
    InitializeCriticalSection(&NewsCS);
    InitializeCriticalSection(&ConsoleCS);

    std::vector<HANDLE> threads;

    // Create 2 threads of writers
    for (unsigned int i = 1; i <= 2; i++) {
        threads.push_back(
            CreateThread(
                NULL, // secure attributes
                      // NULL – default security descriptor, the handle cannot be inherited
                0, // the initial size of the stack (bytes)
                ThreadProc, // pointer to the func to be executed by the thread
                new ThreadParams{i, 1}, // pointer to a params to be passed to the thread
                0, // the flags that control the creation of the thread
                   // 0 – the thread runs immediately after creation
                NULL // pointer to a variable that receives the thread ID
                     // NULL – the thread ID is not returned
            )
        );
    }
    // Create 3 threads of readers
    for (unsigned int i = 1; i <= 3; i++) {
        threads.push_back(
            CreateThread(
                NULL, // secure attributes
                      // NULL – default security descriptor, the handle cannot be inherited
                0, // the initial size of the stack (bytes)
                ThreadProc, // pointer to the func to be executed by the thread
                new ThreadParams{i, 2}, // pointer to a params to be passed to the thread
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
    
    for (const HANDLE& h : threads) CloseHandle(h);
    threads.clear();
    DeleteCriticalSection(&NewsCS);

    cout << "\n\n";

    // Create mutexes-forks
    for (unsigned int i = 0; i < TOTAL_PHILOSOPHERS; i++) {
        forks[i] = CreateMutex(
            NULL, // secure attributes
                  // NULL – the handle cannot be inherited by child processes
            FALSE, // the calling thread doesn't obtain ownership of the mutex
            NULL // the name of the mutex object
                 // NULL – the mutex object is created without a name
        );
    }

    std::vector<HANDLE> philosophers;
    for (unsigned int i = 1; i <= TOTAL_PHILOSOPHERS; i++) {
        philosophers.push_back(
            CreateThread(
                NULL, // secure attributes
                      // NULL – default security descriptor, the handle cannot be inherited
                0, // the initial size of the stack (bytes)
                PhilosopherThreadProc, // pointer to the func to be executed by the thread
                new unsigned int(i), // pointer to a param to be passes to the thread
                0, // the flags that control the creation of the thread
                   // 0 – the thread runs immediately after creation
                NULL // pointer to a variable that receives the thread ID
                     // NULL – the thread ID is not returned
            )
        );
    }

    // Wait for philosophers to finish lunch
    WaitForMultipleObjects(
        philosophers.size(), // the number of object handles in the array (2nd param)
        philosophers.data(), // an array of object handles
        TRUE, // wait for all threads
        INFINITE // wait 'til the thread ends
    );

    // Memory deallocation
    for (const HANDLE& h : philosophers) {
        CloseHandle(h);
    }
    for (unsigned int i = 0; i < TOTAL_PHILOSOPHERS; i++) {
        CloseHandle(forks[i]);
    }
    philosophers.clear();
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

unsigned long WINAPI PhilosopherThreadProc(void* lpParam) {
    unsigned int id = *static_cast<unsigned int*>(lpParam);
    delete static_cast<unsigned int*>(lpParam);

    unsigned int firstFork = id - 1;
    unsigned int secondFork = id % TOTAL_PHILOSOPHERS;

    // Preventing races and deadlocks
    // The last philosopher changes the order of raising the forks.
    if (id == TOTAL_PHILOSOPHERS) {
        std::swap(firstFork, secondFork);
    }

    for (unsigned int i = 0; i < 1; i++) {
        logMessage("Philosopher " + to_string(id) + " thinks.");
        
        WaitForSingleObject(forks[firstFork], INFINITE);
        logMessage("Philosopher " + to_string(id) + " took the first fork");
        
        WaitForSingleObject(forks[secondFork], INFINITE);
        logMessage("Philosopher " + to_string(id) + " took the second fork");
        
        // Having lunch
        Sleep(2000);
        
        ReleaseMutex(forks[secondFork]);
        logMessage("Philosopher " + to_string(id) + " put down the second fork");
        
        ReleaseMutex(forks[firstFork]);
        logMessage("Philosopher " + to_string(id) + " put down the first fork");

        logMessage("Philosopher " + to_string(id) + " finished his lunch");
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

void logMessage(const string& message) {
    EnterCriticalSection(&ConsoleCS);
    cout << message << std::endl;
    LeaveCriticalSection(&ConsoleCS);
}