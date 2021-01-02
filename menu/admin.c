#include "admin.h"

void __add_book() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                  Tambah  Buku                  ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);

    int total = scan_number("Masukkan jumlah buku yang akan diiputkan [0 untuk kembali]: ");
    if (total == 0)
        return;

    for (int i = 0; i < total; i++) {
        printf(
            "Buku ke-%d\n"
            "\n",
            i + 1);

        char title[MAXNAME_LENGTH], author[MAXNAME_LENGTH];
        int pages;

        scan_string("Masukkan judul buku: ", title, MAXNAME_LENGTH);
        scan_string("Masukkan penulis buku: ", author, MAXNAME_LENGTH);
        do {
            pages = scan_number("Masukkan jumlah halaman buku: ");
            if (pages < 0) {
                printf("Jumlah halaman buku tidak bisa negatif!\n");
                continue;
            }

            break;
        } while (true);

        int idx = findbook_title(title);
        if (idx != -1) {
            book_t* found = &BOOK_LIST[idx];

            if (strcmp(found->author, author) == 0) {
                printf("Buku ini sudah ada di database buku!\n");
                continue;
            }
        }

        createbook(++LAST_BOOK_ID, title, author, pages, "-", 0L);
        printf("Buku berhasil ditambahkan!\n");
    }

    await_enter();
}

void __remove_book() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                   Hapus Buku                   ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);
    int targetid;

    do {
        targetid = scan_number("Masukkan ID buku: ");
        if (targetid < 0) {
            printf("ID buku tidak dapat negatif!\n");
            continue;
        }

        break;
    } while (1);

    if (removebook(targetid))
        printf("Buku berhasil dihapus!\n");
    else
        printf("Buku tidak ditemukan!\n");

    await_enter();
}

