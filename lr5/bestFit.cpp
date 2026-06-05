#include <iostream>
#include <windows.h>
#include <vector>

using std::cout;

// Info about best block found
struct BestBlock {
    LPVOID baseAddress;
    SIZE_T size;
};

LPVOID AllocateBestFit(SIZE_T size) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    MEMORY_BASIC_INFORMATION mbi;
    unsigned char* addr = (unsigned char*)si.lpMinimumApplicationAddress;
    
    LPVOID bestFitAddress = NULL;
    int minExcess = INT_MAX;
    
    // Scan memory and look for the smallest sufficient block
    while (addr < si.lpMaximumApplicationAddress) {
        if (VirtualQuery(addr, &mbi, sizeof(mbi)) != sizeof(mbi))
            break;
        
        // Block must be free and its size must be sufficient
        if (mbi.State == MEM_FREE && mbi.RegionSize >= size) { 
            int excess = mbi.RegionSize - size; // unused memory
            if (excess < minExcess) {
                minExcess = excess;
                bestFitAddress = mbi.BaseAddress;
            }
        }
        addr = (unsigned char*)mbi.BaseAddress + mbi.RegionSize;
    }

    // Best block found
    // Allocate memory at the block's address
    if (bestFitAddress != NULL) {
        return VirtualAlloc(
            bestFitAddress,
            size,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        );
    }
    return NULL; // best block not found
}

BOOL FreeBestFit(LPVOID lpAddress) {
    return VirtualFree(lpAddress, 0, MEM_RELEASE);
}

int main() {
    cout << "Test Best fit strategy\n";

    SIZE_T requestSize = 256 * 1024; // 256 KiB
    cout << "Attempt to allocate " << requestSize / 1024
         << " KiB using Best Fit strategy\n";

    LPVOID memory = AllocateBestFit(requestSize);

    if (memory != NULL) {
        cout << "Memory allocated at address: " << memory << "\n";
        
        cout << (
            FreeBestFit(memory)
                ? "Successful memory deallocation"
                : "Failed to deallocate memory"
            ) << "\n";
    } else {
        cout << "No sufficient free block not found\n";
    }

    return 0;
}