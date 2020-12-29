#include "user.h"

/**
 * @brief Views the book detail
 */
void __view_book(book_t* book) {
    clearscreen();

    printf(
        "---------------------------------------------------\n"
        "Detail Buku:\n"
        "\n"
        "Judul   : %s\n"
        "Penulis : %s\n"
        "Halaman : %d\n"
        "Tersedia: %s\n"
        "---------------------------------------------------\n",
        book->title, book->author, book->pages, isbook_borrowed(*book) ? "No" : "Yes");
}

void __do_borrow(book_t* book) {
    clearscreen();
    __view_book(book);

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
        "(Y) Yes\n"
        "(N) No\n");

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

        CURRENT_USER->book_ids[CURRENT_USER->book_count] = book->id;
        CURRENT_USER->book_count++;

        printf("Anda berhasil meminjam buku ini!\n");
    }
}

int __select_sortname() {
    clearscreen();
    printf(
        "Urutan buku:\n"
        "1. Berdasarkan ID buku\n"
        "2. Berdasarkan judul buku\n"
        "3. Berdasarkan penulis buku\n"
        "4. Berdasarkan jumlah halaman buku\n"
        "5. Berdasarkan status peminjaman\n"
        "0. Kembali\n");

    bool isvalid = true;
    do {
        int choice = scan_number(">> ");

        if (choice >= 0 || choice <= 5) {
            return choice - 1;
        } else {
            isvalid = false;
            printf("Pilihan tidak dapat ditemukan!\n");
        }
    } while (!isvalid);

    return -1;
}

int __select_sorttype() {
    clearscreen();
    printf(
        "Tipe urutan:\n"
        "1. Urutan ke atas\n"
        "2. Urutan ke bawah\n");

    bool isvalid = true;
    do {
        int choice = scan_number(">> ");

        if (choice >= 0 || choice <= 2) {
            return choice - 1;
        } else {
            isvalid = false;
            printf("Pilihan tidak dapat ditemukan!\n");
        }
    } while (!isvalid);

    return -1;
}

/**
 * @brief Handles the book borrowing for user 
 */
void __borrow_books(book_sort name, sort_type type) {
    int page = 1;

    while (true) {
        clearscreen();

        bookpack_t pack = book_paginate(BOOK_LIST, BLENGTH, name, type, page);
        if (pack.len == 0) {
            printf("Data buku tidak dapat ditemukan!\n");
            await_enter();
            return;
        }

        char LINE[] = "------------------------------------------------------------------------------------------------------------------\n";

        printf(LINE);
        printf("| %-3s | %-40s | %-40s | %-7s | %-8s |\n", "ID", "Judul", "Penulis", "Halaman", "Tersedia");
        printf(LINE);

        for (int i = 0; i < (int)pack.len; i++) {
            book_t* tmp = &pack.list[i];

            printf("| %-3u | %-40s | %-40s | %-7u | %-8s |\n",
                   tmp->id, tmp->title, tmp->author, tmp->pages, isbook_borrowed(*tmp) ? "No" : "Yes");
        }

        printf(LINE);

        printf(
            "\n"
            "1. Pilih buku\n"
            "2. Urutkan buku\n"
            "3. Halaman selanjutnya\n"
            "4. Halaman sebelumnya\n"
            "5. Kembali\n");

        bool isvalid = true;
        do {
            int choice = scan_number("Pilihan >> ");

            int targetid = 0;

            int targetidx = -1;
            book_t* targetbook;

            int tempsort = 0;
            int temptype = 0;

            switch (choice) {
                case 1:
                    targetid = scan_number("Masukkan ID buku: ");
                    targetidx = findbook(targetid);

                    if (targetidx == -1) {
                        printf("Tidak dapat menemukan ID buku!\n");
                        await_enter();
                        return;
                    }

                    targetbook = &BOOK_LIST[targetidx];
                    if (strlen(targetbook->borrower) == 0) {
                        printf("Buku ini sedang dipinjam oleh seseorang!");
                        await_enter();
                        return;
                    }

                    __do_borrow(targetbook);
                    await_enter();
                    return;
                case 2:
                    tempsort = __select_sortname();
                    if (tempsort == -1)
                        break;

                    temptype = __select_sorttype();
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
                case 5:
                    return;
                default:
                    printf("Pilihan tidak dapat ditemukan!\n");
                    await_enter();

                    isvalid = false;
                    break;
            }
        } while (!isvalid);
    }
}

/**
 * @brief Shows the user's borrowed books
 */
void __show_borrowed_books() {
    clearscreen();

    int total = CURRENT_USER->book_count;
    if (total == 0) {
        printf("Anda sedang tidak meminjam buku apapun!\n");
        await_enter();
        return;
    }

    char LINE[] = "----------------------------------------------------------------------------------------------------------\n";

    printf(LINE);
    printf("| %-3s | %-40s | %-40s | %-10s |\n", "ID", "Judul", "Penulis", "Due Date");
    printf(LINE);

    for (int i = 0; i < total; i++) {
        int idx = findbook(CURRENT_USER->book_ids[i]);

        book_t* tmp = &BOOK_LIST[idx];
        struct tm* ltm = localtime(&tmp->btime);

        char duedate[] = "dd-MM-yyyy";
        sprintf(duedate, "%02d-%02d-%d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
        strftime(duedate, strlen(duedate) + 1, "%d-%m-%Y", ltm);

        printf("| %-3u | %-40s | %-40s | %-10s |\n", tmp->id, tmp->title, tmp->author, duedate);
    }

    printf(LINE);

    await_enter();
}

void showuser_menu() {
    clearscreen();

    printf(
        "Selamat datang %s di Libras\n"
        "\n"
        "1. Pinjam buku\n"
        "2. Mengembalikan buku\n"
        "3. Lihat buku pinjaman\n"
        "4. Ganti password\n"
        "0. Logout\n",
        CURRENT_USER->name);

    bool isvalid = true;
    do {
        int choice = scan_number(">> ");
        switch (choice) {
            case 1:
                __borrow_books(ID_SORT, ASCENDING);
                break;
            case 2:
                __show_borrowed_books();
                break;
            case 3:
                __show_borrowed_books();
                break;
            case 4:
                break;
            case 0:
                CURRENT_USER = NULL;
                break;
            default:
                printf("Pilihan tidak dapat ditemukan!\n");
                await_enter();
                isvalid = false;
                break;
        }
    } while (!isvalid);
}