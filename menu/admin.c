#include "admin.h"

void __add_book() {
    clearscreen();

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                  Tambah  Buku                  ║\n"
        L"╚════════════════════════════════════════════════╝\n");

    int total = 0;
    do {
        total = scan_number(L"Masukkan jumlah buku yang akan diinputkan [0 untuk kembali]: ");

        if (total == 0)
            return;
        if (total < 1) {
            wprintf(L"Jumlah buku tidak boleh negatif!\n");
            continue;
        }

        break;
    } while (true);

    int i = 0;
    for (i = 0; i < total; i++) {
        clearscreen();

        wprintf(
            L"╔════════════════════════════════════════════════╗\n"
            L"║                     CATATAN                    ║\n"
            L"╠════════════════════════════════════════════════╣\n"
            L"║                                                ║\n"
            L"║ • Semua string atau teks pada input memiliki   ║\n"
            L"║   panjang maksimal 40 character dan panjang    ║\n"
            L"║   minimal 3 character.                         ║\n"
            L"║                                                ║\n"
            L"║ • Input '0' untuk membatalkan seluruh proses   ║\n"
            L"║   penambahan buku dan kembali ke user menu.    ║\n"
            L"║                                                ║\n"
            L"╚════════════════════════════════════════════════╝\n"
            L"\n");

        wprintf(
            L"Buku ke-%d\n"
            L"\n",
            i + 1);

        wchar_t title[MAXNAME_LENGTH], author[MAXNAME_LENGTH];
        int pages, relyear;

        do {
            scan_string(L"Masukkan judul buku: ", title, MAXNAME_LENGTH);

            if (wcscmp(title, L"0") == 0)
                return;
            if (wcslen(title) < 3) {
                wprintf(L"Minimal panjang judul buku adalah 3 character!\n");
                continue;
            }

            break;
        } while (true);

        do {
            scan_string(L"Masukkan penulis buku: ", author, MAXNAME_LENGTH);

            if (wcscmp(author, L"0") == 0)
                return;
            if (wcslen(author) < 3) {
                wprintf(L"Minimal panjang penulis buku adalah 3 character!\n");
                continue;
            }

            break;
        } while (true);

        do {
            relyear = scan_number(L"Masukkan tahun rilis buku: ");

            if (relyear == 0)
                return;
            if (relyear < 1) {
                wprintf(L"Tahun rilis buku harus positif!\n");
                continue;
            }

            break;
        } while (true);

        do {
            pages = scan_number(L"Masukkan jumlah halaman buku: ");

            if (pages == 0)
                return;
            if (pages < 0) {
                wprintf(L"Jumlah halaman buku tidak bisa negatif!\n");
                continue;
            }

            break;
        } while (true);

        if (BLENGTH >= 1) {
            int idx = findbook_title(title);
            if (idx != -1) {
                book_t* found = &BOOK_DB[idx];

                if (wcscmp(found->author, author) == 0) {
                    wprintf(L"Buku ini sudah ada di database buku!\n");
                    continue;
                }
            }
        }

        createbook(++LAST_BOOK_ID, title, author, relyear, pages, L"-", 0L);
        wprintf(L"Buku berhasil ditambahkan!\n");

        await_enter();
    }

    clearscreen();

    wprintf(
        L"╔═══════════════════════════════════════════════════════╗\n"
        L"║                        CATATAN                        ║\n"
        L"╠═══════════════════════════════════════════════════════╣\n"
        L"║                                                       ║\n"
        L"║  Buku yang telah ditambahkan tidak akan langsung      ║\n"
        L"║  mendapatkan isi buku.                                ║\n"
        L"║                                                       ║\n"
        L"║  Untuk menambahkan isi buku atau halaman buku:        ║\n"
        L"║  1. Letakkan file text (.txt) ke dalam folder dengan  ║\n"
        L"║     alamat file `" ANSI_YELLOW "DATABASE/BOOK_DB/(id buku pilihan)" ANSI_RESET
        "`  ║\n"
        L"║     dimana program berada.                            ║\n"
        L"║                                                       ║\n"
        L"║  2. Berikan nama file text (.txt) tersebut sebuah     ║\n"
        L"║     angka untuk merepresentasikan halaman untuk buku  ║\n"
        L"║     tersebut.                                         ║\n"
        L"║     Contoh: 1.txt, 2.txt, dan 3.txt                   ║\n"
        L"║                                                       ║\n"
        L"║  3. Setelah semua langkah diatas diselesaikan         ║\n"
        L"║    para pengguna akan mulai dapat membaca buku        ║\n"
        L"║    yang telah dipinjam.                               ║\n"
        L"║                                                       ║\n"
        L"║  Contoh penambahan isi buku:                          ║\n"
        L"║  • ID buku yang ingin diisi adalah 12 dan file        ║\n"
        L"║    isi/halaman buku adalah 1.txt, 2.txt, 3.txt.       ║\n"
        L"║                                                       ║\n"
        L"║   • Penempatan file:                                  ║\n"
        L"║      " ANSI_YELLOW "DATABASE" ANSI_RESET
        "                                         ║\n"
        L"║       " ANSI_YELLOW "└─BOOK_DB" ANSI_RESET
        "                                       ║\n"
        L"║         " ANSI_YELLOW "└─12" ANSI_RESET
        "                                          ║\n"
        L"║           " ANSI_YELLOW "├─1.txt" ANSI_RESET
        "                                     ║\n"
        L"║           " ANSI_YELLOW "├─2.txt" ANSI_RESET
        "                                     ║\n"
        L"║           " ANSI_YELLOW "├─3.txt" ANSI_RESET
        "                                     ║\n"
        L"║           " ANSI_YELLOW "└─metadata.txt" ANSI_RESET
        "                              ║\n"
        L"║                                                       ║\n"
        L"╚═══════════════════════════════════════════════════════╝\n"
        L"\n");

    save_books();
    await_enter();
}

