#include <iostream>
#include <windows.h>

using std::cout;

LPVOID AllocateBestFit(SIZE_T size) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    SIZE_T granularity = si.dwAllocationGranularity;

    MEMORY_BASIC_INFORMATION mbi;
    unsigned char* addr = (unsigned char*)si.lpMinimumApplicationAddress;
    
    LPVOID bestFitAddress = NULL;
    SIZE_T minExcess = SIZE_MAX;
    
    // Scan memory and look for the smallest sufficient block
    while (addr < si.lpMaximumApplicationAddress) {
        if (VirtualQuery(addr, &mbi, sizeof(mbi)) != sizeof(mbi))
            break;
        
        // Block must be free and its size must be sufficient
        if (mbi.State == MEM_FREE) { 
            ULONG_PTR rawAddr = (ULONG_PTR)mbi.BaseAddress;
            ULONG_PTR alignedAddr = (rawAddr + granularity - 1) & ~(granularity - 1);
            
            SIZE_T alignmentWaste = alignedAddr - rawAddr;
            
            if (mbi.RegionSize >= size + alignmentWaste) {
                SIZE_T usableSize = mbi.RegionSize - alignmentWaste;
                SIZE_T excess = usableSize - size; // unused memory
                
                if (excess < minExcess) {
                    minExcess = excess;
                    bestFitAddress = (LPVOID)alignedAddr;
                }
            }
            
        }
        // Next region
        addr = (unsigned char*)mbi.BaseAddress + mbi.RegionSize;
    }

    // Best block found
    // Allocate memory at the block's address
    if (bestFitAddress != NULL) {
        LPVOID allocated = VirtualAlloc(
            bestFitAddress,
            size,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        );

        if (allocated == NULL)
            cout << "VirtualAlloc failed. Error code: "
                 << GetLastError() << "\n";
        return allocated;
    }
    return NULL; 
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
        
        if (FreeBestFit(memory))
            cout << "Successful memory deallocation\n";
        else
            cout << "Failed to deallocate memory. Error: " << GetLastError() << "\n";
    } else {
        cout << "No sufficient free block found or allocation failed\n";
    }

    return 0;
}