/*
1. Використовуючи введення-виведення в стилі C створити файл 
та записати в нього структуровані дані.
2. Вивести створений файл на екран.
3. Видалити з файлу дані відповідно до варіанта.
4. Додати в файл дані відповідно до варіанта.
5. Вивести змінений файл на екран.

6. Використовуючи введення-виведення в стилі C++ створити файл 
та записати в нього структуровані дані.
7. Вивести створений файл на екран.
8. Видалити з файлу дані відповідно до варіанта.
9. Додати в файл дані відповідно до варіанта.
10. Вивести змінений файл на екран.

Структура даних "DVD-диск":
• назва фільма;
• режисер;
• тривалість;
• ціна.

Видалення: видалити всі елементи з ціною вище заданої.
Додавання: додати елемент з номером K.

1. Описати структури.
2. Відкрити файл для запису елементу.
3. Організувати інтерфейс для внесення значень полів елементів.
4. Організувати пошук.
5. Організувати інтерфейс виведення результату запита.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* FILENAME = "t.txt";
const unsigned int MAX_FILM_NAME = 100;
const unsigned int MAX_DIRECTOR_LENGTH = 30;
const unsigned int MAX_DURATION = 300;
const unsigned int MAX_PRICE = 10000;

typedef struct {
    char film_name[MAX_FILM_NAME + 1]; // '\0'
    char director[MAX_DIRECTOR_LENGTH + 1]; // '\0'
    unsigned int duration;
    unsigned int price;
} DVD_disk;

DVD_disk makeDVD();
void clearBuffer();
DVD_disk readDVD();
size_t countDVDs();
size_t readAllDVDs(DVD_disk[], const size_t);
void addDVD(DVD_disk);
void writeDVDs(DVD_disk[], size_t);
static void printHeader();
static void printRow(const DVD_disk);
static void printFooter();

int main() {
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

    DVD_disk dvds[countDVDs()];
    
    size_t DVDsCount = readAllDVDs(dvds, countDVDs());

    printHeader();
    for (size_t i = 0; i < DVDsCount; i++) printRow(dvds[i]);
    printFooter();

    printf("Натисніть Enter...");
    getchar();
    system("cls");

    unsigned int price = 0;
    printf("Введіть ціну: ");
    while (scanf("%u", &price) != 1 || price == 0 || price > MAX_PRICE) {
        fprintf(stderr, "\nПомилка! Введіть число від 1 до %u: ", MAX_PRICE);
        clearerr(stdin);
        clearBuffer();
    }
    clearBuffer();

    system("cls");
    
    DVD_disk newDVD = makeDVD();

    unsigned int k = 0;
    printf("Введіть номер, куди потрібно вставити новий диск: ");
    while (scanf("%u", &k) != 1 || k == 0 || k > DVDsCount + 1) {
        fprintf(stderr, "\nПомилка! Введіть число від 1 до %zu: ", DVDsCount + 1);
        clearerr(stdin);
        clearBuffer();
    }
    clearBuffer();

    k--;

    DVD_disk newDVDs[DVDsCount + 1];

    for (size_t i = 0; i < k; i++) newDVDs[i] = dvds[i];
    newDVDs[k] = newDVD;
    for (size_t i = k; i < DVDsCount; i++) newDVDs[i + 1] = dvds[i];

    DVDsCount++;

    size_t filteredCount = 0;
    for (size_t i = 0; i < DVDsCount; i++) {
        if (newDVDs[i].price <= price) filteredCount++;
    }

    DVD_disk filteredDVDs[filteredCount];
    for (size_t i = 0, j = 0; i < DVDsCount; i++) {
        if (newDVDs[i].price <= price) {
            filteredDVDs[j++] = newDVDs[i];
        }
    }

    writeDVDs(filteredDVDs, filteredCount);

    system("cls");
    printHeader();
    for (size_t i = 0; i < filteredCount; i++) printRow(filteredDVDs[i]);
    printFooter();

    return 0;
}

DVD_disk makeDVD() {
    DVD_disk dvd;
    printf("Назва фільму: ");
    while(fgets(dvd.film_name, sizeof(dvd.film_name), stdin) == NULL) {
        printf("\nПомилка! Введіть до %u символів: ", MAX_FILM_NAME);
        clearerr(stdin);
        clearBuffer();
    }
    dvd.film_name[strcspn(dvd.film_name, "\n")] = '\0';

    printf("Режисер: ");
    while (fgets(dvd.director, sizeof(dvd.director), stdin) == NULL
           || strlen(dvd.director) == 0
           || strlen(dvd.director) > MAX_DIRECTOR_LENGTH) {
        fprintf(stderr, "\nПомилка! Введіть до %u символів: ", 
            MAX_DIRECTOR_LENGTH);
        clearerr(stdin);
        clearBuffer();
    }
    dvd.director[strcspn(dvd.director, "\n")] = '\0';

    printf("Тривалість (хв): ");
    while (scanf("%u", &dvd.duration) != 1 
            || dvd.duration == 0
            || dvd.duration > MAX_DURATION) {
        fprintf(stderr, "\nПомилка! Введіть ціле число від 1 до %u: ",
            MAX_DURATION);
        clearerr(stdin);
        clearBuffer();
    }
    clearBuffer();

    printf("Ціна: ");
    while (scanf("%u", &dvd.price) != 1 || dvd.price > MAX_PRICE) {
        fprintf(stderr, "\nПомилка! Введіть число від 1 до %u: ", MAX_PRICE);
        clearerr(stdin);
        clearBuffer();
    }
    clearBuffer();

    return dvd;
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

DVD_disk readDVD() {
    DVD_disk dvd = {"", "", 0, 0};
    
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        fprintf(stderr, "Помилка! Перевірте, чи існує файл %s!\n", FILENAME);
        return dvd;
    }

    int parsed = fscanf(fp, "%[^;];%[^;];%u;%u\n", 
                        dvd.film_name, dvd.director, &dvd.duration, &dvd.price);

    // fscanf returns count of successfully read items (should be 4)
    if (parsed != 4) {
        fprintf(stderr, "Помилка! Некоректний формат даних у файлі!\n");
    }

    fclose(fp);
    return dvd;
}

size_t countDVDs() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) return 0;

    size_t count = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') count++;
    }

    fclose(fp);
    return count;
}

size_t readAllDVDs(DVD_disk dvds[], const size_t MAX_SIZE) {
    FILE* fp = fopen(FILENAME, "r");

    if (fp == NULL) {
        fprintf(stderr, "\nПомилка! Перевірте, чи існує файл %s!\n", FILENAME);
        return 0;
    }

    size_t count = 0;

    while (count < MAX_SIZE &&
        fscanf(fp, "%[^;];%[^;];%u;%u\n",
            dvds[count].film_name,
            dvds[count].director,
            &dvds[count].duration,
            &dvds[count].price
        )
    ) count++;

    fclose(fp);
    return count;
}

void addDVD(DVD_disk dvd) {
    FILE *fp = fopen(FILENAME, "a");
    
    if (fp == NULL) {
        fprintf(stderr, "\nПомилка відкриття файлу для запису!\n");
        return;
    }

    // fprintf(fp, "Фільм: %s.\nРежисер: %s.\nТривалість: %u.\nЦіна: %u грн.\n",
    //         dvd.film_name, dvd.director, dvd.duration, dvd.price);
    fprintf(fp, "%s;%s;%u;%u\n",
        dvd.film_name,
        dvd.director,
        dvd.duration,
        dvd.price);
    
    fclose(fp);
}

void writeDVDs(DVD_disk dvds[], size_t dvdsCount) {
    FILE *fp = fopen(FILENAME, "w");
    
    if (fp == NULL) {
        fprintf(stderr, "\nПомилка відкриття файлу для запису!\n");
        return;
    }
    
    for (size_t i = 0; i < dvdsCount; i++) {
        fprintf(fp, "%s;%s;%u;%u\n",
            dvds[i].film_name,
            dvds[i].director,
            dvds[i].duration,
            dvds[i].price
        );
    }
    fclose(fp);
}

static void printHeader() {
    printf("\n+------------------------------------------+--------------------------------+------------+-----------+\n");
    printf("| %-51s | %-37s | %-10s | %-13s |\n", "Назва фільму", "Режисер", "Тривалість", "Ціна");
    printf("+------------------------------------------+--------------------------------+------------+-----------+\n");
}

static void printRow(const DVD_disk dvd) {
    printf("| %-40.40s | %-30.30s | %-7u хв | %-5u грн |\n", 
           dvd.film_name, dvd.director, dvd.duration, dvd.price);
}

static void printFooter() {
    printf("+------------------------------------------+--------------------------------+------------+-----------+\n\n");
}