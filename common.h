/**
 * @file common.h
 * 
 * @brief This holds every common functions or libraries
 *          that will be used within the program
 */

#ifndef __COMMON_H__
#define __COMMON_H__

// built-ins libraries
#include <conio.h>
#include <ctype.h>
#include <dirent.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// local libraries
#include "utils.h"

/** Maximum name length */
#define MAXNAME_LENGTH 41
/** The elements shown each page */
#define ELEMENTS_PER_PAGE 10
/** The maximum borrowed books per user */
#define MAX_BORROW 10
/** The secinds for a day or 24 hours */
#define DAY_IN_SECONDS ((60L * 60L) * 24L)
/** The seconds for 7 days or 1 week */
#define WEEK_IN_SECONDS ((DAY_IN_SECONDS * 7L))
/** The fine when a user returns the book late */
#define LATE_FINE 500L

// -                                                   - //
// ---------------------- Structs ---------------------- //
// -                                                   - //

typedef struct user_t {
    /** the user's username */
    char name[MAXNAME_LENGTH];
    /** the user's password */
    char password[MAXNAME_LENGTH];
    /** determines whether the user is an admin or member */
    bool isadmin;
    /** The amount of borrowed books */
    unsigned book_count;
    /** The borrowed books (array of book id) */
    unsigned book_ids[MAX_BORROW];
} user_t;

typedef struct book_t {
    /** the book id*/
    unsigned id;
    /** the book title */
    char title[MAXNAME_LENGTH];
    /** the book author */
    char author[MAXNAME_LENGTH];
    /** the book total page */
    unsigned pages;
    /** the username who borrows the book */
    char borrower[MAXNAME_LENGTH];
    /** the due date for the currently borrowed book */
    time_t duetime;
} book_t;

/**
 * @brief All available book sorting
 */
typedef enum book_sort {
    /** sorts by book id */
    ID_SORT,
    /** sorts by book title */
    TITLE_SORT,
    /** sorts by book author */
    AUTHOR_SORT,
    /** sorts by total book pages (number) */
    PAGES_SORT,
    /** sorts by book availablity (is it borrowed?) */
    AVAILABILITY_SORT
} book_sort;

typedef enum sort_type {
    ASCENDING,
    DESCENDING
} sort_type;

typedef struct bookpack_t {
    /** the paginated book array */
    book_t* list;
    /** the length of the elements */
    unsigned len;
    /** the maximum page*/
    unsigned maxpage;
} bookpack_t;

// -                                                            - //
// ---------------------- Global Variables ---------------------- //
// -                                                            - //

/** The logged in user */
extern user_t* CURRENT_USER;
/** The user based book sort */
extern book_sort MAIN_BOOK_SORT;

/** The user array */
extern user_t* USER_LIST;
/** The user list length */
extern unsigned ULENGTH;

/** The book array */
extern book_t* BOOK_LIST;
/** The book list length */
extern unsigned BLENGTH;

// -                                                     - //
// ---------------------- Functions ---------------------- //
// -                                                     - //

/**
 * @brief Decides the book comparator function to be used
 */
void* book_comparator(sort_type type);

/**
 * @brief Create a user instance and then also adds it to the user array
 */
user_t createuser(char name[], char password[], bool isadmin, unsigned* book_ids, unsigned book_count);

/**
 * @brief Create a book instance and then also adds it to the book array
 */
book_t createbook(unsigned id, char title[], char author[], unsigned pages, char borrower[], time_t btime);

/**
 * @brief Determines if a book is being borrowed or not
 */
bool isbook_borrowed(book_t book);

/**
 * @brief Finds a user by the username (using binary search method)
 *
 * @return the index to the user, will return -1 if none is found
 */
int finduser(char name[]);

/**
 * @brief Finds a book by the id (using binary search method)
 * 
 * @return the index to the book, will return -1 if none is found
 */
int findbook(unsigned id);

/**
 * @brief Creates a pagination for a book array
 */
bookpack_t book_paginate(book_t* arr, unsigned len, book_sort name, sort_type type, int page);

// -                                               - //
// ---------------------- End ---------------------- //
// -                                               - //

#endif  // ! __COMMON_H__