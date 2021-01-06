#include "common.h"

int LAST_BOOK_ID = 0;

user_t* CURRENT_USER = NULL;
book_sort MAIN_BOOK_SORT = ID_SORT;

user_t* USER_DB = NULL;
size_t ULENGTH = 0;

book_t* BOOK_DB = NULL;
size_t BLENGTH = 0;

// -                                                      - //
// ---------------------- Comparator ---------------------- //
// -                                                      - //

int comparebook_asc(const book_t* a, const book_t* b) {
    switch (MAIN_BOOK_SORT) {
        case ID_SORT:
            return a->id - b->id;
        case TITLE_SORT:
            return wcscmp(a->title, b->title);
        case AUTHOR_SORT:
            return wcscmp(a->author, b->author);
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
    return wcscmp(a->name, b->name);
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

int createuser(wchar_t name[], wchar_t password[], bool isadmin, int* book_ids, int book_count) {
    user_t user;

    wcscpy(user.name, name);
    wcscpy(user.password, password);
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
    USER_DB = safe_alloc(USER_DB, ULENGTH, sizeof(user_t));
    USER_DB[ULENGTH - 1] = user;

    return ULENGTH - 1;
}

int createbook(int id, wchar_t title[], wchar_t author[], int pages, wchar_t* borrower, time_t duetime) {
    book_t book;

    wcscpy(book.title, title);
    wcscpy(book.author, author);

    if (borrower != NULL && wcslen(borrower) != 0)
        wcscpy(book.borrower, borrower);

    book.id = id;
    book.pages = pages;
    book.duetime = duetime;

    // menambahkan buku ke dalam database buku
    BLENGTH++;
    BOOK_DB = safe_alloc(BOOK_DB, BLENGTH, sizeof(book_t));
    BOOK_DB[BLENGTH - 1] = book;

    return BLENGTH - 1;
}

bool removebook(int id) {
    if (id <= 0) {
        wprintf(L"Buku ini tidak dapat ditemukan\n");
        return false;
    }

    int idx = findbook(id);
    if (idx == -1) {
        wprintf(L"Buku ini tidak dapat ditemukan\n");
        return false;
    }

    book_t* found = &BOOK_DB[idx];
    if (isbook_borrowed(*found)) {
        wprintf(L"Buku yang sedang dipinjam tidak dapat dihapus!\n");
        return false;
    }

    char bookpath[FILENAME_MAX];
    sprintf(bookpath, BOOK_DATABASE_PATH "/%d", found->id);
    delete_recursively(bookpath);

    BLENGTH--;
    if ((size_t)idx != BLENGTH)
        memcpy((BOOK_DB + idx), (BOOK_DB + idx + 1), sizeof(book_t) * (BLENGTH - idx));

    BOOK_DB = safe_alloc(BOOK_DB, BLENGTH, sizeof(book_t));

    wprintf(L"Buku berhasil dihapuskan dari database!\n");
    save_books();
    return true;
}

bool isbook_borrowed(book_t book) {
    return wcslen(book.borrower) != 0 && wcscmp(book.borrower, L"-") != 0 && book.duetime != 0;
}

int finduser(wchar_t name[]) {
    int left = 0;
    int right = ULENGTH - 1;
    int mid = 0;

    quicksort_user(USER_DB, ULENGTH, compareuser);

    do {
        mid = (left + right) / 2;
        user_t* temp = &USER_DB[mid];

        int compare = wcscmp(name, temp->name);
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

    quicksort_book(BOOK_DB, BLENGTH, book_comparator(ASCENDING));
    MAIN_BOOK_SORT = before;

    int left = 0;
    int right = BLENGTH - 1;
    int mid = 0;

    do {
        mid = (left + right) / 2;
        book_t* temp = &BOOK_DB[mid];

        if (id == temp->id)
            return mid;

        if (id < temp->id)
            right = mid - 1;
        if (id > temp->id)
            left = mid + 1;
    } while (left <= right);

    return -1;
}

int findbook_title(wchar_t title[]) {
    book_sort before = MAIN_BOOK_SORT;
    MAIN_BOOK_SORT = TITLE_SORT;

    quicksort_book(BOOK_DB, BLENGTH, book_comparator(ASCENDING));
    MAIN_BOOK_SORT = before;

    int left = 0;
    int right = BLENGTH - 1;
    int mid = 0;

    do {
        mid = (left + right) / 2;
        book_t* temp = &BOOK_DB[mid];

        int compare = wcscmp(title, temp->title);
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
    quicksort_book(BOOK_DB, BLENGTH, book_comparator(type));

    // membuat sebuah bookpack sebagai informasi/data dari pembuatan halaman ini
    bookpaginate_t pack;
    pack.len = len;
    pack.list = (arr + (ELEMENTS_PER_PAGE * (page - 1)));
    pack.maxpage = maxpage;

    return pack;
}

void view_book(book_t* book) {
    clearscreen();
    wprintf(
        L"╔══════════════════════════════════════════════════════╗\n"
        L"║                     Detail  Buku                     ║\n"
        L"╠══════════════════════════════════════════════════════╣\n"
        L"║                                                      ║\n"
        L"║ · Judul   : %-40ls ║\n"
        L"║ · Penulis : %-40ls ║\n"
        L"║ · Halaman : %-40d ║\n"
        L"║ · Tersedia: %-40ls ║\n"
        L"║                                                      ║\n"
        L"╚══════════════════════════════════════════════════════╝\n",
        book->title,
        book->author,
        book->pages,
        isbook_borrowed(*book) ? L"No" : L"Yes");
}

int select_booksort() {
    clearscreen();
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

    bool isvalid = true;
    do {
        int choice = scan_number(L"Pilihan [0-5] >> ");

        if (choice >= 0 && choice <= 5) {
            return choice - 1;
        } else {
            isvalid = false;
            wprintf(L"Pilihan tidak dapat ditemukan!\n");
        }
    } while (!isvalid);

    return -1;
}

int select_sorttype() {
    clearscreen();
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

    bool isvalid = true;

    do {
        int choice = scan_number(L"Pilihan [0-2] >> ");

        if (choice >= 0 && choice <= 2) {
            return choice - 1;
        } else {
            isvalid = false;
            wprintf(L"Pilihan tidak dapat ditemukan!\n");
        }
    } while (!isvalid);

    return -1;
}

bool await_confirmation(wchar_t* message) {
    if (message == NULL || wcslen(message) == 0) {
        wprintf(L"Gagal dalam membuat konfirmasi!\n");
        return false;
    }

    wprintf(
        L"%ls"
        L"\n"
        L"(Y) Ya\n"
        L"(N) Tidak\n", message);

    wchar_t answer;
    do {
        wprintf(L">> ");
        answer = towupper(getwchar());
        fflush(stdin);

        if (answer != L'Y' && answer != L'N') {
            wprintf(L"Pilihan tidak valid!\n");
            continue;
        }

        break;
    } while (true);

    return answer == L'Y';
}

void save_books() {
    mkdir(BOOK_DATABASE_PATH);

    FILE* lastidfile = fopen(BOOK_DATABASE_PATH "/lastid.txt", "w");
    fprintf(lastidfile, "%d\n", LAST_BOOK_ID);
    fclose(lastidfile);

    size_t i = 0;
    for (i = 0; i < BLENGTH; i++) {
        book_t* tmp = &BOOK_DB[i];

        int buf = FILENAME_MAX + strlen(BOOK_DATABASE_PATH);

        char path[buf];
        sprintf(path, BOOK_DATABASE_PATH "/%d", tmp->id);
        mkdir(path);

        buf += 13;

        char metadata[buf];

        strcpy(metadata, path);
        strcat(metadata, "/");
        strcat(metadata, "metadata.txt");

        FILE* curfile = fopen(metadata, "w");
        set_utf8_encoding(curfile);

        fwprintf(curfile, L"%d;%ls;%ls;%d;%ls;%lld\n",
                tmp->id, tmp->title, tmp->author, tmp->pages, tmp->borrower, tmp->duetime);
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
        wchar_t title[MAXNAME_LENGTH],
            author[MAXNAME_LENGTH],
            borrower[MAXNAME_LENGTH];
        time_t duetime;

        FILE* file = fopen(metadatapath, "r");
        set_utf8_encoding(file);

        if (file != NULL) {
            fwscanf(file, L"%d;%l[^;];%l[^;];%d;%l[^;];%lld", &id, title, author, &pages, borrower, &duetime);
            fclose(file);
            createbook(id, title, author, pages, borrower, duetime);
        }
    }

    closedir(folder);
}

void save_users() {
    FILE* file = fopen(USER_DATABASE_PATH, "w");
    set_utf8_encoding(file);

    size_t i = 0, j = 0;
    for (i = 0; i < ULENGTH; i++) {
        user_t* tmp = &USER_DB[i];

        wchar_t borrowed[101 + 11];
        wcscpy(borrowed, L"");

        for (j = 0; j < MAX_BORROW; j++) {
            wchar_t intstr[11];

            swprintf(intstr, 12, L";%d", tmp->book_ids[j]);
            wcscat(borrowed, intstr);
        }

        wcscat(borrowed, L"\n");
        fwprintf(file, L"%ls;%ls;%d;%d%ls", tmp->name, tmp->password, tmp->isadmin, tmp->book_count, borrowed);
    }

    fclose(file);
}

void load_users() {
    FILE* file = fopen(USER_DATABASE_PATH, "r");
    set_utf8_encoding(file);

    while (!feof(file)) {
        wchar_t name[MAXNAME_LENGTH], password[MAXNAME_LENGTH];
        int book_count, book_ids[MAX_BORROW];
        int isadmin;

        wchar_t tmp[101 + 11];

        int scanres = fwscanf(file, L"%l[^;];%l[^;];%d;%d;%l[^\n]", name, password, &isadmin, &book_count, tmp);
        if (scanres == EOF || fgetc(file) == EOF)
            break;

        int idx = 0;
        wchar_t* token = wcstok(tmp, L";");

        while (token != NULL) {
            int bookid = _wtoi(token);
            book_ids[idx++] = bookid;

            token = wcstok(NULL, L";");
        }

        createuser(name, password, isadmin, book_ids, book_count);
    }

    fclose(file);
}

bool readbook(book_t* current, int page) {
    clearscreen();

    if (!isbook_borrowed(*current)) {
        wprintf(L"Anda sedang tidak meminjam buku ini!\n");
        return false;
    }
    if (wcscmp(current->borrower, CURRENT_USER->name) != 0) {
        wprintf(L"Anda bukanlah orang yang meminjam buku ini!\n");
        return false;
    }
    if (page > current->pages) {
        wprintf(L"Buku ini hanya terdapat %d halaman!\n", current->pages);
        return false;
    }

    char path[FILENAME_MAX];
    sprintf(path, BOOK_DATABASE_PATH "/%d/%d.txt", current->id, page);

    FILE* file = fopen(path, "r");
    set_utf8_encoding(file);

    if (file == NULL) {
        wprintf(L"Tidak dapat menemukan halaman %d pada buku!\n", page);
        return false;
    }

    wchar_t line[257];
    while (fgetws(line, 257, file) != NULL)
        wprintf(line);
    fclose(file);

    wprintf(
        L"\n"
        L"\n"
        L"Halaman: %d/%d\n"
        L"Tekan 'enter' untuk selesai membaca...\n", page, current->pages);

    getwchar();
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
        wprintf(L"Anda sedang tidak meminjam buku apapun!\n");
        return false;
    }

    MAIN_BOOK_SORT = ID_SORT;
    quicksort_book(BOOK_DB, BLENGTH, book_comparator(ASCENDING));

    wchar_t LINE[108];
    wcscpy(LINE, L"──────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

    wprintf(LINE);
    wprintf(L"│ %-3ls │ %-40ls │ %-40ls │ %-10ls │\n", L"ID", L"Judul", L"Penulis", L"Due Date");
    wprintf(LINE);

    int i = 0;
    for (i = 0; i < total; i++) {
        int idx = findbook(CURRENT_USER->book_ids[i]);

        book_t* tmp = &BOOK_DB[idx];
        struct tm* ltm = localtime(&tmp->duetime);

        wchar_t duedate[11];
        swprintf(duedate, 11, L"%02d-%02d-%d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
        wcsftime(duedate, 11, L"%d-%m-%Y", ltm);

        wprintf(L"│ %-3d │ %-40ls │ %-40ls │ %-10ls │\n", tmp->id, tmp->title, tmp->author, duedate);
    }

    wprintf(LINE);
    return true;
}