void __add_user() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                  Tambah  User                  ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);

    char username[MAXNAME_LENGTH], password[MAXNAME_LENGTH];
    bool isadmin = false;

    do {
        scan_string("Username [0 untuk kembali]: ", username, MAXNAME_LENGTH);
        if (strcmp(username, "0") == 0)
            return;
        if (strlen(username) < 3) {
            printf("Username minimal terdiri dari 3 kata!\n");
            continue;
        }

        if (strchr(username, ' ') != NULL) {
            printf("Spasi pada username tidak diperbolehkan\n");
            continue;
        }
        if (finduser(username) != -1) {
            printf("Username telah terpakai!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf("Password [0 untuk kembali]: ");
        getpass(password, MAXNAME_LENGTH);

        if (strcmp(password, "0") == 0)
            return;
        if (strlen(password) < 5) {
            printf("Password minimal terdiri dari 5 kata!\n");
            continue;
        }

        char* charchek_ptr = strpbrk(password, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        char* numcheck_ptr = strpbrk(password, "0123456789");

        if (charchek_ptr == NULL || numcheck_ptr == NULL) {
            printf("Password harus terdiri dari huruf dan angka!\n");
            continue;
        }

        break;
    } while (true);

    do {
        char confirm[MAXNAME_LENGTH];

        printf("Konfirmasi password [0 untuk kembali]: ");
        getpass(confirm, MAXNAME_LENGTH);

        if (strcmp(confirm, "0") == 0)
            return;
        if (strcmp(password, confirm) != 0) {
            printf("Password konfirmasi salah!\n");
            continue;
        }

        break;
    } while (true);

    printf(
        "Apakah anda ingin menjadikan akun ini sebuah admin?\n"
        "(Y) Ya\n"
        "(N) Tidak\n");

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

    isadmin = answer == 'Y';

    printf("Akun dengan username \"%s\" telah dibuat!\n", username);
    createuser(username, password, isadmin, NULL, 0);

    await_enter();
}

void __view_books() {
    int page = 1;

    book_sort name = ID_SORT;
    sort_type type = ASCENDING;

    while (true) {
        clearscreen();

        set_utf8_encoding(stdout);
        wprintf(
            L"╔════════════════════════════════════════════════╗\n"
            L"║                  Daftar  Buku                  ║\n"
            L"╚════════════════════════════════════════════════╝\n"
            L"\n");

        set_default_encoding(stdout);
        bookpaginate_t pack = book_paginate(BOOK_LIST, BLENGTH, name, type, page);
        if (pack.len == 0) {
            printf("Data buku tidak dapat ditemukan!\n");
            await_enter();
            return;
        }

        char LINE[116] = "";
        makeline(LINE, 114);
        strcat(LINE, "\n");

        printf(LINE);
        printf("%c %-3s %c %-40s %c %-40s %c %-7s %c %-8s %c\n",
               179, "ID", 179, "Judul", 179, "Penulis", 179, "Halaman", 179, "Tersedia", 179);
        printf(LINE);

        for (int i = 0; i < (int)pack.len; i++) {
            book_t* tmp = &pack.list[i];

            printf("%c %-3u %c %-40s %c %-40s %c %-7u %c %-8s %c\n",
                   179, tmp->id, 179, tmp->title, 179, tmp->author, 179, tmp->pages, 179, isbook_borrowed(*tmp) ? "No" : "Yes", 179);
        }

        printf(LINE);
        printf("Page: %d/%d\n", page, pack.maxpage);

        printf(
            "\n"
            "1. Pilih buku\n"
            "2. Urutkan buku\n"
            "3. Halaman selanjutnya\n"
            "4. Halaman sebelumnya\n"
            "0. Kembali\n");

        bool isvalid = true;
        do {
            int choice = scan_number("Pilihan [0-4] >> ");

            int targetid = 0;

            int targetidx = -1;
            book_t* targetbook;

            int tempsort = 0;
            int temptype = 0;

            switch (choice) {
                case 1:
                    targetid = scan_number("Masukkan ID buku [0 untuk kembali]: ");
                    if (targetid == 0)
                        break;

                    targetidx = findbook(targetid);
                    if (targetidx == -1) {
                        printf("Tidak dapat menemukan ID buku!\n");
                        await_enter();
                        return;
                    }

                    targetbook = &BOOK_LIST[targetidx];

                    view_book(targetbook);
                    await_enter();
                    return;
                case 2:
                    tempsort = select_booksort();
                    if (tempsort == -1)
                        break;

                    temptype = select_sorttype();
                    if (temptype == -1)
                        break;

                    name = (book_sort)tempsort;
                    type = (sort_type)temptype;
                    break;
                case 3:
                    page++;
                    break;
                case 4:
                    page--;
                    break;
                case 0:
                    return;
                default:
                    printf("Pilihan tidak dapat ditemukan!\n");
                    isvalid = false;
                    break;
            }
        } while (!isvalid);
    }
}

void __view_users() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                  Daftar  User                  ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);

    char LINE[116] = "";
    makeline(LINE, 114);
    strcat(LINE, "\n");

    printf(LINE);
    printf("%c %-3s %c %-40s %c %-5s %c\n",
           179, "No.", 179, "Username", 179, "Admin", 179);
    printf(LINE);

    for (int i = 0; i < (int)ULENGTH; i++) {
        user_t* tmp = &USER_LIST[i];
        printf("%c %-3d %c %-40s %c %-5s %c\n", 
        179, i + 1, 179, tmp->name, 179, (tmp->isadmin ? "Yes" : "No"), 179);
    }

    printf(LINE);
    await_enter();
}

void showadmin_menu() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                   ADMIN MENU                   ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ [1] Tambah buku                                ║\n"
        L"║ [2] Hapus buku                                 ║\n"
        L"║ [3] Tambah user                                ║\n"
        L"║ [4] Lihat daftar buku                          ║\n"
        L"║ [5] Lihat daftar user                          ║\n"
        L"║ [0] Logout                                     ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);
    bool isvalid = true;

    do {
        int choice = scan_number("Masukkan pilihan anda >> ");
        switch (choice) {
            case 1:
                __add_book();
                break;
            case 2:
                __remove_book();
                break;
            case 3:
                __add_user();
                break;
            case 4:
                __view_books();
                break;
            case 5:
                __view_users();
                break;
            case 0:
                break;
            default:
                isvalid = false;
                printf("Pilihan tidak dapat ditemukan!\n");
                break;
        }
    } while (!isvalid);
}