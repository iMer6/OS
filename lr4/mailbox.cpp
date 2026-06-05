#include <windows.h>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cerr;

const DWORD MAX_BOX_BUFFER = 4096; // max allowed buffer size

struct MailboxHeader {
    DWORD message_count;
    DWORD total_size;
    DWORD max_size;
    DWORD crc32; // Cyclic Redundancy Check
};

struct MessageHeader {
    DWORD size; 
};

class Mailbox {
private:
    string folder_name;
    string file_path;

    DWORD CalculateCRC32(const BYTE* data, DWORD length) {
        DWORD crc = 0xFFFFFFFF; // standart
        for (DWORD i = 0; i < length; ++i) {
            crc ^= data[i];
            for (int j = 0; j < 8; ++j) {
                if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320;
                // 0xEDB88320 – polynom IEEE 802.3 CRC32
                // Uniform distribution of hashes
                else crc >>= 1;
            }
        }
        return ~crc; // NOT operation
    }

    void UpdateIntegrity(HANDLE hFile) {
        DWORD fileSize = GetFileSize(hFile, NULL); // file size
        if (
            fileSize == INVALID_FILE_SIZE
            || fileSize < sizeof(MailboxHeader)
            || fileSize > MAX_BOX_BUFFER
        ) return;

        BYTE buffer[MAX_BOX_BUFFER] = { 0 };
        
        SetFilePointer(
            hFile,
            0,
            NULL,
            FILE_BEGIN
        );
        DWORD bytesRead;
        ReadFile(
            hFile,
            buffer,
            fileSize,
            &bytesRead,
            NULL
        ); 

        // Reset the CRC field before counting
        MailboxHeader* header = reinterpret_cast<MailboxHeader*>(buffer);
        // reinterpret_cast – bitwise conversion between types
        header->crc32 = 0;

        DWORD newCrc = CalculateCRC32(buffer, fileSize);
        
        // Write the new CRC value back to the file at the desired offset
        SetFilePointer(
            hFile, // file handle (where need to change position?)
            offsetof( // offset in bytes of crc32 of MailboxHeader
                MailboxHeader,
                crc32
            ), // how many bytes the pointer needs to be moved?
            NULL, // requierd for files > 4Gb
                  // NULL – 64-bit shift is not needed
            FILE_BEGIN // countdown starts from the ending of the file
        ); // set pointer to start
        DWORD bytesWritten;
        WriteFile(
            hFile, // a handle to the file
            &newCrc, // where to read the data from?
            sizeof(DWORD), // how many bytes needs to be written?
            &bytesWritten, // where to write the number of written bytes?
            NULL // if the file was opened without FILE_FLAG_OVERLAPPED must be NULL
        );
    }

public:
    Mailbox(string folder, string filename) {
        folder_name = folder;
        file_path = folder + "\\" + filename;
    }

    // Create (open) files and directories
    bool Create(DWORD max_size) {
        CreateDirectoryA(
            folder_name.c_str(), // the path of the directory
            NULL // default security descriptor
        );

        HANDLE hFile = CreateFileA(
            file_path.c_str(), // file path
            GENERIC_READ | GENERIC_WRITE,  // file opening mode (read + write)
            0, // sharing mode
               // 0 – prevents other processes from opening a file
               // if they request delete, read, or write access
            NULL, // the handle cannot be inherited (default)
            CREATE_ALWAYS, // an action to take on a file that exists (or not)
                // If the file does not exist, it will be created.
                // If it does, it will be overwritten (old data will be deleted)
            FILE_ATTRIBUTE_NORMAL, // the file doesn't have other attributes set
            NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
        ); // create and open file in ASCII
        
        if (hFile == INVALID_HANDLE_VALUE) return false;

        // Initializing the initial header
        MailboxHeader header = { 0, 0, max_size, 0 };
        DWORD bytesWritten;
        WriteFile(
            hFile, // a handle to the file
            &header, // where to read the data from?
            sizeof(MailboxHeader), // how many bytes do need to write?
            &bytesWritten, // where to write the number of written bytes
            NULL // if the file was opened without FILE_FLAG_OVERLAPPED must be NULL
        ); // write data in the file
        
        UpdateIntegrity(hFile);
        CloseHandle(hFile); // close file, deallocate memory
        return true;
    }

