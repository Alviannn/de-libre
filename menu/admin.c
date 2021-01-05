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

    int i = 0;
    for (i = 0; i < total; i++) {
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
        printf(
            "Buku berhasil ditambahkan!\n"
            "\n"
            "CATATAN:\n"
            "Buku yang telah ditambahkan tidak langsung mendapatkan isi-isi buku tersebut!\n"
            "\n"
            "Untuk menambahkan isi buku atau halaman buku:\n"
            "1. Letakkan file text (.txt) ke dalam folder dengan ID buku yang diinginkan\n"
            "2. Nama file text tersebut harus berupa angka untuk merepresentasikan halaman buku.\n"
            "3. Selesai!\n");
    }

    save_books();
    await_enter();
}

void __remove_book() {
    int page = 1;

    book_sort name = ID_SORT;
    sort_type type = ASCENDING;

    while (true) {
        clearscreen();

        set_utf8_encoding(stdout);
        wprintf(
            L"╔════════════════════════════════════════════════╗\n"
            L"║                   Hapus Buku                   ║\n"
            L"╚════════════════════════════════════════════════╝\n"
            L"\n");

        bookpaginate_t pack = book_paginate(BOOK_LIST, BLENGTH, name, type, page);
        if (pack.len == 0) {
            set_default_encoding(stdout);
            printf("Data buku tidak dapat ditemukan!\n");
            await_enter();
            return;
        }

        wchar_t LINE[116];
        wcscpy(LINE, L"──────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

        wprintf(LINE);
        wprintf(L"│ %-3s │ %-40s │ %-40s │ %-7s │ %-8s │\n", "ID", "Judul", "Penulis", "Halaman", "Tersedia");
        wprintf(LINE);

        int i = 0;
        for (i = 0; i < pack.len; i++) {
            book_t* tmp = &pack.list[i];

            wprintf(L"│ %-3d │ %-40s │ %-40s │ %-7d │ %-8s │\n",
                    tmp->id, tmp->title, tmp->author, tmp->pages, isbook_borrowed(*tmp) ? "No" : "Yes");
        }

        wprintf(LINE);

        set_default_encoding(stdout);
        printf(
            "Page: %d/%d\n"
            "\n"
            "1. Pilih buku\n"
            "2. Urutkan buku\n"
            "3. Halaman selanjutnya\n"
            "4. Halaman sebelumnya\n"
            "0. Kembali\n",
            page, pack.maxpage);

        bool isvalid = true;
        do {
            int choice = scan_number("Pilihan [0-4] >> ");

            int targetid = 0;
            int tempsort = 0;
            int temptype = 0;

            switch (choice) {
                case 1:
                    targetid = scan_number("Masukkan ID buku [0 untuk kembali]: ");
                    if (targetid == 0)
                        break;

                    if (await_confirmation("Apakah anda yakin ingin menghapus buku ini?\n")) {
                        removebook(targetid);
                        await_enter();
                    }
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

        bookpaginate_t pack = book_paginate(BOOK_LIST, BLENGTH, name, type, page);
        if (pack.len == 0) {
            set_default_encoding(stdout);
            printf("Data buku tidak dapat ditemukan!\n");
            await_enter();
            return;
        }

        wchar_t LINE[116];
        wcscpy(LINE, L"──────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

        wprintf(LINE);
        wprintf(L"│ %-3s │ %-40s │ %-40s │ %-7s │ %-8s │\n", "ID", "Judul", "Penulis", "Halaman", "Tersedia");
        wprintf(LINE);

        int i = 0;
        for (i = 0; i < pack.len; i++) {
            book_t* tmp = &pack.list[i];

            wprintf(L"│ %-3d │ %-40s │ %-40s │ %-7d │ %-8s │\n",
                    tmp->id, tmp->title, tmp->author, tmp->pages, isbook_borrowed(*tmp) ? "No" : "Yes");
        }

        wprintf(LINE);

        set_default_encoding(stdout);
        printf(
            "Page: %d/%d\n"
            "\n"
            "1. Pilih buku\n"
            "2. Urutkan buku\n"
            "3. Halaman selanjutnya\n"
            "4. Halaman sebelumnya\n"
            "0. Kembali\n",
            page, pack.maxpage);

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
                    CURRENT_USER = NULL;
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

    wchar_t line[60];
    wcscpy(line, L"──────────────────────────────────────────────────────────\n");

    wprintf(line);
    wprintf(L"│ %-3s │ %-40s │ %-5s │\n", "No.", "Username", "Admin");
    wprintf(line);

    size_t i = 0;
    for (i = 0; i < ULENGTH; i++) {
        user_t* tmp = &USER_LIST[i];
        wprintf(L"│ %-3llu │ %-40s │ %-5s │\n", i + 1, tmp->name, (tmp->isadmin ? "Yes" : "No"));
    }
    wprintf(line);

    set_default_encoding(stdout);
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
        L"║ [3] Lihat daftar buku                          ║\n"
        L"║ [4] Lihat daftar user                          ║\n"
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
                __view_books();
                break;
            case 4:
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