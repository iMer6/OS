#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::string;

// ASCII file processing function
void ProcessAscii(const string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    string line;
    int lineCount = 0;
    cout << "ASCII file\n";
    while (std::getline(file, line)) {
        lineCount++;
        cout << "Line " << lineCount
             << ". Length: " << line.length() << " characters\n";
    }
    cout << "Total lines: " << lineCount << "\n";
}

// UNICODE (UTF-16) file processing function
void ProcessUnicode(const string& filepath) {
    std::wifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return;

    // Pass BOM (2 bytes) by Reposition the file pointer
    file.seekg(2);

    std::wstring line;
    int lineCount = 0;
    cout << "Unicode file\n";
    while (std::getline(file, line)) {
        // Remove '\r' if it is at the end of a line
        if (!line.empty() && line.back() == L'\r') line.pop_back();
        lineCount++;
        cout << "Line " << lineCount
             << ". Length: " << line.length() << " characters\n";
    }
    cout << "Total lines: " << lineCount << "\n";
}

void AnalyzeFile(const string& folder, const string& filename) {
    string fullPath = folder + "\\" + filename;

    // Get the file size via WinAPI
    HANDLE hFile = CreateFileA(
        fullPath.c_str(), // file path
        GENERIC_READ, // file opening mode (read)
        FILE_SHARE_READ, // sharing mode
            // Other processes can read the file at the same time
        NULL, // the handle cannot be inherited (default)
        OPEN_EXISTING, // opens the file only if it exists
            // If the file doesn't exist ==> error
        FILE_ATTRIBUTE_NORMAL, // the file doesn't have other attributes set
        NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
    );
    if (hFile == INVALID_HANDLE_VALUE) return;
    
    LARGE_INTEGER fileSize;
    GetFileSizeEx(hFile, &fileSize);
    CloseHandle(hFile);

    cout << "\nFile: " << filename
         << ". Size: " << fileSize.QuadPart << " bytes\n";

    // BOM coding check
    std::ifstream checkFile(fullPath, std::ios::binary);
    unsigned char bom[2] = {0, 0};
    checkFile.read((char*)bom, 2);
    checkFile.close();

    if (bom[0] == 0xFF && bom[1] == 0xFE) {
        ProcessUnicode(fullPath);
    } else {
        ProcessAscii(fullPath);
    }
}

int main() {
    cout << "Program 2 starting\n";
    string folder = "C:\\C++\\OS\\lr7\\files";
    string searchPath = folder + "\\*.*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // Ignore system directories ("." and "..")
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                AnalyzeFile(folder, findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    } else {
        cout << "Folder " << folder << " not found or empty.\n";
    }

    cout << "Program 2 finished.\n";
    system("pause");
    return 0;
}