#include "common.h"

int LAST_BOOK_ID = 0;

user_t* CURRENT_USER = NULL;
book_sort MAIN_BOOK_SORT = ID_SORT;

user_t* USER_LIST = NULL;
size_t ULENGTH = 0;

book_t* BOOK_LIST = NULL;
size_t BLENGTH = 0;

// -                                                      - //
// ---------------------- Comparator ---------------------- //
// -                                                      - //

int comparebook_asc(const book_t* a, const book_t* b) {
    switch (MAIN_BOOK_SORT) {
        case ID_SORT:
            return a->id - b->id;
        case TITLE_SORT:
            return strcmp(a->title, b->title);
        case AUTHOR_SORT:
            return strcmp(a->author, b->author);
        case PAGES_SORT:
            return a->pages - b->pages;
        case AVAILABILITY_SORT:
            if (!isbook_borrowed(*a) && !isbook_borrowed(*b))
                return a->id - b->id;
            else
                return !isbook_borrowed(*b);
    }

    return a->id - b->id;
}

int comparebook_desc(const book_t* a, const book_t* b) {
    return comparebook_asc(b, a);
}

int compareuser(const user_t* a, const user_t* b) {
    return strcmp(a->name, b->name);
}

// -                                                     - //
// ---------------------- Functions ---------------------- //
// -                                                     - //

void* book_comparator(sort_type type) {
    if (type == ASCENDING)
        return comparebook_asc;
    else
        return comparebook_desc;
}

int createuser(char name[], char password[], bool isadmin, int* book_ids, int book_count) {
    user_t user;

    strcpy(user.name, name);
    strcpy(user.password, password);
    user.isadmin = isadmin;

    // Membersihkan bytes/memory pada array book_ids
    memset(user.book_ids, 0, sizeof(int) * MAX_BORROW);
    user.book_count = 0;

    // Memasukkan id-id buku dan juga jumlahnya pada atribut yg dituju
    if (book_ids != NULL && book_count > 0) {
        memcpy(user.book_ids, book_ids, sizeof(int) * book_count);
        user.book_count = book_count;
    }

    // menambahkan user ke dalam database buku
    ULENGTH++;
    USER_LIST = safe_alloc(USER_LIST, ULENGTH, sizeof(user_t));
    USER_LIST[ULENGTH - 1] = user;

    return ULENGTH - 1;
}

int createbook(int id, char title[], char author[], int pages, char* borrower, time_t duetime) {
    book_t book;

    strcpy(book.title, title);
    strcpy(book.author, author);

    if (borrower != NULL && strlen(borrower) != 0)
        strcpy(book.borrower, borrower);

    book.id = id;
    book.pages = pages;
    book.duetime = duetime;

    // menambahkan buku ke dalam database buku
    BLENGTH++;
    BOOK_LIST = safe_alloc(BOOK_LIST, BLENGTH, sizeof(book_t));
    BOOK_LIST[BLENGTH - 1] = book;

    return BLENGTH - 1;
}

bool removebook(int id) {
    if (id <= 0) {
        printf("Buku ini tidak dapat ditemukan\n");
        return false;
    }

    int idx = findbook(id);
    if (idx == -1) {
        printf("Buku ini tidak dapat ditemukan\n");
        return false;
    }

    book_t* found = &BOOK_LIST[idx];
    if (isbook_borrowed(*found)) {
        printf("Buku yang sedang dipinjam tidak dapat dihapus!\n");
        return false;
    }

    char bookpath[FILENAME_MAX];
    sprintf(bookpath, BOOK_DATABASE_PATH "/%d", found->id);
    delete_recursively(bookpath);

    BLENGTH--;
    if ((size_t)idx != BLENGTH)
        memcpy((BOOK_LIST + idx), (BOOK_LIST + idx + 1), sizeof(book_t) * (BLENGTH - idx));

    BOOK_LIST = safe_alloc(BOOK_LIST, BLENGTH, sizeof(book_t));

    printf("Buku berhasil dihapuskan dari database!\n");
    save_books();
    return true;
}

bool isbook_borrowed(book_t book) {
    return strlen(book.borrower) != 0 && strcmp(book.borrower, "-") != 0 && book.duetime != 0;
}

