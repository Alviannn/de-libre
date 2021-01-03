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

int comparebook_asc(const void* a, const void* b) {
    book_t p = *(book_t*)a;
    book_t q = *(book_t*)b;

    switch (MAIN_BOOK_SORT) {
        case ID_SORT:
            return p.id - q.id;
        case TITLE_SORT:
            return strcmp(p.title, q.title);
        case AUTHOR_SORT:
            return strcmp(p.author, q.author);
        case PAGES_SORT:
            return p.pages - q.pages;
        case AVAILABILITY_SORT:
            if (!isbook_borrowed(p) && !isbook_borrowed(q))
                return p.id - q.id;
            else
                return !isbook_borrowed(q);
    }

    return p.id - q.id;
}

int comparebook_desc(const void* a, const void* b) {
    return comparebook_asc(b, a);
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
    if (id < 0) {
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

    BLENGTH--;
    if ((size_t)idx != BLENGTH)
        memcpy((BOOK_LIST + idx), (BOOK_LIST + idx + 1), sizeof(book_t) * (BLENGTH - idx));

    BOOK_LIST = safe_alloc(BOOK_LIST, BLENGTH, sizeof(book_t));
    return true;
}

bool isbook_borrowed(book_t book) {
    return strlen(book.borrower) == 0 || strcmp(book.borrower, "-") || book.duetime == 0;
}

int finduser(char name[]) {
    int left = 0;
    int right = ULENGTH - 1;
    int mid = 0;

    do {
        mid = (left + right) / 2;
        user_t* temp = &USER_LIST[mid];

        int compare = strcmp(name, temp->name);
        if (compare == 0)
            return mid;

        if (compare > 0)
            right = mid - 1;
        if (compare < 0)
            left = mid + 1;
    } while (left <= right);

    return -1;
}

int findbook(int id) {
    book_sort before = MAIN_BOOK_SORT;
    MAIN_BOOK_SORT = ID_SORT;

    quicksort(BOOK_LIST, BLENGTH, sizeof(book_t), book_comparator(ASCENDING));
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

    quicksort(BOOK_LIST, BLENGTH, sizeof(book_t), book_comparator(ASCENDING));
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

        if (compare > 0)
            right = mid - 1;
        if (compare < 0)
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
    quicksort(BOOK_LIST, BLENGTH, sizeof(book_t), book_comparator(type));

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