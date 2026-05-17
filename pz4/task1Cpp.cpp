/*
1. Використовуючи введення-виведення в стилі C++ створити файл 
та записати в нього структуровані дані.
2. Вивести створений файл на екран.
3. Видалити з файлу всі елементи з ціною вище заданої.
4. Додати в файл елемент з номером K.
5. Вивести змінений файл на екран.

Структура даних "DVD-диск":
• назва фільма;
• режисер;
• тривалість;
• ціна.

1. Описати структури.
2. Відкрити файл для запису елементу.
3. Організувати інтерфейс для внесення значень полів елементів.
4. Організувати пошук.
5. Організувати інтерфейс виведення результату запита.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <regex>

using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::setw;
using std::left;
using std::right;
using std::getline;

const string FILENAME = "t.txt";
constexpr size_t MAX_RECORDS = 100;
constexpr unsigned int MAX_FILM_NAME = 100;
constexpr unsigned int MAX_DIRECTOR_LENGTH = 30;
constexpr unsigned int MAX_DURATION = 300;
constexpr unsigned int MAX_PRICE = 10000;

struct DVD_disk {
    string filmName;
    string director;
    unsigned int duration;
    unsigned int price;
};

void clearBuffer();
unsigned int validUInt(const string&, const unsigned int, const unsigned int);
DVD_disk makeDVD();
size_t readAllDVDs(DVD_disk[], const size_t);
void addDVD(const DVD_disk&);
void writeDVDs(const DVD_disk[], size_t);
static void printHeader();
static void printRow(const DVD_disk&);
static void printFooter();

int main() {
    std::setlocale(LC_ALL, "uk_UA.UTF-8");

    DVD_disk data[] = {
        {"Avatar", "James Cameron", 162, 1234},
        {"Inception", "Christopher Nolan", 148, 880},
        {"The Matrix", "Lana & Lilly Wachowski", 136, 870},
        {"Se7en", "David Fincher", 127, 860},
        {"Back to the Future", "Robert Zemeckis", 116, 850},
        {"Alien", "Ridley Scott", 117, 850},
        {"The Fast and the Furious", "Rob Cohen", 106, 680},
        {"Now You See Me 2", "Jon M. Chu", 129, 640}
    };
    
    size_t length = sizeof(data) / sizeof(data[0]);
    for (size_t i = 0; i < length; i++) addDVD(data[i]);

    DVD_disk dvds[MAX_RECORDS];
    size_t DVDsCount = readAllDVDs(dvds, MAX_RECORDS);

    printHeader();
    for (size_t i = 0; i < DVDsCount; i++) printRow(dvds[i]);
    printFooter();

    cout << "Натисніть Enter...";
    cin.get();
    system("cls");

    const unsigned int target_price = validUInt("Введіть ціну: ", 1, MAX_PRICE);

    system("cls");
    
    DVD_disk newDVD = makeDVD();

    unsigned int k = validUInt("Введіть номер, куди потрібно вставити новий диск: ", 1, DVDsCount + 1);
    k--;

    DVD_disk newDVDs[MAX_RECORDS];

    for (size_t i = 0; i < k; i++) newDVDs[i] = dvds[i];
    newDVDs[k] = newDVD;
    for (size_t i = k; i < DVDsCount; i++) newDVDs[i + 1] = dvds[i];

    DVDsCount++;

    DVD_disk filteredDVDs[MAX_RECORDS];
    size_t filteredCount = 0;

    for (size_t i = 0; i < DVDsCount; i++) {
        if (newDVDs[i].price <= target_price) {
            filteredDVDs[filteredCount] = newDVDs[i];
            filteredCount++;
        }
    }

    writeDVDs(filteredDVDs, filteredCount);

    system("cls");
    printHeader();
    for (size_t i = 0; i < filteredCount; i++) printRow(filteredDVDs[i]);
    printFooter();

    return 0;
}

void clearBuffer() {
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

unsigned int validUInt(const string& message, const unsigned int MIN, const unsigned int MAX) {
    unsigned int value;
    cout << message;
    while (!(cin >> value) || value < MIN || value > MAX) {
        cerr << "\nПомилка! Введіть число від " << MIN << " до " << MAX << ": ";
        cin.clear();
        clearBuffer();
    }
    clearBuffer();
    return value;
}

DVD_disk makeDVD() {
    DVD_disk dvd;
    
    cout << "Назва фільму: ";
    getline(cin, dvd.filmName);
    
    cout << "Режисер: ";
    getline(cin, dvd.director);
    
    dvd.duration = validUInt("Тривалість (хв): ", 1, MAX_DURATION);
    dvd.price = validUInt("Ціна: ", 1, MAX_PRICE);
    
    return dvd;
}

size_t readAllDVDs(DVD_disk dvds[], const size_t MAX_SIZE) {
    std::ifstream f(FILENAME.data(), std::ios::in);
    
    if (!f.is_open()) {
        cerr << "Помилка! Перевірте, чи існує файл " << FILENAME << "!\n";
        return 0;
    }

    size_t count = 0;
    string line;
    
    // ([^;]+) - any characters except semicolon (group)
    // (\\d+)  - numbers (group)
    std::regex rgx(R"(([^;]+);([^;]+);(\d+);(\d+))");
    std::smatch match;

    while (count < MAX_SIZE && getline(f, line)) {
        if (line.empty()) continue;
        
        if (std::regex_match(line, match, rgx)) {
            // match[0] – the whole line
            // match[1], match[2], ... – groups
            dvds[count].filmName = match[1].str();
            dvds[count].director = match[2].str();
            dvds[count].duration = static_cast<unsigned int>(std::stoul(match[3].str()));
            dvds[count].price = static_cast<unsigned int>(std::stoul(match[4].str()));
            count++;
        }
    }
    return count;
}

void addDVD(const DVD_disk& dvd) {
    std::ofstream f(FILENAME.data(), std::ios::app);
    if (!f.is_open()) {
        cerr << "\nПомилка відкриття файлу для додавання!\n";
        return;
    }
    f << dvd.filmName << ";"
      << dvd.director << ";"
      << dvd.duration << ";"
      << dvd.price << "\n";
}

void writeDVDs(const DVD_disk dvds[], size_t count) {
    std::ofstream f(FILENAME.data(), std::ios::out);
    if (!f.is_open()) {
        cerr << "\nПомилка відкриття файлу для запису!\n";
        return;
    }
    for (size_t i = 0; i < count; i++) {
        f << dvds[i].filmName << ";"
          << dvds[i].director << ";"
          << dvds[i].duration << ";"
          << dvds[i].price << "\n";
    }
}

void printHeader() {
    cout << "\n+------------------------------------------+--------------------------------+------------+-----------+\n";
    cout << "| " << left << setw(51) << "Назва фільму" 
        << " | " << setw(37) << "Режисер" 
        << " | " << setw(10) << "Тривалість" 
        << " | " << setw(13) << "Ціна" << " |\n";
    cout << "+------------------------------------------+--------------------------------+------------+-----------+\n";
}

void printRow(const DVD_disk& dvd) {    
    cout << "| " << left << setw(40) << dvd.filmName.substr(0, 40) 
        << " | " << setw(30) << dvd.director.substr(0, 30) 
        << " | " << right << setw(7) << dvd.duration << " хв" 
        << " | " << setw(5) << dvd.price << " грн |\n";
}

void printFooter() {
    cout << "+------------------------------------------+--------------------------------+------------+-----------+\n\n";
}