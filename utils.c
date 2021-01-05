#include "common.h"

#define ESC_KEY 27
#define ENTER_KEY 13
#define BACKSPACE_KEY 8
#define EOT_KEY 3
#define NULL_KEY 0

void await_enter() {
    wprintf(L"Tekan enter untuk lanjut...");
    getwchar();
    fflush(stdin);
}

void sleep(long millis) {
    clock_t expected = clock() + millis;

    while (clock() < expected) {
        // do nothing
    }
}

void clearscreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

double scan_number(wchar_t* message) {
    double input = 0;
    int scanresult = 0;

    do {
        if (message != NULL)
            wprintf(message);

        scanresult = wscanf(L"%lf", &input);
        fflush(stdin);

        if (scanresult != 0)
            break;

        wprintf(L"Input ini hanya dapat menerima angka!\n");
    } while (1);

    return input;
}

void scan_string(wchar_t* message, wchar_t* dest, size_t size) {
    if (message != NULL)
        wprintf(message);

    fgetws(dest, size, stdin);
    if (dest[wcslen(dest) - 1] == '\n')
        dest[wcslen(dest) - 1] = 0;

    fflush(stdin);
}

void getpass(wchar_t* dest, size_t size) {
    wchar_t key = 0;

    size_t count = 0;
    memset(dest, 0, size);

    // selagi key yg terbaca bukan 'enter' key, lanjutnya input scanningnya
    while ((key = _getwch()) != ENTER_KEY) {
        // melakukan apa yg seharusnya 'backspace' key lakukan
        // ya... ngedelete character per character
        if (key == BACKSPACE_KEY && count != 0) {
            count--;
            dest[count] = 0;
            wprintf(L"\b \b");
            continue;
        }

        // didalam windows, jika kita tekan 'ESC' atau escape key
        // apa yang sudah tertulis akan terhapuskan
        if (key == ESC_KEY) {
            memset(dest, 0, size);

            size_t i = 0;
            for (i = 0; i < count; i++)
                wprintf(L"\b \b");

            count = 0;
            continue;
        }
        // di windows, jika user menekan CTRL + C, kita akan paksa keluar dari program
        // dan 'CTRL + C' itu pada ASCII adalah EOT (end of text)
        if (key == EOT_KEY)
            exit(EXIT_FAILURE);
        // jika user menekan arrow key, skip
        if (key == NULL_KEY) {
            // arrow key ada 2 inputnya
            _getwch();
            continue;
        }
        // jika key yg ditekan invalid (tidak diketahui arti ASCII-nya) atau melebihi kapasitas string
        // skip
        if (key < 32 || key > 126 || count >= size - 1)
            continue;

        dest[count] = key;
        count++;
        wprintf(L"*");
    }

    wprintf(L"\n");
}

void* safe_alloc(void* mem, size_t num_elems, size_t elem_size) {
    void* tmp = NULL;

    // free memory yg ditargetkan jika kosong
    if (num_elems == 0 && mem != NULL) {
        free(mem);
        mem = NULL;
        return mem;
    }

    // jika target memory adalah NULL maka kita inisialisasi alokasikan memory
    if (mem == NULL)
        tmp = malloc(num_elems * elem_size);
    // dan jika tidak, kita alokasi ulang memory
    else
        tmp = realloc(mem, num_elems * elem_size);

    // jika proses alokasi memory gagal, maka kita akan hentikan program
    // karena kegagalan ini hanya bisa terjadi karena tidak memiliki memory
    if (tmp == NULL) {
        wprintf(
            L"\n"
            L"Error: Failed to reallocate memory for %p!\n"
            L"       This is most likely caused because there's no memory available!\n",
            mem);

        free(mem);
        exit(EXIT_FAILURE);
    } else {
        mem = tmp;
    }

    return mem;
}

void set_utf8_encoding(FILE* file) {
    _setmode(_fileno(file), UTF8_ENCODING);
}

void set_default_encoding(FILE* file) {
    _setmode(_fileno(file), DEFAULT_ENCODING);
}

// -                                                     - //
// ---------------------- QuickSort ---------------------- //
// -                                                     - //

ll __partition_user(user_t* base, ll low, ll high, cmpfunc_user_t __cmpfunc) {
    user_t* pivot = &base[high];
    ll idx = (low - 1);

    ll i = 0;
    for (i = low; i <= high - 1; i++) {
        user_t* a = &base[i];

        if (__cmpfunc(pivot, a) > 0) {
            idx++;

            user_t* b = &base[idx];
            user_t tmp = *a;

            *a = *b;
            *b = tmp;
        }
    }

    idx++;

    user_t* a = &base[high];
    user_t* b = &base[idx];
    user_t tmp = *a;

    *a = *b;
    *b = tmp;

    return idx;
}

void __handlesort_user(user_t* base, ll low, ll high, cmpfunc_user_t __cmpfunc) {
    if (low < high) {
        ll pi = __partition_user(base, low, high, __cmpfunc);

        __handlesort_user(base, low, pi - 1, __cmpfunc);
        __handlesort_user(base, pi + 1, high, __cmpfunc);
    }
}

void quicksort_user(user_t* base, size_t length, cmpfunc_user_t __cmpfunc) {
    __handlesort_user(base, 0, length - 1, __cmpfunc);
}

// -------------------------------------------------------------------------------------------------- //

ll __partition_book(book_t* base, ll low, ll high, cmpfunc_book_t __cmpfunc) {
    book_t* pivot = &base[high];
    ll idx = (low - 1);

    ll i = 0;
    for (i = low; i <= high - 1; i++) {
        book_t* a = &base[i];
        if (__cmpfunc(pivot, a) > 0) {
            idx++;

            book_t* b = &base[idx];
            book_t tmp = *a;

            *a = *b;
            *b = tmp;
        }
    }

    idx++;

    book_t* a = &base[high];
    book_t* b = &base[idx];
    book_t tmp = *a;

    *a = *b;
    *b = tmp;

    return idx;
}

void __handlesort_book(book_t* base, ll low, ll high, cmpfunc_book_t __cmpfunc) {
    if (low < high) {
        ll pi = __partition_book(base, low, high, __cmpfunc);

        __handlesort_book(base, low, pi - 1, __cmpfunc);
        __handlesort_book(base, pi + 1, high, __cmpfunc);
    }
}

void quicksort_book(book_t* base, size_t length, cmpfunc_book_t __cmpfunc) {
    __handlesort_book(base, 0, length - 1, __cmpfunc);
}

bool isdir(char* path) {
    if (path == NULL)
        return false;

    struct stat filestat;
    if (stat(path, &filestat) != 0)
        return false;

    return S_ISDIR(filestat.st_mode);
}

void delete_recursively(char* path) {
    if (path == NULL)
        return;

    if (!isdir(path)) {
        remove(path);
        return;
    }

    DIR* dir = opendir(path);
    struct dirent* readd = NULL;

    char anotherpath[FILENAME_MAX * 2];

    int count = 0;
    while ((readd = readdir(dir)) != NULL) {
        count++;
        if (count <= 2)
            continue;

        strcpy(anotherpath, path);
        strcat(anotherpath, "/");
        strcat(anotherpath, readd->d_name);

        if (isdir(anotherpath))
            delete_recursively(anotherpath);
        else
            remove(anotherpath);
    }

    rmdir(path);
    closedir(dir);
}