int finduser(char name[]) {
    int left = 0;
    int right = ULENGTH - 1;
    int mid = 0;

    quicksort_user(USER_LIST, ULENGTH, compareuser);

    do {
        mid = (left + right) / 2;
        user_t* temp = &USER_LIST[mid];

        int compare = strcmp(name, temp->name);
        if (compare == 0)
            return mid;

        if (compare < 0)
            right = mid - 1;
        if (compare > 0)
            left = mid + 1;
    } while (left <= right);

    return -1;
}

int findbook(int id) {
    book_sort before = MAIN_BOOK_SORT;
    MAIN_BOOK_SORT = ID_SORT;

    quicksort_book(BOOK_LIST, BLENGTH, book_comparator(ASCENDING));
    MAIN_BOOK_SORT = before;

    int left = 0;
    int right = BLENGTH - 1;
    int mid = 0;

    do {
        mid = (left + right) / 2;
        book_t* temp = &BOOK_LIST[mid];

        if (id == temp->id)
            return mid;

        if (id < temp->id)
            right = mid - 1;
        if (id > temp->id)
            left = mid + 1;
    } while (left <= right);

    return -1;
}

int findbook_title(char title[]) {
    book_sort before = MAIN_BOOK_SORT;
    MAIN_BOOK_SORT = TITLE_SORT;

    quicksort_book(BOOK_LIST, BLENGTH, book_comparator(ASCENDING));
    MAIN_BOOK_SORT = before;

    int left = 0;
    int right = BLENGTH - 1;
    int mid = 0;

    do {
        mid = (left + right) / 2;
        book_t* temp = &BOOK_LIST[mid];

        int compare = strcmp(title, temp->title);
        if (compare == 0)
            return mid;

        if (compare < 0)
            right = mid - 1;
        if (compare > 0)
            left = mid + 1;
    } while (left <= right);

    return -1;
}

bookpaginate_t book_paginate(book_t* arr, int length, book_sort name, sort_type type, int page) {
    int maxpage = (length / ELEMENTS_PER_PAGE) + (length % ELEMENTS_PER_PAGE == 0 ? 0 : 1);
    int len = ELEMENTS_PER_PAGE;

    // Mengecek halaman yang dipilih
    if (length == 0 || page < 1 || page > maxpage) {
        bookpaginate_t empty;

        empty.len = 0;
        empty.list = NULL;

        return empty;
    }

    // Menentukan jumlah buku yg akan dibagi untuk halaman ini
    if (page == maxpage) {
        len = length % ELEMENTS_PER_PAGE;

        if (len == 0)
            len = ELEMENTS_PER_PAGE;
    }

    // mengurutkan database buku
    MAIN_BOOK_SORT = name;
    quicksort_book(BOOK_LIST, BLENGTH, book_comparator(type));

    // membuat sebuah bookpack sebagai informasi/data dari pembuatan halaman ini
    bookpaginate_t pack;
    pack.len = len;
    pack.list = (arr + (ELEMENTS_PER_PAGE * (page - 1)));
    pack.maxpage = maxpage;

    return pack;
}

void view_book(book_t* book) {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔══════════════════════════════════════════════════════╗\n"
        L"║                     Detail  Buku                     ║\n"
        L"╠══════════════════════════════════════════════════════╣\n"
        L"║                                                      ║\n"
        L"║ · Judul   : %-40s ║\n"
        L"║ · Penulis : %-40s ║\n"
        L"║ · Halaman : %-40d ║\n"
        L"║ · Tersedia: %-40s ║\n"
        L"║                                                      ║\n"
        L"╚══════════════════════════════════════════════════════╝\n",
        book->title,
        book->author,
        book->pages,
        isbook_borrowed(*book) ? "No" : "Yes");

    set_default_encoding(stdout);
}

