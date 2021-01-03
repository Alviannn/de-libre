#include "login.h"

void __login_user() {
    clearscreen();
    int useridx = -1;
    char username[MAXNAME_LENGTH], password[MAXNAME_LENGTH];

    do {
        scan_string("Username [0 untuk kembali]: ", username, MAXNAME_LENGTH);
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
        printf("Password [0 untuk kembali]: ");
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
    // Note registrasi
    // - Username tidak boleh ada spasi, panjang minimal 3 character
    // - Password harus ada angka dan character, minimal panjang 5 character

    clearscreen();
    char username[MAXNAME_LENGTH], password[MAXNAME_LENGTH];

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
        "\nRegistrasi akun telah selesai!\n"
        "Sekarang Anda dapat login ke dalam program dengan akun ini!\n");
    createuser(username, password, false, NULL, 0);
    await_enter();
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
                // todo: pesan keluar?
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Pilihan tidak dapat ditemukan!\n");
                isvalid = false;
                break;
        }
    } while (!isvalid);
}
