#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>

using std::cout;
using std::vector;
using std::unordered_set;
using std::unordered_map;

// FIFO
size_t firstInFirstOut(const vector<int>& pages, const size_t capacity) {
    unordered_set<int> cache; // hashset (whether the page is already in memory)
    std::queue<int> indexes; // to store pages
    size_t faults = 0; // miss counter

    for (int page : pages) {
        if (cache.find(page) == cache.end()) {
            // Page fault (cache full)
            if (cache.size() == capacity) {
                cache.erase(indexes.front()); // remove 1st item from the cache
                indexes.pop(); // remove 1st item from the queue
            }
            cache.insert(page); // add page to the cache
            indexes.push(page); // add page to the end of the queue
            faults++;
        }
    }
    return faults;
}

// LRU
size_t leastRecentlyUser(const vector<int>& pages, const size_t capacity) {
    unordered_set<int> cache;
    unordered_map<int, int> indexes;
    size_t faults = 0; // miss counter

    for (size_t i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (cache.find(page) == cache.end()) {
            // Page fault (cache full)
            if (cache.size() == capacity) {
                int lru = INT_MAX; // min index (oldest)
                int leastUsed;
                for (const int& item : cache) {
                    if (indexes[item] < lru) {
                        lru = indexes[item];
                        leastUsed = item;
                    }
                }
                cache.erase(leastUsed); // remove oldest item from the cache
            }
            cache.insert(page); // add page to the cache
            faults++;
        }
        indexes[page] = i; // update page's last used index
    }
    return faults;
}

// MRU
size_t mostRecentlyUsed(const vector<int>& pages, size_t capacity) {
    unordered_set<int> cache;
    unordered_map<int, int> indexes; // most used index
    size_t faults = 0; // miss counter

    for (size_t i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (cache.find(page) == cache.end()) {
            // Page fault (cache full)
            if (cache.size() == capacity) {
                int mru = -1; // max index (newest)
                int mostUsed = -1;
                for (const int& item : cache) {
                    if (indexes[item] > mru) {
                        mru = indexes[item];
                        mostUsed = item;
                    }
                }
                cache.erase(mostUsed); // remove newest page from the cache
            }
            cache.insert(page); // add page to the cache
            faults++;
        }
        indexes[page] = i; // update page's last used index
    }
    return faults;
}

// OPT / Belady's algorithm
size_t optimal(const vector<int>& pages, const size_t capacity) {
    unordered_set<int> cache;
    size_t faults = 0; // miss counter

    for (size_t i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (cache.find(page) != cache.end()) continue;

        // Page fault (cache full)
        if (cache.size() == capacity) {
            size_t farthest = i;
            int unnecessaryPage = -1;
            for (const int& item : cache) {
                size_t j;
                for (j = i + 1; j < pages.size(); j++)
                    if (pages[j] == item) break;
                
                // If the page no longer appears at all
                if (j == pages.size()) {
                    unnecessaryPage = item;
                    break;
                }

                if (j > farthest) {
                    farthest = j;
                    unnecessaryPage = item;
                }
            }
            cache.erase(
                unnecessaryPage == -1
                    ? *cache.begin()
                    : unnecessaryPage
            );
        }
        cache.insert(page); // add page to the set
        faults++;
    }
    return faults;
}

int main() {
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    size_t capacity = 3; // total physical frames in memory

    cout << "Comparison of page replacement algorithms\n";
    cout << "Sequence: ";
    for(int p : pages) cout << p << " ";
    cout << "\nFrame capacity: " << capacity << "\n\n";

    cout << "Count of uploads and downloads (faults):\n";
    cout << "FIFO: " << firstInFirstOut(pages, capacity) << "\n";
    cout << "LRU:  " << leastRecentlyUser(pages, capacity) << "\n";
    cout << "MRU:  " << mostRecentlyUsed(pages, capacity) << "\n";
    cout << "OPT:  " << optimal(pages, capacity) << "\n";

    return 0;
}