int select_booksort() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                  URUTAN  BUKU                  ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ [1] Berdasarkan ID buku                        ║\n"
        L"║ [2] Berdasarkan judul buku                     ║\n"
        L"║ [3] Berdasarkan penulis buku                   ║\n"
        L"║ [4] Berdasarkan jumlah halaman buku            ║\n"
        L"║ [5] Berdasarkan status peminjaman              ║\n"
        L"║ [0] Kembali                                    ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);
    bool isvalid = true;

    do {
        int choice = scan_number("Pilihan [0-5] >> ");

        if (choice >= 0 && choice <= 5) {
            return choice - 1;
        } else {
            isvalid = false;
            printf("Pilihan tidak dapat ditemukan!\n");
        }
    } while (!isvalid);

    return -1;
}

int select_sorttype() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                TIPE  PENGURUTAN                ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ [1] Urutan ke atas                             ║\n"
        L"║ [2] Urutan ke bawah                            ║\n"
        L"║ [0] Kembali                                    ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);
    bool isvalid = true;

    do {
        int choice = scan_number("Pilihan [0-2] >> ");

        if (choice >= 0 && choice <= 2) {
            return choice - 1;
        } else {
            isvalid = false;
            printf("Pilihan tidak dapat ditemukan!\n");
        }
    } while (!isvalid);

    return -1;
}

bool await_confirmation(char* message) {
    if (message == NULL || strlen(message) == 0) {
        printf("Gagal dalam membuat konfirmasi!\n");
        return false;
    }

    printf(
        "%s"
        "\n"
        "(Y) Ya\n"
        "(N) Tidak\n", message);

    char answer = 0;
    do {
        printf(">> ");
        answer = toupper(getchar());
        fflush(stdin);

        if (answer != 'Y' && answer != 'N') {
            printf("Pilihan tidak valid!\n");
            continue;
        }

        break;
    } while (true);

    return answer == 'Y';
}

void save_books() {
    mkdir(BOOK_DATABASE_PATH);

    FILE* lastidfile = fopen(BOOK_DATABASE_PATH "/lastid.txt", "w");
    fprintf(lastidfile, "%d\n", LAST_BOOK_ID);
    fclose(lastidfile);

    size_t i = 0;
    for (i = 0; i < BLENGTH; i++) {
        book_t tmp = BOOK_LIST[i];

        int buf = FILENAME_MAX + strlen(BOOK_DATABASE_PATH);

        char path[buf];
        sprintf(path, BOOK_DATABASE_PATH "/%d", tmp.id);
        mkdir(path);

        buf += 13;

        char metadata[buf];

        strcpy(metadata, path);
        strcat(metadata, "/");
        strcat(metadata, "metadata.txt");

        FILE* curfile = fopen(metadata, "w");
        fprintf(curfile, "%d;%s;%s;%d;%s;%lld\n",
                tmp.id, tmp.title, tmp.author, tmp.pages, tmp.borrower, tmp.duetime);
        fclose(curfile);
    }
}

void load_books() {
    FILE* flastid = fopen(BOOK_DATABASE_PATH "/lastid.txt", "r");
    if (flastid != NULL) {
        fscanf(flastid, "%d", &LAST_BOOK_ID);
        fclose(flastid);
    } else {
        LAST_BOOK_ID = 0;
    }

    DIR* folder = opendir(BOOK_DATABASE_PATH);
    if (folder == NULL)
        return;

    struct dirent* readd = NULL;
    int count = 0;

    while ((readd = readdir(folder)) != NULL) {
        count++;

        if (count <= 2)
            continue;
        if (strstr(readd->d_name, ".txt") != NULL)
            continue;

        int buf = FILENAME_MAX + strlen(BOOK_DATABASE_PATH);

        int rawid = atoi(readd->d_name);
        char metadatapath[buf + 13];

        sprintf(metadatapath, BOOK_DATABASE_PATH "/%d/metadata.txt", rawid);

        int id, pages;
        char title[MAXNAME_LENGTH],
            author[MAXNAME_LENGTH],
            borrower[MAXNAME_LENGTH];
        time_t duetime;

        FILE* file = fopen(metadatapath, "r");
        if (file != NULL) {
            fscanf(file, "%d;%[^;];%[^;];%d;%[^;];%lld", &id, title, author, &pages, borrower, &duetime);
            fclose(file);
            createbook(id, title, author, pages, borrower, duetime);
        }
    }

    closedir(folder);
}

