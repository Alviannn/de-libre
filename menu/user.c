#include "user.h"

void __print_receipt(book_t* book, long fine) {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║               Pengembalian  Buku               ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    wprintf(
        L"ID    : %d\n"
        L"Judul : %s\n"
        L"\n"
        L"Denda : Rp %'ld,00\n",
        book->id, book->title, fine);

    set_default_encoding(stdout);
}

void __do_borrow(book_t* book) {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║           Konfirmasi Peminjaman Buku           ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");
    set_default_encoding(stdout);

    view_book(book);

    if (isbook_borrowed(*book)) {
        printf("Buku ini sedang dipinjam oleh orang lain!\n");
        return;
    }
    if (CURRENT_USER->book_count >= MAX_BORROW) {
        printf("Anda hanya dapat meminjam 10 buku!\n");
        return;
    }

    bool ans = await_confirmation("Apakah anda yakin ingin meminjam buku ini?\n");
    if (ans) {
        strcpy(book->borrower, CURRENT_USER->name);
        book->duetime = time(NULL) + WEEK_IN_SECONDS;

        CURRENT_USER->book_ids[CURRENT_USER->book_count] = book->id;
        CURRENT_USER->book_count++;

        printf("Anda berhasil meminjam buku ini!\n");

        save_books();
        save_users();
    }
}

/**
 * @brief Handles the book borrowing for user 
 */
void __borrow_books(book_sort name, sort_type type) {
    int page = 1;

    while (true) {
        clearscreen();

        set_utf8_encoding(stdout);
        wprintf(
            L"╔════════════════════════════════════════════════╗\n"
            L"║                 Meminjam  Buku                 ║\n"
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
        for (i = 0; i < (int)pack.len; i++) {
            book_t* tmp = &pack.list[i];

            wprintf(L"│ %-3d │ %-40s │ %-40s │ %-7u │ %-8s │\n",
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
                    __do_borrow(targetbook);

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

void __return_borrowed_books() {
    book_t* target = NULL;
    // the index on user instance
    int uidx = -1;
    int bookcount = CURRENT_USER->book_count;

    do {
        clearscreen();

        set_utf8_encoding(stdout);
        wprintf(
            L"╔════════════════════════════════════════════════╗\n"
            L"║               Mengembalikan Buku               ║\n"
            L"╚════════════════════════════════════════════════╝\n"
            L"\n");
        set_default_encoding(stdout);

        bool res = show_borrowed_books();
        if (!res) {
            await_enter();
            return;
        }

        int id = scan_number("\nMasukkan ID buku [0 untuk kembali]: ");
        if (id == 0)
            return;

        // does linear search
        // because 10 is the max amount, so no worries
        int i = 0;
        for (i = 0; i < bookcount; i++) {
            int tmpid = CURRENT_USER->book_ids[i];

            if (id == tmpid) {
                uidx = i;
                break;
            }
        }

        if (uidx == -1) {
            printf("Tidak dapat menemukan ID buku yang sedang dipinjam!\n");
            await_enter();
            continue;
        }

        int idx = findbook(id);
        target = &BOOK_LIST[idx];
        
        break;
    } while (true);

    bool ans = await_confirmation("Apakah anda yakin ingin mengembalikan buku ini?\n");
    if (ans) {
        int start = uidx;
        int* bookids = CURRENT_USER->book_ids;

        if (uidx + 1 == bookcount) {
            bookids[start] = 0;
        } else {
            memcpy((bookids + start), (bookids + start + 1), sizeof(int) * (bookcount - start - 1));
            bookids[bookcount - 1] = 0;
        }

        time_t diff = time(NULL) - target->duetime;

        // if the user returns the book late
        if (diff >= DAY_IN_SECONDS) {
            long multiplier = diff / DAY_IN_SECONDS;
            long totalfine = LATE_FINE * multiplier;

            __print_receipt(target, totalfine);
        }

        target->duetime = 0;
        strcpy(target->borrower, "-");
        CURRENT_USER->book_count--;

        save_books();
        save_users();

        printf("Buku telah dikembalikan!\n");
    }

    await_enter();
}

/**
 * @brief Changes the user's password
 */
void __change_password() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                 Ganti Password                 ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");
    set_default_encoding(stdout);

    user_t* current = CURRENT_USER;
    char oldpass[MAXNAME_LENGTH], newpass[MAXNAME_LENGTH];

    do {
        printf("Masukkan password lama anda [0 untuk kembali]: ");
        getpass(oldpass, MAXNAME_LENGTH);

        if (strcmp(oldpass, "0") == 0)
            return;
        if (strcmp(oldpass, current->password) != 0) {
            printf("Password anda salah!\n");
            continue;
        }

        break;
    } while (true);

    do {
        printf("Masukkan password baru anda [0 untuk kembali]: ");
        getpass(newpass, MAXNAME_LENGTH);

        if (strcmp(newpass, "0") == 0)
            return;
        if (strcmp(newpass, oldpass) == 0) {
            printf("Password baru anda tidak boleh sama dengan password lama anda!\n");
            continue;
        }

        if (strlen(newpass) < 5) {
            printf("Password minimal terdiri dari 5 kata!\n");
            continue;
        }

        char* charchek_ptr = strpbrk(newpass, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        char* numcheck_ptr = strpbrk(newpass, "0123456789");

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
        if (strcmp(newpass, confirm) != 0) {
            printf("Password konfirmasi salah!\n");
            continue;
        }

        break;
    } while (true);

    strcpy(current->password, newpass);
    printf("Password berhasil diubah!\n");

    await_enter();
}

void __read_book() {
    clearscreen();

    bool canshow = show_borrowed_books();
    if (!canshow) {
        await_enter();
        return;
    }

    book_t* current = NULL;
    printf(
        "NOTE: Anda dapat mengisi '0' untuk keluar dari menu ini!\n"
        "\n");

    while (true) {
        if (current == NULL) {
            int id = scan_number("Masukkan ID buku yang ingin dibaca: ");

            if (id == 0)
                return;
            if (id < 1)
                printf("ID buku tidak valid!\n");

            int idx = findbook(id);
            if (idx == -1)
                printf("Tidak dapat menemukan buku dengan ID ini!\n");

            current = &BOOK_LIST[idx];
        }

        clearscreen();
        printf(
            "NOTE: Anda dapat mengisi '0' untuk keluar dari menu ini!\n"
            "\n");


        printf("Masukkan halaman yang ingin dibaca [1-%d]: ", current->pages);
        int page = scan_number(NULL);
        if (page == 0)
            return;

        bool canread = readbook(current, page);
        if (!canread)
            await_enter();
    }
}

void showuser_menu() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                   USER  MENU                   ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ [1] Pinjam buku                                ║\n"
        L"║ [2] Mengembalikan buku                         ║\n"
        L"║ [3] Lihat buku pinjaman                        ║\n"
        L"║ [4] Baca buku pinjaman                         ║\n"
        L"║ [5] Ganti password                             ║\n"
        L"║ [0] Logout                                     ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    set_default_encoding(stdout);
    bool isvalid = true;

    do {
        int choice = scan_number("Pilihan [0-4] >> ");
        switch (choice) {
            case 1:
                __borrow_books(ID_SORT, ASCENDING);
                break;
            case 2:
                __return_borrowed_books();
                break;
            case 3:
                show_borrowed_books();
                await_enter();
                break;
            case 4:
                __read_book();
                break;
            case 5:
                __change_password();
                break;
            case 0:
                CURRENT_USER = NULL;
                break;
            default:
                printf("Pilihan tidak dapat ditemukan!\n");
                isvalid = false;
                break;
        }
    } while (!isvalid);
}