    bool CheckIntegrity() {
        // Open the file
        HANDLE hFile = CreateFileA(
            file_path.c_str(), // file path
            GENERIC_READ, // file opening mode (read)
            FILE_SHARE_READ, // sharing mode (read)
                // Other processes can read the file at the same time
            NULL, // the handle cannot be inherited (default)
            OPEN_EXISTING, // opens the file only if it exists
                // If the file doesn't exist ==> error
            FILE_ATTRIBUTE_NORMAL, // the file doesn't have other attributes set
            NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
        );

        if (hFile == INVALID_HANDLE_VALUE) return false;

        // Size check
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize > MAX_BOX_BUFFER) {
            CloseHandle(hFile); // close file, deallocate memory
            return false;
        }

        // Read data from file to buffer
        BYTE buffer[MAX_BOX_BUFFER] = { 0 };
        DWORD bytesRead;
        ReadFile(
            hFile, // where to read data from?
            buffer, // where to write the read data?
            fileSize, // buffer size (how many bytes needs to be read?)
            &bytesRead, // where to record the number of characters read?
            NULL // synchronous mode
        );
        CloseHandle(hFile); // close file, deallocate memory

        MailboxHeader* header = reinterpret_cast<MailboxHeader*>(buffer);
        DWORD originalCrc = header->crc32;
        header->crc32 = 0;

