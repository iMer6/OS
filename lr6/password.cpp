#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <limits>

// isGoodPassword
#include <locale> // std::locale
#include <cwctype> // std::isupper, std::isLower, std::isDigit

using std::string;
using std::setw;
using std::cerr;
using std::cout;
using std::wcout;
using std::wcin;

/**
 * @brief Computes a hash value for a given wide-character string using the djb2 algorithm.
 *
 * This function implements the classic djb2 string hashing algorithm proposed by 
 * Daniel J. Bernstein. It uses the initial magic constant 5381 and a bitwise 
 * shift-and-add loop to minimize collisions and distribute hash values uniformly.
 *
 * @param str a pointer to the wide-character string to be hashed.
 * @param length the number of characters in the string.
 * @return An unsigned long representing the computed hash value.
 * 
 * @note http://www.cse.yorku.ca/~oz/hash.html
 * @note The choice of 5381 as the initial value is a mathematical heuristic 
 * that helps reduce collisions in short strings.
 * @note Linear time, O(n), where n is the length of the input string.
 */
unsigned long hash(const wchar_t* str, size_t length) {
    unsigned long hash = 5381; // initial hash number

    for (size_t i = 0; i < length; i++) {
        if (*(str + i) == L'\0') break;

        // *(str + i) – value (char) at index 'i'

        hash = ((hash << 5) + hash) + *(str + i);
        // hash = hash * 33 + c
        // 33 – empirically best magic number
    }
    return hash;
}

/**
 * @brief Validates if a password meets the strength requirements.
 *
 * This function checks whether a given wide-character password string satisfies 
 * the minimum security criteria. A password is considered "good" if it is at 
 * least 8 characters long and contains at least one uppercase letter, one 
 * lowercase letter, and one digit. It utilizes the system's native locale 
 * to properly support multi-byte alphabets (e.g., Cyrillic).
 *
 * @param password a pointer to the wide-character string to be checked.
 * @param length the number of characters in the string (password).
 * @return bool. Does the password meet all security criteria? 
 
 * @note This function optimizes performance by returning \c true as soon as all 
 * three character conditions are satisfied before fully iterating through the string.
 * @note Uses \c std::locale("") to adapt to the user's system locale configuration 
 * for accurate \c std::isupper, \c std::islower, and \c std::isdigit evaluation.
 */
bool isGoodPassword(const wchar_t* password, size_t length) {
    if (length < 8) return false;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;

    static const std::locale loc("");

    for (size_t i = 0; i < length; i++) {
        if (*(password + i) == L'\0') break;

        wchar_t ch = *(password + i);
        if (std::isupper(ch, loc)) hasUpper = true;
        if (std::islower(ch, loc)) hasLower = true;
        if (std::isdigit(ch, loc)) hasDigit = true;
        
        if (hasUpper && hasLower && hasDigit) return true;
    }
    return hasUpper && hasLower && hasDigit;
}

/**
 * @brief Reads a password from the console without echoing input characters.
 * 
 * Temporarily disables the console input echo (`ENABLE_ECHO_INPUT`)
 * to ensure privacy while typing a password. The original console mode is 
 * restored immediately after reading. The resulting string is automatically 
 * stripped of trailing newline characters (`\r`, `\n`).
 * 
 * @param buffer pointer to a pre-allocated `wchar_t` buffer
 * where the entered password will be stored.
 * @param maxLength the max size of the buffer (including the null-terminator). 
 * The function will read at most `maxLength - 1` characters.
 * 
 * @return Was the password read successfully?
 * 
 * @note This function modifies the console state only during its execution.
 */
bool readPassword(wchar_t* buffer, DWORD maxLength) {
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdIn, &mode);
    
    // Turn off the display of entered characters on the screen (ECHO)
    SetConsoleMode(hStdIn, mode & ~ENABLE_ECHO_INPUT);

    DWORD read = 0;
    // Read directly into a protected locked buffer
    BOOL success = ReadConsoleW(hStdIn, buffer, maxLength - 1, &read, NULL);
    
    SetConsoleMode(hStdIn, mode); // returning console mode back

    if (!success) return false;

    // Remove characters '\r' and '\n' at the end
    for (DWORD i = 0; i < read; i++) {
        if (*(buffer + i) == L'\r' || *(buffer + i) == L'\n') {
            *(buffer + i) = L'\0';
            break;
        }
    }
    return true;
}

/**
 * @brief Prompts the user to set and confirm a new password,
 * performs validation, and returns its hash.
 *
 * Allocates locked virtual memory pages to prevent swapping to disk, reads
 * the password twice using `readPassword`, checks strength via `isGoodPassword`,
 * and returns the hash. Clears and frees all memory before exit.
 * 
 * @return The calculated hash of the password
 * if it matches and passes strength requirements.
 * Returns 0 if memory allocation fails, an input error occurs, 
 * passwords do not match, or the password is too weak.
 *
 * @note Guarantees secure memory cleanup (`SecureZeroMemory`) on all return paths.
 */
