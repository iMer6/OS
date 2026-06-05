#include <iostream>
#include <windows.h>
#include <iomanip>

using std::cout;
using std::left;
using std::setw;
using std::hex;
using std::dec;

void dumpFreeBlocks() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    MEMORY_BASIC_INFORMATION mbi;
    // Start from minimum possible app address
    unsigned char* addr = (unsigned char*)si.lpMinimumApplicationAddress; 

    cout << left << setw(18) << "Hex address"
         << setw(15) << "Size (KiB)"
         << "State\n";
    cout << "---------------------------------------------\n";

    // Scan address space
    while (addr < si.lpMaximumApplicationAddress) {
        if (VirtualQuery(addr, &mbi, sizeof(mbi)) != sizeof(mbi)) break;
        
        if (mbi.State == MEM_FREE) // only free blocks (65536)
            cout << "0x" << setw(16) << hex << (uintptr_t)mbi.BaseAddress
                    << dec << setw(15) << (mbi.RegionSize / 1024) // KiB
                    << "MEM_FREE\n";
        
        // Move to next region
        addr = (unsigned char*)mbi.BaseAddress + mbi.RegionSize;
    }
}

int main() {
    system("cls");

    cout << "\nList of free blocks before allocation\n";
    dumpFreeBlocks();
    
    system("pause");

    // Allocate 1 KiB of virtual memory
    SIZE_T sizeToAlloc = 1024; // 1 KiB in bytes
    cout << "\nAllocation " << sizeToAlloc / 1024
         << " kibibyte of memory via VirtualAlloc\n";
    
    LPVOID pAlloc = VirtualAlloc(
        NULL, // starting address of the region to allocate
              // NULL – system determines where to allocate the region
        sizeToAlloc, // size of the region (bytes)
        MEM_RESERVE | MEM_COMMIT, // type of memory allocation
            // MEM_COMMIT – allocate virtual addresses with real physycal pages of memory
            // MEM_RESERVE – reserves a range from the virtual address space
        PAGE_READWRITE // the memory protection
            // Program can read + write data
    );

    if (pAlloc == NULL) {
        cout << "Error: could not allocate memory.\n";
        return 1;
    }

    cout << "Memory allocate by address: " << pAlloc << "\n";
    
    cout << "\nList of free blocks after allocation\n";
    dumpFreeBlocks();

    // Deallocate memory
    VirtualFree(pAlloc, 0, MEM_RELEASE);

    return 0;
}