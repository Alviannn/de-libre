#include "common.h"
#include "menu/admin.h"
#include "menu/login.h"
#include "menu/user.h"

int main() {
    // digunakan untuk format angka
    // misal: 100000 -> 100,000
    setlocale(LC_NUMERIC, "");

    USER_LIST = malloc(sizeof(user_t));
    BOOK_LIST = malloc(sizeof(book_t));

    mkdir(DATABASE_PATH);

    load_books();
    load_users();

    if (finduser("admin") == -1) {
        createuser("admin", "admin", true, NULL, 0);
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

    if (USER_LIST != NULL)
        free(USER_LIST);
    if (BOOK_LIST != NULL)
        free(BOOK_LIST);

    ULENGTH = 0;
    BLENGTH = 0;

    return 0;
}