unsigned long setPassword() {
    const SIZE_T PAGE_SIZE = 4096; // page size (bytes)
    wchar_t* pw = (wchar_t*)VirtualAlloc(
        NULL, // starting address of the block to allocate
              // NULL – system determines where to allocate the block
        PAGE_SIZE, // size of the block (bytes)
        MEM_RESERVE | MEM_COMMIT, // type of memory allocation
            // MEM_COMMIT – allocate virtual addresses with real physycal pages of memory
            // MEM_RESERVE – reserves a range from the virtual address space
        PAGE_READWRITE // the memory protection
            // Program can read + write data
    );

    if (!pw) {
        cerr << "Memory allocation error!\n";
        return 0;
    }

    // Do not dump to disk (swapping to pagefile)
    VirtualLock(pw, PAGE_SIZE);
    
    wchar_t* pwRepeat = (wchar_t*)VirtualAlloc(
        NULL, // starting address of the block to allocate
        // NULL – system determines where to allocate the block
        PAGE_SIZE, // size of the block (bytes)
        MEM_RESERVE | MEM_COMMIT, // type of memory allocation
        // MEM_COMMIT – allocate virtual addresses with real physycal pages of memory
        // MEM_RESERVE – reserves a range from the virtual address space
        PAGE_READWRITE // the memory protection
        // Program can read + write data
    );

    if (!pwRepeat) {
        cerr << "Memory allocation error!\n";

        SecureZeroMemory(pw, PAGE_SIZE);
        VirtualUnlock(pw, PAGE_SIZE);
        VirtualFree(pw, 0, MEM_RELEASE);

        return 0;
    }

    VirtualLock(pwRepeat, PAGE_SIZE);

    wcout << L"Enter new password: ";
    if (!readPassword(pw, PAGE_SIZE)) {
        cerr << "Error reading password";

        SecureZeroMemory(pw, PAGE_SIZE);
        VirtualUnlock(pw, PAGE_SIZE);
        VirtualFree(pw, 0, MEM_RELEASE);

        SecureZeroMemory(pwRepeat, PAGE_SIZE);
        VirtualUnlock(pwRepeat, PAGE_SIZE);
        VirtualFree(pwRepeat, 0, MEM_RELEASE);

        return 0;
    }
    wcout << L"\n";

    const size_t pwLen = wcslen(pw);

    wcout << L"Repeat new password: ";
    if (!readPassword(pwRepeat, PAGE_SIZE)) {
        cerr << "Error reading second password";

        SecureZeroMemory(pw, PAGE_SIZE);
        VirtualUnlock(pw, PAGE_SIZE);
        VirtualFree(pw, 0, MEM_RELEASE);
        
        SecureZeroMemory(pwRepeat, PAGE_SIZE);
        VirtualUnlock(pwRepeat, PAGE_SIZE);
        VirtualFree(pwRepeat, 0, MEM_RELEASE);

        return 0;
    }
    wcout << L"\n";

    bool matches = wcscmp(pw, pwRepeat) == 0;

    SecureZeroMemory(pwRepeat, PAGE_SIZE);
    VirtualUnlock(pwRepeat, PAGE_SIZE);
    VirtualFree(pwRepeat, 0, MEM_RELEASE);

    unsigned long finalHash = 0;

    if (!matches) {
        wcout << L"Passwords do not match!\n";
    } else if (!isGoodPassword(pw, pwLen)) {
        wcout << L"Password is too weak!\n"
             << L"Should include at least 8 characters long, "
             << L"uppercase, lowercase and numbers\n";
    } else {
        finalHash = hash(pw, pwLen);
        wcout << "Password successfully set!\n";
    }

    SecureZeroMemory(pw, PAGE_SIZE);
    VirtualUnlock(pw, PAGE_SIZE);
    VirtualFree(pw, 0, MEM_RELEASE);
    return finalHash;
}

/**
 * @brief Safely reads the password and verifies it against the correct hash.
 * 
 * Allocates locked virtual memory to prevent the password from being written to 
 * the disk (paging file), safely hashes the input, and guarantees that the 
 * plain-text password is wiped from RAM immediately after verification.
 * 
 * @param correctHash the expected valid hash.
 * @return @p correctHash == entered password hash or false on allocation failure.
 */
bool checkPassword(const unsigned long correctHash) {
    const SIZE_T PAGE_SIZE = 4096; // page size (bytes)

    wchar_t* inputPw = (wchar_t*)VirtualAlloc(
        NULL, // starting address of the block to allocate
              // NULL – system determines where to allocate the block
        PAGE_SIZE, // size of the block (bytes)
        MEM_RESERVE | MEM_COMMIT, // type of memory allocation
            // MEM_COMMIT – allocate virtual addresses with real physycal pages of memory
            // MEM_RESERVE – reserves a range from the virtual address space
        PAGE_READWRITE // the memory protection
            // Program can read + write data
    );
    
    if (!inputPw) {
        cerr << "Memory allocation error\n";
        return false;
    }

    VirtualLock(inputPw, PAGE_SIZE); // block the allocated page in the RAM

    wcout << "\nEnter password to log in: ";
    if (!readPassword(inputPw, PAGE_SIZE)) {
        cerr << "Error reading login password\n";

        SecureZeroMemory(inputPw, PAGE_SIZE);
        VirtualUnlock(inputPw, PAGE_SIZE);
        VirtualFree(inputPw, 0, MEM_RELEASE);

        return false;
    }
    wcout << L"\n";

    const size_t inputLen = wcslen(inputPw);
    const unsigned long inputHash = hash(inputPw, inputLen);

    const bool isMatch = inputHash == correctHash;

    SecureZeroMemory(inputPw, PAGE_SIZE); // fills memory of the page with zeros
    VirtualUnlock(inputPw, PAGE_SIZE);
    VirtualFree(inputPw, 0, MEM_RELEASE);

    return isMatch;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    system("cls");

    std::cout << "User registration\n";
    const unsigned long savedHash = setPassword();

    if (savedHash == 0) {
        cerr << L"Registration failed\n";
        return 1;
    }

    wcout << L"\nUser login\n";
    if (checkPassword(savedHash)) {
        wcout << L"Access is allowed\n";
    } else {
        wcout << L"Access denied! Incorrect password";
        return 1;
    }

    return 0;
}