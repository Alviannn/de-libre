#include "common.h"

user_t* CURRENT_USER = NULL;
book_sort MAIN_BOOK_SORT = ID_SORT;

user_t* USER_LIST = NULL;
unsigned ULENGTH = 0;

book_t* BOOK_LIST = NULL;
unsigned BLENGTH = 0;

// -                                                      - //
// ---------------------- Comparator ---------------------- //
// -                                                      - //

int comparebook_asc(const void* a, const void* b) {
    book_t p = *(book_t*)a;
    book_t q = *(book_t*)b;

    switch (MAIN_BOOK_SORT) {
        case ID_SORT:
            return p.id - q.id;
        case TITLE_SORT:
            return strcmp(p.title, q.title);
        case AUTHOR_SORT:
            return strcmp(p.author, q.author);
        case PAGES_SORT:
            return p.pages - q.pages;
        case AVAILABILITY_SORT:
            if (!isbook_borrowed(p) && !isbook_borrowed(q))
                return p.id - q.id;
            else
                return !isbook_borrowed(q);
    }

    return p.id - q.id;
}

int comparebook_desc(const void* a, const void* b) {
    return comparebook_asc(b, a);
}

// -                                                     - //
// ---------------------- Functions ---------------------- //
// -                                                     - //

void* book_comparator(sort_type type) {
    if (type == ASCENDING)
        return comparebook_asc;
    else
        return comparebook_desc;
}

user_t createuser(char name[], char password[], bool isadmin, unsigned* book_ids, unsigned book_count) {
    user_t user;

    strcpy(user.name, name);
    strcpy(user.password, password);
    user.isadmin = isadmin;

    // clears the book_ids array
    memset(user.book_ids, 0, sizeof(int) * MAX_BORROW);

    // applies the book_ids array from the param to the attribute
    user.book_count = book_count;
    if (book_ids != NULL && book_count != 0)
        memcpy(user.book_ids, book_ids, sizeof(int) * book_count);

    // adds the user to the user array
    ULENGTH++;
    USER_LIST = realloc(USER_LIST, ULENGTH * sizeof(user_t));
    USER_LIST[ULENGTH - 1] = user;

    return user;
}

book_t createbook(unsigned id, char title[], char author[], unsigned pages, char borrower[], time_t btime) {
    book_t book;

    strcpy(book.title, title);
    strcpy(book.author, author);
    strcpy(book.borrower, borrower);

    book.id = id;
    book.pages = pages;
    book.btime = btime;

    // adds the book to the book array
    BLENGTH++;
    BOOK_LIST = realloc(BOOK_LIST, BLENGTH * sizeof(book_t));
    BOOK_LIST[BLENGTH - 1] = book;

    return book;
}

bool isbook_borrowed(book_t book) {
    return strlen(book.borrower) == 0 || strcmp(book.borrower, "-");
}

int finduser(char name[]) {
    int left = 0;
    int right = ULENGTH - 1;
    int mid = 0;

    do {
        mid = (left + right) / 2;
        user_t temp = USER_LIST[mid];

        if (strcmp(name, temp.name) == 0)
            return mid;

        if (strcmp(name, temp.name) < 0)
            right = mid - 1;
        if (strcmp(name, temp.name) > 0)
            left = mid + 1;
    } while (left <= right);

    return -1;
}

int findbook(unsigned id) {
    book_sort before = MAIN_BOOK_SORT;
    MAIN_BOOK_SORT = ID_SORT;

    quicksort(BOOK_LIST, BLENGTH, sizeof(book_t), book_comparator(ASCENDING));

    MAIN_BOOK_SORT = before;

    int left = 0;
    int right = BLENGTH - 1;
    int mid = 0;

    do {
        mid = (left + right) / 2;
        book_t temp = BOOK_LIST[mid];

        if (id == temp.id)
            return mid;

        if (id < temp.id)
            right = mid - 1;
        if (id > temp.id)
            left = mid + 1;
    } while (left <= right);

    return -1;
}

bookpack_t book_paginate(book_t* arr, unsigned length, book_sort name, sort_type type, int page) {
    int maxpage = (length / ELEMENTS_PER_PAGE) + (length % ELEMENTS_PER_PAGE == 0 ? 0 : 1);
    int len = ELEMENTS_PER_PAGE;

    // checks the selected page
    if (length == 0 || page < 1 || page > maxpage) {
        bookpack_t empty;

        empty.len = 0;
        empty.list = NULL;

        return empty;
    }

    // decide the books to be shown on the current page
    if (page == maxpage) {
        len = length % ELEMENTS_PER_PAGE;

        if (len == 0)
            len = ELEMENTS_PER_PAGE;
    }

    // sorts the main book array
    MAIN_BOOK_SORT = name;
    quicksort(BOOK_LIST, BLENGTH, sizeof(book_t), book_comparator(type));

    // creates the book pack (just for the pagination part)
    bookpack_t pack;
    pack.len = len;
    pack.list = (arr + (ELEMENTS_PER_PAGE * (page - 1)));

    return pack;
}