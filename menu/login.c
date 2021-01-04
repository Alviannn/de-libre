#include "login.h"

void __login_user() {
    clearscreen();

    set_utf8_encoding(stdout);
    wprintf(
        L"╔════════════════════════════════════════════════╗\n"
        L"║                   LOGIN NOTE                   ║\n"
        L"╠════════════════════════════════════════════════╣\n"
        L"║                                                ║\n"
        L"║ · Input '0' untuk membatalkan registrasi       ║\n"
        L"║   dan kembali ke menu awal.                    ║\n"
        L"║                                                ║\n"
        L"╚════════════════════════════════════════════════╝\n"
        L"\n");
    set_default_encoding(stdout);

    int useridx = -1;
    char username[MAXNAME_LENGTH], password[MAXNAME_LENGTH];

    do {
        scan_string("Username: ", username, MAXNAME_LENGTH);
        if (strcmp(username, "0") == 0)
            return;

        useridx = finduser(username);
        if (useridx == -1) {
            printf("Akun dengan username ini tidak dapat ditemukan!\n");
            continue;
        }

        break;
    } while (true);

    user_t* found = &USER_LIST[useridx];
    do {
        printf("Password: ");
        getpass(password, MAXNAME_LENGTH);

        if (strcmp(password, "0") == 0)
            return;

        if (strcmp(password, found->password) != 0) {
            printf("Password yang anda masukkan salah!\n");
            continue;
        }

        break;
    } while (true);

    CURRENT_USER = found;
    printf("\nAnda telah berhasil login!\n");
    await_enter();
}

void __register_user() {
    clearscreen();

    set_utf8_encoding(stdout);
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
    set_default_encoding(stdout);

    char username[MAXNAME_LENGTH], password[MAXNAME_LENGTH];
    do {
        scan_string("Username: ", username, MAXNAME_LENGTH);
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
        printf("Password: ");
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

        printf("Konfirmasi password: ");
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
        "\nRegistrasi akun telah selesai!\n"
        "Sekarang Anda dapat login ke dalam program dengan akun ini!\n");
    createuser(username, password, false, NULL, 0);
    await_enter();
}

void __exit_program() {
    clearscreen();

    set_utf8_encoding(stdout);

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

    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 51; j++)
            wprintf(L"%lc", msg[count++]);

        sleep(50);
    }
    
    wprintf(L"\n");

    set_default_encoding(stdout);

    exit(EXIT_SUCCESS);
}

void showlogin_menu() {
    clearscreen();

    set_utf8_encoding(stdout);
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

    set_default_encoding(stdout);
    bool isvalid = true;

    do {
        int choice = scan_number("Pilihan [0-2] >> ");
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
                printf("Pilihan tidak dapat ditemukan!\n");
                isvalid = false;
                break;
        }
    } while (!isvalid);
}