void save_users() {
    FILE* file = fopen(USER_DATABASE_PATH, "w");

    size_t i = 0, j = 0;
    for (i = 0; i < ULENGTH; i++) {
        user_t tmp = USER_LIST[i];

        char borrowed[101 + 11] = "";
        for (j = 0; j < MAX_BORROW; j++) {
            char intstr[11] = "";

            sprintf(intstr, ";%d", tmp.book_ids[j]);
            strcat(borrowed, intstr);
        }

        strcat(borrowed, "\n");
        fprintf(file, "%s;%s;%d;%d%s", tmp.name, tmp.password, tmp.isadmin, tmp.book_count, borrowed);
    }

    fclose(file);
}

void load_users() {
    FILE* file = fopen(USER_DATABASE_PATH, "r");
    while (!feof(file)) {
        char name[MAXNAME_LENGTH], password[MAXNAME_LENGTH];
        int book_count, book_ids[MAX_BORROW];
        int isadmin;

        char tmp[101 + 11];

        int scanres = fscanf(file, "%[^;];%[^;];%d;%d;%[^\n]", name, password, &isadmin, &book_count, tmp);
        if (scanres == EOF || fgetc(file) == EOF)
            break;

        int idx = 0;
        char* token = strtok(tmp, ";");

        while (token != NULL) {
            int bookid = atoi(token);
            book_ids[idx++] = bookid;

            token = strtok(NULL, ";");
        }

        printf("%s is created\n", name);
        createuser(name, password, isadmin, book_ids, book_count);
    }

    fclose(file);
}

bool readbook(book_t* current, int page) {
    clearscreen();

    if (!isbook_borrowed(*current)) {
        printf("Anda sedang tidak meminjam buku ini!\n");
        return false;
    }
    if (strcmp(current->borrower, CURRENT_USER->name) != 0) {
        printf("Anda bukanlah orang yang meminjam buku ini!\n");
        return false;
    }
    if (page > current->pages) {
        printf("Buku ini hanya terdapat %d halaman!\n", current->pages);
        return false;
    }

    char path[FILENAME_MAX];
    sprintf(path, BOOK_DATABASE_PATH "/%d/%d.txt", current->id, page);

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Tidak dapat menemukan halaman %d pada buku!\n", page);
        return false;
    }

    char line[257] = "";
    while (fgets(line, 257, file) != NULL)
        printf("%s", line);
    fclose(file);

    printf(
        "\n"
        "\n"
        "Halaman: %d/%d\n"
        "Tekan 'enter' untuk selesai membaca...\n", page, current->pages);

    getchar();
    fflush(stdin);
    return true;
}

/**
 * @brief Shows the user's borrowed books
 * 
 * @return true if the function successfully executed, false is otherwise
 */
bool show_borrowed_books() {
    clearscreen();

    int total = CURRENT_USER->book_count;
    if (total == 0) {
        printf("Anda sedang tidak meminjam buku apapun!\n");
        return false;
    }

    MAIN_BOOK_SORT = ID_SORT;
    quicksort_book(BOOK_LIST, BLENGTH, book_comparator(ASCENDING));

    set_utf8_encoding(stdout);
    wchar_t LINE[108];
    wcscpy(LINE, L"──────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

    wprintf(LINE);
    wprintf(L"│ %-3s │ %-40s │ %-40s │ %-10s │\n", "ID", "Judul", "Penulis", "Due Date");
    wprintf(LINE);

    int i = 0;
    for (i = 0; i < total; i++) {
        set_default_encoding(stdout);
        int idx = findbook(CURRENT_USER->book_ids[i]);

        book_t* tmp = &BOOK_LIST[idx];
        struct tm* ltm = localtime(&tmp->duetime);

        char duedate[] = "dd-MM-yyyy";
        sprintf(duedate, "%02d-%02d-%d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
        strftime(duedate, strlen(duedate) + 1, "%d-%m-%Y", ltm);

        set_utf8_encoding(stdout);
        wprintf(L"│ %-3d │ %-40s │ %-40s │ %-10s │\n", tmp->id, tmp->title, tmp->author, duedate);
    }

    wprintf(LINE);
    set_default_encoding(stdout);

    return true;
}