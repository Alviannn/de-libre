#include "login.h"

void __login_user() {
    clearscreen();

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                   LOGIN NOTE                   ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ · Input '0' untuk membatalkan login dan        ║\n"
        L"║   kembali ke menu awal.                        ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    int useridx = -1;
    wchar_t username[MAXNAME_LENGTH], password[MAXNAME_LENGTH];

    do {
        scan_string(L"Username: ", username, MAXNAME_LENGTH);
        if (wcscmp(username, L"0") == 0)
            return;

        useridx = finduser(username);
        if (useridx == -1) {
            wprintf(L"Akun dengan username ini tidak dapat ditemukan!\n");
            continue;
        }

        break;
    } while (true);

    user_t* found = &USER_LIST[useridx];
    do {
        wprintf(L"Password: ");
        getpass(password, MAXNAME_LENGTH);

        if (wcscmp(password, L"0") == 0)
            return;

        if (wcscmp(password, found->password) != 0) {
            wprintf(L"Password yang anda masukkan salah!\n");
            continue;
        }

        break;
    } while (true);

    CURRENT_USER = found;
    wprintf(L"\nAnda telah berhasil login!\n");
    await_enter();
}

void __register_user() {
    clearscreen();

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║               REGISTRATION  NOTE               ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ · Username tidak diperbolehkan ada spasi,      ║\n"
        L"║   panjang minimalnya adalah 3 character        ║\n"
        L"║                                                ║\n"
        L"║ · Password harus terdapat angka dan huruf,     ║\n"
        L"║   panjang minimalnya adalah 5 character        ║\n"
        L"║                                                ║\n"
        L"║ · Input '0' untuk membatalkan registrasi       ║\n"
        L"║   dan kembali ke menu awal.                    ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    wchar_t username[MAXNAME_LENGTH], password[MAXNAME_LENGTH];
    do {
        scan_string(L"Username: ", username, MAXNAME_LENGTH);
        if (wcscmp(username, L"0") == 0)
            return;
        if (wcslen(username) < 3) {
            wprintf(L"Username minimal terdiri dari 3 kata!\n");
            continue;
        }

        if (wcschr(username, L' ') != NULL) {
            wprintf(L"Spasi pada username tidak diperbolehkan\n");
            continue;
        }
        if (finduser(username) != -1) {
            wprintf(L"Username telah terpakai!\n");
            continue;
        }

        break;
    } while (true);

    do {
        wprintf(L"Password: ");
        getpass(password, MAXNAME_LENGTH);

        if (wcscmp(password, L"0") == 0)
            return;
        if (wcslen(password) < 5) {
            wprintf(L"Password minimal terdiri dari 5 kata!\n");
            continue;
        }

        wchar_t* charchek_ptr = wcspbrk(password, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        wchar_t* numcheck_ptr = wcspbrk(password, L"0123456789");

        if (charchek_ptr == NULL || numcheck_ptr == NULL) {
            wprintf(L"Password harus terdiri dari huruf dan angka!\n");
            continue;
        }

        break;
    } while (true);

    do {
        wchar_t confirm[MAXNAME_LENGTH];

        wprintf(L"Konfirmasi password: ");
        getpass(confirm, MAXNAME_LENGTH);

        if (wcscmp(confirm, L"0") == 0)
            return;
        if (wcscmp(password, confirm) != 0) {
            wprintf(L"Password konfirmasi salah!\n");
            continue;
        }

        break;
    } while (true);

    wprintf(
        L"\nRegistrasi akun telah selesai!\n"
        L"Sekarang Anda dapat login ke dalam program dengan akun ini!\n");

    createuser(username, password, false, NULL, 0);
    save_users();
    
    await_enter();
}

void __exit_program() {
    clearscreen();

    wchar_t msg[970];
    wcscpy(msg,
          L"╔════════════════════════════════════════════════╗\n"
          L"║            ___                                 ║\n"
          L"║             | |_  _  _ |    \\/ _               ║\n"
          L"║             | | |(_|| ||<   / (_)|_|           ║\n"
          L"║                                                ║\n"
          L"║                                                ║\n"
          L"║  Terima kasih sudah menggunakan aplikasi ini!  ║\n"
          L"║                                                ║\n"
          L"║                  Anggota (4):                  ║\n"
          L"║   (1) Alexander Yoga Setiawan - 2440076002     ║\n"
          L"║   (2) Alvian Daru Quthni      - 2440113686     ║\n"
          L"║   (3) Fadlan Muhammad Bahtiar - 2440113566     ║\n"
          L"║   (4) Rahmat Syifana Jaelani  - 2440113585     ║\n"
          L"║                                                ║\n"
          L"║                    GROUP  3                    ║\n"
          L"║          TOPIC: Aplikasi Perpustakaan          ║\n"
          L"║                  KELAS:  LC01                  ║\n"
          L"║                                                ║\n"
          L"╚════════════════════════════════════════════════╝\n");

    int count = 0;
    int i = 0, j = 0;

    for (i = 0; i < 19; i++) {
        for (j = 0; j < 51; j++)
            wprintf(L"%lc", msg[count++]);

        sleep(50);
    }
    
    wprintf(L"\n");

    await_enter();
    exit(EXIT_SUCCESS);
}

void showlogin_menu() {
    clearscreen();

    wprintf(
        L"╔═══╗        ╔╗     ╔╗\n"
        L"╚╗╔╗║        ║║     ║║\n"
        L" ║║║║╔══╗    ║║   ╔╗║╚═╗╔═╗╔══╗\n"
        L" ║║║║║╔╗║    ║║ ╔╗╠╣║╔╗║║╔╝║╔╗║\n"
        L"╔╝╚╝║║║═╣    ║╚═╝║║║║╚╝║║║ ║║═╣\n"
        L"╚═══╝╚══╝    ╚═══╝╚╝╚══╝╚╝ ╚══╝\n"
        L"\n");

    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                   LOGIN MENU                   ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ [1] Login                                      ║\n"
        L"║ [2] Register                                   ║\n"
        L"║ [0] Keluar                                     ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");

    bool isvalid = true;

    do {
        int choice = scan_number(L"Pilihan [0-2] >> ");
        switch (choice) {
            case 1:
                __login_user();
                break;
            case 2:
                __register_user();
                break;
            case 0:
                __exit_program();
                break;
            default:
                wprintf(L"Pilihan tidak dapat ditemukan!\n");
                isvalid = false;
                break;
        }
    } while (!isvalid);
}
