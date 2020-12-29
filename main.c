#include "common.h"
#include "menu/user.h"

void insertbooks() {
    FILE* file = fopen("books.txt", "r");

    int id, pages;
    char title[101], author[101], borrower[101];

    while (!feof(file)) {
        fscanf(file, "%d, %[^,], %[^,], %d, %[^\n]", &id, title, author, &pages, borrower);
        fgetc(file);

        createbook(id, title, author, pages, borrower, 0);
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
    USER_LIST = calloc(0, sizeof(user_t));
    BOOK_LIST = calloc(0, sizeof(book_t));

    unsigned ids[] = {52, 120, 35};

    insertbooks();
    createuser("admin", "admin", true, NULL, 0);
    createuser("Aan", "aan", false, ids, 3);

    CURRENT_USER = &USER_LIST[1];

    while (true) {
        // resets the main sorting point
        MAIN_BOOK_SORT = ID_SORT;

        if (CURRENT_USER == NULL) {
            printf("User is empty!\n");
            exit(EXIT_SUCCESS);
        } else {
            showuser_menu();
        }
    }

    return 0;
}