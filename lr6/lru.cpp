#include <iostream>
#include <climits>

using std::cout;
using std::cerr;
using std::hex;
using std::dec;

struct CacheBlock {
    bool valid = false; // is the block occupied?
    unsigned long tag = 0;
    unsigned long lastUsedTick = 0; // LRU counter
};

class CPUCacheSim {
private:
    static const int ASSOCIATIVITY = 4; // 4-way set-associative cache (n)
    static const int LINES = 128; // l
    CacheBlock cache[LINES][ASSOCIATIVITY]; // cache matrix (128 × 4 = 512 blocks)
    unsigned long simTick = 0; // global counter (++ at call accessMemory)

    // How many times the processor requested a memory address
    // that was already in the cache?
    size_t hits = 0;
    // How many times the processor looked for data
    // but it was not found in the cache?
    size_t misses = 0;

public:
    /**
     * @brief Processes a CPU memory request and checks for a cache hit.
     * 
     * Increments the global simulation tick, decodes the memory address into
     * `offset`, `line`, and `tag` fields, and performs a parallel search across all
     * cache ways for the target line. On a cache hit, updates the hit statistics
     * and refreshes the LRU status tracker for the hit block.
     * 
     * @param[in] address the memory address to be accessed.
     */
    void accessMemory(unsigned long address) {
        simTick++; // memory access

        // Address decoding
        [[maybe_unused]] unsigned long offset = address & 0x3F; // 0..63 (last 6 bits)
        unsigned long line = (address >> 6) & 0x7F; // 0..127 (7 bits – LINES)
        unsigned long tag = address >> 13; // remaining high bits

        // Check for cache hit on current line
        for (size_t i = 0; i < ASSOCIATIVITY; i++) {
            // Cache hit
            if (
                cache[line][i].valid // is this cell not empty?
                && cache[line][i].tag == tag // block data == required data?
            ) {
                // Block found
                hits++;
                // Update LRU status
                // This block is now the freshest
                // and will not be replaced by the cache anytime soon.
                cache[line][i].lastUsedTick = simTick;
                cout << "Address 0x" << hex << address
                     << " ==> Hit (line: " << dec << line << ", way: " << i << ")\n";
                return;
            }
        }

        // Cache Miss – no data in cache
        misses++;
        cout << "Address 0x" << hex << address
             << " ==> Miss (line: " << dec << line << ")\n";

        // Free space or block for eviction
        int targetWay = -1;
        unsigned long minTick = ULONG_MAX;

        for (int i = 0; i < ASSOCIATIVITY; i++) {
            if (!cache[line][i].valid) {
                targetWay = i; // empty space gets priority
                break; // empty space – the best choice
            }
            if (cache[line][i].lastUsedTick < minTick) {
                minTick = cache[line][i].lastUsedTick;
                targetWay = i; // way with the smallest (oldest) tick
            }
        }

        // Cache block eviction
        cache[line][targetWay].valid = true; // cell occupied
        cache[line][targetWay].tag = tag;
        cache[line][targetWay].lastUsedTick = simTick; // just used it
        cout << "Placed in Way: " << targetWay << "\n";
    }

    void printStats() {
        cout << "\nCache statistics\n" << dec;
        cout << "Hits: " << hits << "\n";
        cout << "Misses: " << misses << "\n";
        double hit_rate = (hits + misses > 0)
            ? (double)hits / (hits + misses) * 100
            : 0;
        cout << "Hit Rate: " << hit_rate << "%\n";
    }
};

int main() {
    CPUCacheSim sim;
        
    sim.accessMemory(0x1000); // Miss
    sim.accessMemory(0x1004); // Hit (same block, other offset, within 64 bytes)
    
    // Fill 4 ways for the line with 64 different tags
    // Misses
    sim.accessMemory(0x1000 + (1 << 13));
    sim.accessMemory(0x1000 + (2 << 13));
    sim.accessMemory(0x1000 + (3 << 13));
    
    // All 4 blocks are occupied.
    // Re-accessing the first one will update its tick.
    sim.accessMemory(0x1000); // Hit (way 0), freshest

    sim.accessMemory(0x1000 + (4 << 13)); // Miss ==> LRU eviction

    sim.printStats();
    return 0;
}