void __remove_book(book_t* target) {
    if (await_confirmation(L"Apakah anda yakin ingin menghapus buku ini?\n")) {
        removebook(target->id);
        await_enter();
    }
}

void __view_books(book_t* target) {
    view_book(target);
    await_enter();
}

void __view_users() {
    clearscreen();

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                  Daftar  User                  ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    wchar_t line[60];
    wcscpy(line, L"──────────────────────────────────────────────────────────\n");

    wprintf(line);
    wprintf(L"│ %-3ls │ %-40ls │ %-5ls │\n", L"No.", L"Username", L"Admin");
    wprintf(line);

    size_t i = 0;
    for (i = 0; i < ULENGTH; i++) {
        user_t* tmp = &USER_DB[i];
        wprintf(L"│ %-3llu │ %-40ls │ %-5ls │\n", i + 1, tmp->name, (tmp->isadmin ? L"Yes" : L"No"));
    }
    wprintf(line);

    await_enter();
}

void showadmin_menu() {
    clearscreen();

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

    bool isvalid;
    do {
        int choice = scan_number(L"Masukkan pilihan anda >> ");
        isvalid = true;

        switch (choice) {
            case 1:
                __add_book();
                break;
            case 2:
                showbooks(
                    L"╔════════════════════════════════════════════════╗\n"
                    L"║                   Hapus Buku                   ║\n"
                    L"╚════════════════════════════════════════════════╝\n"
                    L"\n",
                    __remove_book);
                break;
            case 3:
                showbooks(
                    L"╔════════════════════════════════════════════════╗\n"
                    L"║                  Daftar  Buku                  ║\n"
                    L"╚════════════════════════════════════════════════╝\n"
                    L"\n",
                    __view_books);
                break;
            case 4:
                __view_users();
                break;
            case 0:
                CURRENT_USER = NULL;
                return;
            default:
                isvalid = false;
                wprintf(L"Pilihan tidak dapat ditemukan!\n");
                break;
        }
    } while (!isvalid);
}