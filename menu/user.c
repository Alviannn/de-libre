#include "user.h"

void __print_receipt(book_t* book, long fine) {
    clearscreen();

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║               Pengembalian  Buku               ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    wprintf(
        L"ID    : %d\n"
        L"Judul : %ls\n"
        L"\n"
        L"Denda : Rp %'ld,00\n",
        book->id, book->title, fine);
}

void __do_borrow(book_t* book) {
    clearscreen();

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║           Konfirmasi Peminjaman Buku           ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    view_book(book);

    if (isbook_borrowed(book)) {
        wprintf(L"Buku ini sedang dipinjam oleh orang lain!\n");
        return;
    }
    if (CURRENT_USER->book_count >= MAX_BORROW) {
        wprintf(L"Anda hanya dapat meminjam 10 buku!\n");
        return;
    }

    bool ans = await_confirmation(L"Apakah anda yakin ingin meminjam buku ini?\n");
    if (ans) {
        wcscpy(book->borrower, CURRENT_USER->name);
        book->duetime = time(NULL) + WEEK_IN_SECONDS;

        CURRENT_USER->book_ids[CURRENT_USER->book_count] = book->id;
        CURRENT_USER->book_count++;

        wprintf(L"Anda berhasil meminjam buku ini!\n");

        save_books();
        save_users();
    }
}

/**
 * @brief Handles the book borrowing for user 
 */
void __borrow_books(book_t* target) {
    __do_borrow(target);
    await_enter();
}

void __return_borrowed_books() {
    book_t* target = NULL;
    // the index on user instance
    int uidx = -1;
    int bookcount = CURRENT_USER->book_count;

    do {
        clearscreen();

        wprintf(
            L"╔════════════════════════════════════════════════╗\n"
            L"║               Mengembalikan Buku               ║\n"
            L"╚════════════════════════════════════════════════╝\n"
            L"\n");

        bool res = show_borrowed_books();
        if (!res) {
            await_enter();
            return;
        }

        int id = scan_number(L"\nMasukkan ID buku [0 untuk kembali]: ");
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
            wprintf(L"Tidak dapat menemukan ID buku yang sedang dipinjam!\n");
            await_enter();
            continue;
        }

        int idx = findbook(id);
        target = &BOOK_DB[idx];

        break;
    } while (true);

    bool ans = await_confirmation(L"Apakah anda yakin ingin mengembalikan buku ini?\n");
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
        wcscpy(target->borrower, L"-");
        CURRENT_USER->book_count--;

        save_books();
        save_users();

        wprintf(L"Buku telah dikembalikan!\n");
    }

    await_enter();
}

/**
 * @brief Changes the user's password
 */
void __change_password() {
    clearscreen();

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                 Ganti Password                 ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    user_t* current = CURRENT_USER;
    wchar_t oldpass[MAXNAME_LENGTH], newpass[MAXNAME_LENGTH];

    do {
        wprintf(L"Masukkan password lama anda [0 untuk kembali]: ");
        getpass(oldpass, MAXNAME_LENGTH);

        if (wcscmp(oldpass, L"0") == 0)
            return;
        if (wcscmp(oldpass, current->password) != 0) {
            wprintf(L"Password anda salah!\n");
            continue;
        }

        break;
    } while (true);

    do {
        wprintf(L"Masukkan password baru anda [0 untuk kembali]: ");
        getpass(newpass, MAXNAME_LENGTH);

        if (wcscmp(newpass, L"0") == 0)
            return;
        if (wcscmp(newpass, oldpass) == 0) {
            wprintf(L"Password baru anda tidak boleh sama dengan password lama anda!\n");
            continue;
        }

        if (wcslen(newpass) < 5) {
            wprintf(L"Password minimal terdiri dari 5 kata!\n");
            continue;
        }

        wchar_t* charchek_ptr = wcspbrk(newpass, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        wchar_t* numcheck_ptr = wcspbrk(newpass, L"0123456789");

        if (charchek_ptr == NULL || numcheck_ptr == NULL) {
            wprintf(L"Password harus terdiri dari huruf dan angka!\n");
            continue;
        }

        break;
    } while (true);

    do {
        wchar_t confirm[MAXNAME_LENGTH];

        wprintf(L"Konfirmasi password [0 untuk kembali]: ");
        getpass(confirm, MAXNAME_LENGTH);

        if (wcscmp(confirm, L"0") == 0)
            return;
        if (wcscmp(newpass, confirm) != 0) {
            wprintf(L"Password konfirmasi salah!\n");
            continue;
        }

        break;
    } while (true);

    wcscpy(current->password, newpass);
    wprintf(L"Password berhasil diubah!\n");

    save_users();

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
    wprintf(
        L"NOTE: Anda dapat mengisi '0' untuk keluar dari menu ini!\n"
        L"\n");

    while (true) {
        if (current == NULL) {
            int id = scan_number(L"Masukkan ID buku yang ingin dibaca: ");

            if (id == 0)
                return;
            if (id < 1)
                wprintf(L"ID buku tidak valid!\n");

            int idx = findbook(id);
            if (idx == -1) {
                wprintf(L"Tidak dapat menemukan buku dengan ID ini!\n");
                continue;
            }

            current = &BOOK_DB[idx];

            if (!book_hascontent(current)) {
                current = NULL;
                wprintf(L"Buku ini tidak dapat dibaca secara langsung karena tidak memiliki isi!\n");
                continue;
            }
        }

        clearscreen();
        wprintf(
            L"NOTE: Anda dapat mengisi '0' untuk keluar dari menu ini!\n"
            L"\n");

        wprintf(L"Masukkan halaman yang ingin dibaca [1-%d]: ", current->pages);
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

    bool isvalid;
    do {
        int choice = scan_number(L"Pilihan [0-4] >> ");
        isvalid = true;

        switch (choice) {
            case 1:
                showbooks(
                    L"╔════════════════════════════════════════════════╗\n"
                    L"║                 Meminjam  Buku                 ║\n"
                    L"╚════════════════════════════════════════════════╝\n"
                    L"\n",
                    __borrow_books);
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
                return;
            default:
                wprintf(L"Pilihan tidak dapat ditemukan!\n");
                isvalid = false;
                break;
        }
    } while (!isvalid);
}