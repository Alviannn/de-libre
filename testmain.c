#include "common.h"
#include "menu/user.h"

void insertbooks() {
    FILE* file = fopen("test/books.txt", "r");

    int id, pages;
    char title[101], author[101], borrower[101];

    while (!feof(file)) {
        fscanf(file, "%d, %[^,], %[^,], %d, %[^\n]", &id, title, author, &pages, borrower);
        fgetc(file);

        createbook(id, title, author, pages, borrower, time(NULL) - DAY_IN_SECONDS);
    }

    fclose(file);
}

void printbooks() {
    for (int i = 0; i < (int)BLENGTH; i++) {
        book_t tmp = BOOK_LIST[i];
        printf("(%d) {id=%d, title=%s, author=%s, pages=%d, borrower=%s}\n", i + 1, tmp.id, tmp.title, tmp.author, tmp.pages, tmp.borrower);
    }
}

int main() {
    setlocale(LC_NUMERIC, "");
    
    USER_LIST = safe_alloc(USER_LIST, 0, sizeof(user_t));
    BOOK_LIST = safe_alloc(BOOK_LIST, 0, sizeof(book_t));

    int ids[] = {52, 120, 35};

    insertbooks();
    createuser("admin", "admin", true, NULL, 0);
    createuser("Aan", "aan", false, ids, 3);

    int idx = findbook(22);
    removebook(idx);

    CURRENT_USER = &USER_LIST[1];

    while (true) {
        // resets the main sorting point
        MAIN_BOOK_SORT = ID_SORT;

        if (CURRENT_USER == NULL) {
            // login_menu();
            break;
        } else {
            if (CURRENT_USER->isadmin)
                break;
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