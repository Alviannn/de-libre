#include "user.h"

void __print_receipt(book_t* book, long fine) {
    clearscreen();

    char LINE[53] = "";
    makeline(LINE, 51);
    strcat(LINE, "\n");

    printf(LINE);
    printf(
        "                 Pengembalian Buku\n"
        "\n"
        "ID   : %d\n"
        "Judul: %s\n"
        "\n"
        "Denda: Rp %'ld,00\n",
        book->id, book->title, fine);
    printf(LINE);
}

void __do_borrow(book_t* book) {
    clearscreen();
    view_book(book);

    if (isbook_borrowed(*book)) {
        printf("Buku ini sedang dipinjam oleh orang lain!\n");
        return;
    }
    if (CURRENT_USER->book_count >= MAX_BORROW) {
        printf("Anda hanya dapat meminjam 10 buku!\n");
        return;
    }

    printf(
        "Apakah anda yakin ingin meminjam buku ini?\n"
        "\n"
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

    if (answer == 'Y') {
        strcpy(book->borrower, CURRENT_USER->name);
        book->duetime = time(NULL) + WEEK_IN_SECONDS;

        CURRENT_USER->book_ids[CURRENT_USER->book_count] = book->id;
        CURRENT_USER->book_count++;

        printf("Anda berhasil meminjam buku ini!\n");
    }
}

/**
 * @brief Handles the book borrowing for user 
 */
void __borrow_books(book_sort name, sort_type type) {
    int page = 1;

    while (true) {
        clearscreen();

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
        printf("Page: %d/%u\n", page, pack.maxpage);

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

/**
 * @brief Shows the user's borrowed books
 * 
 * @return true if the function successfully executed, false is otherwise
 */
bool __show_borrowed_books() {
    clearscreen();

    int total = CURRENT_USER->book_count;
    if (total == 0) {
        printf("Anda sedang tidak meminjam buku apapun!\n");
        return false;
    }

    char LINE[108] = "";
    makeline(LINE, 106);
    strcat(LINE, "\n");

    printf(LINE);
    printf("%c %-3s %c %-40s %c %-40s %c %-10s %c\n",
           179, "ID", 179, "Judul", 179, "Penulis", 179, "Due Date", 179);
    printf(LINE);

    for (int i = 0; i < total; i++) {
        int idx = findbook(CURRENT_USER->book_ids[i]);

        book_t* tmp = &BOOK_LIST[idx];
        struct tm* ltm = localtime(&tmp->duetime);

        char duedate[] = "dd-MM-yyyy";
        sprintf(duedate, "%02d-%02d-%d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
        strftime(duedate, strlen(duedate) + 1, "%d-%m-%Y", ltm);

        printf("%c %-3u %c %-40s %c %-40s %c %-10s %c\n",
               179, tmp->id, 179, tmp->title, 179, tmp->author, 179, duedate, 179);
    }

    printf(LINE);
    return true;
}

void __return_borrowed_books() {
    book_t* target = NULL;
    // the index on user instance
    int uidx = -1;
    int bookcount = CURRENT_USER->book_count;

    do {
        clearscreen();

        bool res = __show_borrowed_books();
        if (!res) {
            await_enter();
            return;
        }

        int id = scan_number("\nMasukkan ID buku [0 untuk kembali]: ");
        if (id == 0)
            return;

        // does linear search
        // because 10 is the max amount, so no worries
        for (int i = 0; i < bookcount; i++) {
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
        // incase when the selected book is not in the db but the user has it
        // we must remove the book id from the user's instance here
        if (idx == -1) {
            int start = uidx;
            int* bookids = CURRENT_USER->book_ids;

            if (uidx + 1 == bookcount) {
                bookids[start] = 0;
            } else {
                memcpy((bookids + start), (bookids + start + 1), sizeof(int) * (bookcount - start - 1));
                bookids[bookcount - 1] = 0;
            }

            CURRENT_USER->book_count--;
            printf("Buku telah dikembalikan!\n");
            await_enter();
            return;
        }

        target = &BOOK_LIST[idx];
        break;
    } while (true);

    printf(
        "Apakah anda yakin ingin mengembalikan buku ini?\n"
        "\n"
        "(Y) Ya\n"
        "(N) Tidak\n");

    char choice = 0;
    do {
        if (choice != 0)
            printf("Pilihan tidak dapat ditemukan!\n");

        printf(">> ");
        choice = toupper(getchar());
        fflush(stdin);
    } while (choice != 'Y' && choice != 'N');

    if (choice == 'Y') {
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

        printf("Buku telah dikembalikan!\n");
    }

    await_enter();
}

/**
 * @brief Changes the user's password
 */
void __change_password() {
    clearscreen();

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

void showuser_menu() {
    clearscreen();
    
    set_utf8_encoding(stdout);
    wprintf(
        L"╔═════════════════════════════════╗\n"
        L"║   Selamat datang %s di Libras   ║\n"
        L"╠═════════════════════════════════╣\n"
        L"║  [1] Pinjam buku                ║\n"
        L"║  [2] Mengembalikan buku         ║\n"
        L"║  [3] Lihat buku pinjaman        ║\n"
        L"║  [4] Ganti password             ║\n"
        L"║  [0] Logout                     ║\n"
        L"╚═════════════════════════════════╝\n",
        CURRENT_USER->name);

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
                __show_borrowed_books();
                await_enter();
                break;
            case 4:
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