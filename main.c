#include "common.h"
#include "menu/admin.h"
#include "menu/login.h"
#include "menu/user.h"

void load_books() {
    struct _iobuf* file = fopen("books.txt", "r");

    while (!feof(file)) {
        int id, pages;
        char title[MAXNAME_LENGTH], author[MAXNAME_LENGTH], borrower[MAXNAME_LENGTH];

        fscanf(file, "%d, %[^,], %[^,], %d, %[^\n]", &id, title, author, &pages, borrower);
        fgetc(file);

        createbook(id, title, author, pages, borrower, time(NULL) - DAY_IN_SECONDS);
    }

    fclose(file);
}
int main() {
    // digunakan untuk format angka
    // misal: 100000 -> 100,000
    setlocale(LC_NUMERIC, "");

    // alokasi inisialisasi database user dan buku
    USER_LIST = safe_alloc(NULL, 1, sizeof(user_t));
    BOOK_LIST = safe_alloc(NULL, 1, sizeof(book_t));

    createuser("admin", "admin", true, NULL, 0);

    int ids[] = {35, 52, 120};
    createuser("Aan", "aan", false, ids, 3);
    load_books();

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

    if (USER_LIST != NULL)
        free(USER_LIST);
    if (BOOK_LIST != NULL)
        free(BOOK_LIST);

    ULENGTH = 0;
    BLENGTH = 0;

    return 0;
}