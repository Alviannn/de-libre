#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE* file = fopen("ex_books.txt", "r");
    set_utf8_encoding(file);

    while (!feof(file)) {
        wchar_t title[MAXNAME_LENGTH],
            author[MAXNAME_LENGTH],
            borrower[MAXNAME_LENGTH];

        int pages;
        time_t duetime = 0;

        fwscanf(file, L"%l[^,], %l[^,], %d, %l[^\n]",
                title, author, &pages, borrower);
        fgetwc(file);

        createbook(++LAST_BOOK_ID, title, author, pages, borrower, duetime);
    }
    return 0;
}