#include "common.h"
#include "menu/admin.h"
#include "menu/login.h"
#include "menu/user.h"

int main() {
    // digunakan untuk format angka
    // misal: 100000 -> 100,000
    setlocale(LC_NUMERIC, "");

    set_utf8_encoding(stdout);
    set_utf8_encoding(stdin);

    USER_DB = malloc(sizeof(user_t));
    BOOK_DB = malloc(sizeof(book_t));

    mkdir(DATABASE_PATH);

    load_books();
    load_users();

    if (finduser(L"admin") == -1) {
        createuser(L"admin", L"admin", true, NULL, 0);
        save_users();
    }

    while (true) {
        // Reset tipe pengurutan buku utama
        MAIN_BOOK_SORT = ID_SORT;

        if (CURRENT_USER == NULL) {  // Jika user belum login, tunjukkan menu login
            showlogin_menu();
        } else {  // Jika tidak tunjukkan menu admin atau user
            if (CURRENT_USER->isadmin)
                showadmin_menu();
            else
                showuser_menu();
        }
    }

    save_users();
    save_books();

    if (USER_DB != NULL)
        free(USER_DB);
    if (BOOK_DB != NULL)
        free(BOOK_DB);

    ULENGTH = 0;
    BLENGTH = 0;

    return 0;
}