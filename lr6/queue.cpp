#include <iostream>
#include <stdexcept> // runtime_error
#include <windows.h> // HANDLE

using std::cout;
using std::cerr;
using std::runtime_error;

class HeapQueue {
private:
    struct Node {
        int data;
        Node* next;
    };

    HANDLE hHeap;
    Node* head;
    Node* tail;

public:
    // Constructor
    HeapQueue() : head(nullptr), tail(nullptr) {
        hHeap = HeapCreate(0, 0, 0);

        if (hHeap == NULL) {
            throw runtime_error("Heap creation error!");
        }
    }

    // Destructor
    ~HeapQueue() {
        // Delete all elements
        while (!IsEmpty()) {
            Pop();
        }
        HeapDestroy(hHeap);
    }

    /**
     * @brief Adds a new element to the end of the queue.
     * 
     * The method creates a new Node and allocates memory for it using the `HeapAlloc` function.
     * After successfully memory allocation, the provided value is stored in the node,
     * and the next pointer is set to `nullptr`.
     * 
     * If the queue is empty, the new node becomes both the head and the tail of the queue.
     * If the queue already contains elements, the node is appended after the current tail,
     * and then the tail pointer is updated to the new last element.
     * 
     * The queue operates on a FIFO principle.
     * Insertion is always performed at the end of the list.
     * 
     * @param value the value to be added to the queue.
     * 
     * @throw runtime_error – thrown in case of memory allocation fauilure.
     */
    void Push(int value) {
        Node* newNode = (Node*)HeapAlloc(
            hHeap, // a handle to the heap
            HEAP_ZERO_MEMORY, // the heap allocation options.
                // HEAP_ZERO_MEMORY – the allocated memory will be initialized to zero.
            sizeof(Node) // the number of bytes to be allocated
        );

        if (newNode == NULL) {
            throw runtime_error("Allocation memory error");
        }

        newNode->data = value;
        newNode->next = nullptr;

        if (IsEmpty()) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    /**
     * @brief Removes and returns the front element from the queue.
     * 
     * The method retrieves and deletes the element at the front of the queue (FIFO order).
     * If the queue is empty, a `runtime_error` exception is thrown.
     * 
     * The current head node is temporarily stored, and its value is saved before removal.
     * The head pointer is then moved to the next node in the list.
     * If, after the update, the head becomes `nullptr`, the tail pointer is also set to `nullptr`,
     * indicating that the queue is now empty.
     * 
     * Memory allocated for the removed node is released using the `HeapFree` function.
     * 
     * @return `int` – the value stored in the removed front element of the queue.
     * 
     * @throw runtime_error – thrown when attempting to pop from an empty queue.
     */
    int Pop() {
        if (IsEmpty()) {
            throw runtime_error("The queue is empty");
        }

        Node* temp = head;
        int value = temp->data;

        head = head->next;

        if (head == nullptr) {
            tail = nullptr;
        }

        HeapFree(hHeap, 0, temp);

        return value;
    }

    int First() const {
        if (IsEmpty()) {
            throw runtime_error("The queue is empty");
        }
        return head->data;
    }

    bool IsEmpty() const { return head == nullptr; }
};

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    try {
        HeapQueue queue;

        cout << "Add elements: 10, 20, 30\n";
        queue.Push(10);
        queue.Push(20);
        queue.Push(30);

        cout << "First element: " << queue.First() << "\n";

        queue.Pop();
        queue.Pop();

        cout << "Add element: 40\n";
        queue.Push(40);

        queue.Pop();
        queue.Pop();

        cout << "Is the queue empty? " << (queue.IsEmpty() ? "Yes" : "No") << "\n";
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}