        DWORD calculatedCrc = CalculateCRC32(buffer, fileSize);
        return originalCrc == calculatedCrc;
    }

    // Function of adding letters
    bool AddMessage(const string& body) {
        if (!CheckIntegrity()) {
            cout << "Error: mailbox integrity has been broken!\n";
            return false;
        }

        HANDLE hFile = CreateFileA(
            file_path.c_str(), // file path
            GENERIC_READ | GENERIC_WRITE, // file opening mode (read + write)
            0, // sharing mode
               // 0 – prevents other processes from opening a file
               // if they request delete, read or write access
            NULL, // the handle cannot be inherited (default)
            OPEN_EXISTING, // opens the file only if it exists
                // If the file doesn't exist ==> error
            FILE_ATTRIBUTE_NORMAL, // the file doesn't have other attributes set
            NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
        ); // open file

        if (hFile == INVALID_HANDLE_VALUE) return false;

        // Read header
        MailboxHeader header;
        DWORD bytesRead;
        ReadFile(
            hFile, // where to read data from?
            &header, // where to write the read data?
            sizeof(MailboxHeader), // buffer size (how many bytes need to be read?)
            &bytesRead, // where to record the number of characters read?
            NULL // synchronous mode
        );

        DWORD msgSize = body.length();
        DWORD totalRequiredSpace = sizeof(MessageHeader) + msgSize;

        // Check max mailbox size
        if (header.total_size + totalRequiredSpace > header.max_size) {
            cout << "Error: mailbox overflow. Max: " << header.max_size << " bytes.\n";
            CloseHandle(hFile); // close file, deallocate memory
            return false;
        }

        // Move the pointer to the end of the file to write a new letter
        SetFilePointer(
            hFile, // file handle (where need to change position?)
            0, // how many bytes the pointer needs to be moved?
            NULL, // required for files > 4Gb
                  // NULL – 64-bit shift is not needed
            FILE_END // countdown starts from the ending of the file 
        );
        
        DWORD bytesWritten;
        MessageHeader msgHeader = { msgSize }; // message size
        // Write header
        WriteFile(
            hFile, // a handle to the file
            &msgHeader, // where to read the data from?
            sizeof(MessageHeader), // how many bytes need to be written?
            &bytesWritten, // where to write the number of written bytes?
            NULL // if the file was opened without FILE_FLAG_OVERLAPPED must be NULL
        );
        // Write body
        WriteFile(
            hFile, // a handle to the file
            body.c_str(), // where to read the data from?
            msgSize, // how many bytes need to be written?
            &bytesWritten, // where to write the number of written bytes?
            NULL // if the file was opened without FILE_FLAG_OVERLAPPED must be NULL
        ); // message body

        // Title modification
        header.message_count++;
        header.total_size += totalRequiredSpace;

        // Update header
        SetFilePointer(
            hFile, // file handle (where need to change position?)
            0, // how many bytes the pointer needs to bs moved?
            NULL, // required for files > 4Gb
                  // NULL – 64-bit shift is not needed
            FILE_BEGIN // countdown starts from the beginning of the file
        );
        WriteFile(
            hFile, // a handle to the file
            &header, // where to read the data from?
            sizeof(MailboxHeader), // how many bytes need to be written?
            &bytesWritten, // where to write the number of written bytes?
            NULL // if the file was opened without FILE_FLAG_OVERLAPPED must be NULL
        );

        UpdateIntegrity(hFile);
        CloseHandle(hFile); // close file, deallocate memory
        return true;
    }

    // Function of reading emails with and without deletion
    void ReadMessages(bool delete_after_read) {
        if (!CheckIntegrity()) {
            cout << "Error: file broken.!\n";
            return;
        }

        HANDLE hFile = CreateFileA(
            file_path.c_str(), // file path
            GENERIC_READ, // file opening mode (read)
            FILE_SHARE_READ, // sharing mode (read)
                // Other processes can read the file at the same time
            NULL, // the handle cannot be inherited (default)
            OPEN_EXISTING, // opens the file only if it exists
                // If the file doesn't exist ==> error
            FILE_ATTRIBUTE_NORMAL, // the file doesn't have other attributes set
            NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
        ); // open file

        if (hFile == INVALID_HANDLE_VALUE) return;

        // Read header
        MailboxHeader header;
        DWORD bytes;
        ReadFile(
            hFile, // where to read data from?
            &header, // where to store the read data?
            sizeof(MailboxHeader), // buffer size (how many bytes need to be read?)
            &bytes, // where to store the number of characters read?
            NULL // synchronous mode
        );

        cout << "\nList of letters (total: " << header.message_count << ")\n";
        
        // Read letters 
        for (DWORD i = 0; i < header.message_count; ++i) {
            // Read letter header
            MessageHeader msgHeader;
            ReadFile(
                hFile, // where to read data from?
                &msgHeader, // where to store the read data?
                sizeof(MessageHeader), // buffer size (how many bytes need to be read?)
                &bytes, // where to store the number of characters read?
                NULL // synchronous mode
            );

            // Read letter body
            char bodyBuffer[512] = { 0 };
            DWORD readSize = msgHeader.size < 511 ? msgHeader.size : 511;
            ReadFile(
                hFile, // where to read data from?
                bodyBuffer, // where to store the read data?
                readSize, // buffer size (how many bytes need to be read?)
                &bytes, // where to store the number of characters read?
                NULL // synchronous mode
            );

            cout << "Letter " << i + 1 << " [" << msgHeader.size + 4 << " bytes]: " << bodyBuffer << "\n";
        }
        CloseHandle(hFile); // close file, deallocate memory

        if (delete_after_read && header.message_count > 0) {
            DeleteAllMessages();
            cout << "All letters have been deleted after reading.\n";
        }
    }

    // Delete letter by ID
    bool DeleteMessage(DWORD index) {
        if (!CheckIntegrity()) return false;

        HANDLE hFile = CreateFileA(
            file_path.c_str(), // file path
            GENERIC_READ | GENERIC_WRITE, // file opening mode (read + write)
            0, // sharing mode
               // 0 – prevents other processes from opening a file
               // if they request delete, read or write access
            NULL, // the handle cannot be inherited (default)
            OPEN_EXISTING, // opens the file only if it exists
                // If the file doesn't exist ==> error
            FILE_ATTRIBUTE_NORMAL, // the file doestn't have other attributes set
            NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
        );

        if (hFile == INVALID_HANDLE_VALUE) return false;

        // Read header
        MailboxHeader header;
        DWORD bytes;
        ReadFile(
            hFile, // where to read data from?
            &header, // where to store the read data?
            sizeof(MailboxHeader), // buffer size (how many bytes to be read?)
            &bytes, // where to store the number of characters read?
            NULL // synchronous mode
        );

        // Validate message ID
        if (index == 0 || index > header.message_count) {
            cout << "Error: invalid letter number!\n";
            CloseHandle(hFile);
            return false;
        }

        // Delete letter by ID
        // by skip unnecessary letter
        // Info about other letters writing in temp buffers
        // Update old info  
        BYTE newFileBuffer[MAX_BOX_BUFFER] = { 0 };
        DWORD currentBufferPos = sizeof(MailboxHeader);
        DWORD currentMsgSizeSum = 0;

        for (DWORD i = 1; i <= header.message_count; ++i) {
            MessageHeader msgHeader;
            ReadFile(
                hFile, // where to read data from?
                &msgHeader, // where to store the read data?
                sizeof(MessageHeader), // buffer size (how many bytes to be read?)
                &bytes, // where to store the number of characters read?
                NULL // synchronous mode
            );

            char tempBody[512] = { 0 };
            ReadFile(
                hFile, // where to read data from?
                tempBody, // where to store the read data?
                msgHeader.size, // buffer size (how many bytes to be read?)
                &bytes, // where to store the number of characters read?
                NULL // synchronous mode
            );

            // Delete <==> skip
            if (i != index) {
                // Copy the other letters to static buffer
                memcpy(
                    newFileBuffer + currentBufferPos, // copy to
                    &msgHeader, // copy from
                    sizeof(MessageHeader) // number of bytes to copy
                );
                currentBufferPos += sizeof(MessageHeader);

                memcpy(
                    newFileBuffer + currentBufferPos, // copy to
                    tempBody, // copy from
                    msgHeader.size // number of bytes to copy
                );
                currentBufferPos += msgHeader.size;

                currentMsgSizeSum += sizeof(MessageHeader) + msgHeader.size;
            }
        }

        // Update main header
        MailboxHeader* pNewHeader = reinterpret_cast<MailboxHeader*>(newFileBuffer);
        pNewHeader->message_count = header.message_count - 1;
        pNewHeader->total_size = currentMsgSizeSum;
        pNewHeader->max_size = header.max_size;
        pNewHeader->crc32 = 0;

        // Overwrite the entire file from the static array
        SetFilePointer(
            hFile, // file handle (where need to change position?)
            0, // how many bytes should the pointer be shifted?
            NULL, // required for files > 4Gn
                  // NULL – 64-bit shift is not needed
            FILE_BEGIN // countdown starts from the beginning of the file
        );
        WriteFile(
            hFile, // a handle to the file
            newFileBuffer, // where to read the data from?
            currentBufferPos, // how many bytes need to be written?
            &bytes, // where to write the number of written bytes?
            NULL // if the file was opened without FILE_FLAG_OVERLAPPED must be NULL
        );

        // Setting pointer the end of the file when reducing its size
        SetFilePointer(
            hFile, // file handle (where need to change position?)
            currentBufferPos, // how many bytes should the pointer be shifted?
            NULL, // required for files > 4Gb
                  // NULL – 64-bit shift is not needed
            FILE_BEGIN // countdown starts from the beginning of the file
        );
        SetEndOfFile(hFile); // set size of the file

        UpdateIntegrity(hFile);
        CloseHandle(hFile); // close file, deallocate memory
        return true;
    }

    void DeleteAllMessages() {
        HANDLE hFile = CreateFileA(
            file_path.c_str(), // file path
            GENERIC_READ | GENERIC_WRITE, // file opening mode (read + write)
            0, // sharing mode
               // 0 – prevents other processes from opening a file
               // if they request delete, read, or write access
            NULL, // the handle cannot be inherit (default)
            OPEN_EXISTING, // opens the file only if it exists
                // If the file doesn't exist ==> error
            FILE_ATTRIBUTE_NORMAL, // the file doesn't have other attributes set
            NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
        );

        if (hFile == INVALID_HANDLE_VALUE) return;

        // Read header
        MailboxHeader header;
        DWORD bytes;
        ReadFile(
            hFile, // where to read data from?
            &header, // where to store the read data?
            sizeof(MailboxHeader), // buffer size (how many bytes to be read?)
            &bytes, // where to store the number of characters read?
            NULL // synchronous mode
        );

        // Update info
        header.message_count = 0;
        header.total_size = 0;
        header.crc32 = 0;

        // Trim the file
        SetFilePointer(
            hFile, // file handle (where need to change position?)
            0, // how many bytes should the pointer be shifted?
            NULL, // required for files > 4Gb
                  // NULL – 64-bit shift is not needed
            FILE_BEGIN // countdown starts from the beginning of the file
        );
        WriteFile(
            hFile, // a handle to the file
            &header, // where to read the data from?
            sizeof(MailboxHeader), // how many bytes need to be written?
            &bytes, // where to write the number of written bytes?
            NULL // if the file was opened without FILE_FLAG_OVERLAPPED must be NULL
        );
        SetEndOfFile(hFile); // trim the file to the size of just the header

        UpdateIntegrity(hFile);
        CloseHandle(hFile);
    }

    // Determine the number of letters and display the status
    void PrintInfo() {
        HANDLE hFile = CreateFileA(
            file_path.c_str(), // file path
            GENERIC_READ, // file opening mode (read)
            FILE_SHARE_READ, // sharing mode (read)
                // Other processes can read the file at the same time
            NULL, // the handle cannot be inherit (default)
            OPEN_EXISTING, // opens the file only if it exists
                // If the file doesn't exist ==> error
            FILE_ATTRIBUTE_NORMAL, // the file doesn't have other attributes set
            NULL // don't ignore parameter 6 (FILE_ATTRIBUTE_NORMAL)
        ); // open file

        if (hFile == INVALID_HANDLE_VALUE) return;

        // Read memory
        MailboxHeader header;
        DWORD bytes;
        ReadFile(
            hFile, // where to read data from?
            &header, // where to store the read data?
            sizeof(MailboxHeader), // buffer size (how many bytes to be read?)
            &bytes, // where to store the number of characters read?
            NULL // synchronous mode
        );

        CloseHandle(hFile); // close file, deallocate memory

        cout << "\nMailbox\n"
             << "Total letters: " << header.message_count << "\n"
             << "Used space: " << header.total_size << " / " << header.max_size << " bytes\n"
             << "Цілісність даних: " << (CheckIntegrity() ? "OK" : "FAILED") << "\n\n";
    }

    // Determining the total number of mailboxes (FindFirstFile/FindNextFile)
    static int GetTotalMailboxesCount(const string& folder) {
        string search_path = folder + "\\*.txt";
        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA(
            search_path.c_str(), // file path
            &findData // buffer (where to store info about the found file?)
        ); // file search

        if (hFind == INVALID_HANDLE_VALUE) return 0;

        int count = 0;
        do {
            if (
                !(findData.dwFileAttributes
                & FILE_ATTRIBUTE_DIRECTORY)
            ) count++;
        } while (
            FindNextFileA(
                hFind, // a search handle
                &findData // buffer (where to store info about the found file?)
            )
        ); // search for the following files

        FindClose(hFind); // close search
        return count;
    }
};

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    system("cls");

    string folder = "Mail";
    Mailbox box(folder, "mailbox.txt");

    cout << "Create mailbox об'ємом 1000 bytes\n";
    box.Create(1000);

    box.AddMessage("First message.");
    box.AddMessage("Second message.");
    box.AddMessage("Third message.");

    box.PrintInfo();
    box.ReadMessages(false);

    cout << "\nDelete letter 2\n";
    box.DeleteMessage(2);

    box.PrintInfo();
    box.ReadMessages(false);

    cout << "\nTotal mailboxes: " 
         << Mailbox::GetTotalMailboxesCount(folder) << "\n";

    // Reading with deletion
    // cout << "\nRead letters with deletion:\n";
    // box.ReadMessages(true);

    box.PrintInfo();

    return 0;
}