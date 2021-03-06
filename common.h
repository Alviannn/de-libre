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
#include <sys/stat.h>
#include <time.h>
#include <wchar.h>

/** Panjang maksimum string pada program ini */
#define MAXNAME_LENGTH 41
/** Banyak elemen pada sebuah halaman (untuk proses pembuatan halaman array buku) */
#define ELEMENTS_PER_PAGE 10
/** Maksimum banyak peminjaman buku per user */
#define MAX_BORROW 10
/** Jumlah detik setiap 1 hari atau 24 jam */
#define DAY_IN_SECONDS ((60L * 60L) * 24L)
/** Jumlah detik setiap 1 minggu atau 7 hari  */
#define WEEK_IN_SECONDS ((DAY_IN_SECONDS * 7L))
/** Denda default untuk user yg telat mengembalikan buku */
#define LATE_FINE 500L

// Alamat folder database utama
#define DATABASE_PATH "./DATABASE"
// Alamat file/folder database buku
#define BOOK_DATABASE_PATH DATABASE_PATH "/BOOK_DB"
// Alamat file database user
#define USER_DATABASE_PATH DATABASE_PATH "/user_db.txt"

// UTF-8 encoding category
#define UTF8_ENCODING 0x40000
// Default encoding category
#define DEFAULT_ENCODING 0x4000

#define ANSI_YELLOW "\e[33m"
#define ANSI_RESET "\e[0m"

// -                                                   - //
// ---------------------- Structs ---------------------- //
// -                                                   - //

typedef struct user_t {
    /** Username user */
    wchar_t name[MAXNAME_LENGTH];
    /** password user */
    wchar_t password[MAXNAME_LENGTH];
    /** Menentukan apakah user ini seorang admin atau member */
    bool isadmin;
    /** Banyak buku yg dipinjam user */
    int book_count;
    /** Buku yg dipinjam user (array ID buku, int) */
    int book_ids[MAX_BORROW];
} user_t;

typedef struct book_t {
    /** ID buku */
    int id;
    /** judul buku */
    wchar_t title[MAXNAME_LENGTH];
    /** penulis buku */
    wchar_t author[MAXNAME_LENGTH];
    /** tahun rilis buku */
    int relyear;
    /** jumlah halaman buku */
    int pages;
    /** Username peminjaman buku */
    wchar_t borrower[MAXNAME_LENGTH];
    /** Jatuh tempo peminjaman buku */
    time_t duetime;
} book_t;

/**
 * @brief Tipe pengurutan buku yg tersedia
 */
typedef enum book_sort {
    /** Mengurut berdasarkan ID buku */
    ID_SORT,
    /** Mengurut berdasarkan judul buku */
    TITLE_SORT,
    /** Mengurut berdasarkan penulis buku */
    AUTHOR_SORT,
    /** Mengurut berdasarkan tahun rilis buku */
    RELYEAR_SORT,
    /** Mengurut berdasarkan jumlah halaman buku */
    PAGES_SORT,
    /** Mengurut berdasarkan status peminjaman buku */
    AVAILABILITY_SORT
} book_sort;

typedef enum sort_type {
    /** Urutan keatas */
    ASCENDING,
    /** Urutan kebawah */
    DESCENDING
} sort_type;

typedef struct bookpaginate_t {
    /** Array buku yg dihalamankan */
    book_t* list;
    /** Banyak elemen pada array buku yg dihalamankan */
    int len;
    /** Halaman maksimum */
    int maxpage;
} bookpaginate_t;

// -                                                            - //
// ---------------------- Global Variables ---------------------- //
// -                                                            - //

/** ID buku yg terakhir dicatat */
extern int LAST_BOOK_ID;

/** User yg sedang menggunakan program (atau user pada saat ini) */
extern user_t* CURRENT_USER;
/** Pengurut buku utama */
extern book_sort MAIN_BOOK_SORT;

/** Database user (array user) */
extern user_t* USER_DB;
/** Banyak user pada database user */
extern size_t ULENGTH;

/** Database buku (array buku) */
extern book_t* BOOK_DB;
/** Banyak buku pada database buku */
extern size_t BLENGTH;

// -                                                     - //
// ---------------------- Functions ---------------------- //
// -                                                     - //

/**
 * @brief Menentukan function comparator yg akan digunakan untuk pengurutan
 */
void* book_comparator(sort_type type);

/**
 * @brief Membuat sebuah data struct buku, dan juga menambahkan user tersebut ke dalam database user
 * 
 * @param name       username user
 * @param password   password user
 * @param isadmin    status admin (true atau false)
 * @param book_ids   array id buku yg dipinjam
 * @param book_count banyak buku yg dipinjam
 * @return index dimana struct user ini berada
 */
int createuser(wchar_t name[], wchar_t password[], bool isadmin, int* book_ids, int book_count);

/**
 * @brief Membuat sebuah data struct buku, dan juga menambahkan buku tersebut ke dalam database buku
 * 
 * @param id       ID buku
 * @param title    judul buku
 * @param author   penulis buku
 * @param relyear  tahun rilis buku
 * @param pages    jumlah halaman buku
 * @param borrower username peminjam buku
 * @param duetime  jatuh tempo peminjaman buku
 * @return index dimana struct buku ini berada
 */
int createbook(int id, wchar_t title[], wchar_t author[], int relyear, int pages, wchar_t* borrower, time_t duetime);

/**
 * @brief Menghapus buku dari database buku berdasarkan ID buku
 * 
 * @param id ID buku yang ingin dihapus
 * @return true jika penghapusan berhasil, false jika sebaliknya
 */
bool removebook(int id);

/**
 * @brief Menentukan apakah sebuah buku sedang dipinjam atau tidak
 * 
 * @param book target pointer buku
 */
bool isbook_borrowed(const book_t* book);

/**
 * @brief Menentukan apakah sebuah buku memiliki isi yang dapat dibaca
 * 
 * @param book target pointer buku
 * @return true jika buku memiliki isi, false jika sebaliknya
 */
bool book_hascontent(const book_t* book);

/**
 * @brief Mencari sebuah user berdasarkan username (dengan menggunakan metode binary search)
 *
 * @param name username yang dicari
 * @return Index user yang dicari, tetapi akan return -1 jika user tidak dapat ditemukan
 */
int finduser(wchar_t name[]);

/**
 * @brief Mencari sebuah buku berdasarkan ID buku (dengan menggunakan metode binary search)
 * 
 * @param id ID buku yang dicari
 * @return index buku yang dicari, tetapi akan return -1 jika buku tidak dapat ditemukan
 */
int findbook(int id);

/**
 * @brief Mencari sebuah buku berdasarkan judul buku (dengan menggunakan metode binary search)
 * 
 * @param title judul buku yang dicari
 * @return index buku yang dicari, tetapi akan return -1 jika buku tidak dapat ditemukan
 */
int findbook_title(wchar_t title[]);

/**
 * @brief Membuat halaman (maks. 10 elemen per halaman)
 * 
 * @param arr  target array buku
 * @param len  panjang array buku
 * @param name tipe pengurutan buku (lihat enum book_sort)
 * @param type tipe pengurutan (keatas atau kebawah)
 * @param page halaman yg dituju
 * @return hasil penghalaman array buku
 */
bookpaginate_t book_paginate(book_t* arr, int len, book_sort name, sort_type type, int page);

/**
 * @brief Melihat detail buku
 * 
 * @param book buku yang dituju
 */
void view_book(book_t* book);

/**
 * @brief Prompt untuk seleksi tipe sortingan buku
 * 
 * @return enum tipe sortingan buku, -1 jika gagal
 */
int select_booksort();

/**
 * @brief Prompt untuk seleksi tipe sortingan
 * 
 * @return enum tipe sortingan, -1 jika gagal
 */
int select_sorttype();

/**
 * @brief Menunggu user untuk mengkonfirmasi jawabannya
 * 
 * @param message pesan yang akan dikirimkan kepada user
 * @return true jika mendapatkan jawaban Iya, false jika Tidak
 */
bool await_confirmation(wchar_t* message);

/**
 * @brief Meyimpan database buku
 */
void save_books();

/**
 * @brief Membaca database buku pada file
 */
void load_books();

/**
 * @brief Meyimpan database user pada file
 */
void save_users();

/**
 * @brief Membaca database user pada file
 */
void load_users();

/**
 * @brief Membaca buku pada halaman tertentu
 * 
 * @param current Pointer buku yang ingin dibaca
 * @param page halaman pada buku
 * @return true jika buku dapat dibaca, false jika sebaliknya
 */
bool readbook(book_t* current, int page);

/**
 * @brief Menampilkan buku-buku yang sedang dipinjm user
 * 
 * @return true jika user ada buku yang dipinjam, false jika tidak ada buku yang dipinjam
 */
bool show_borrowed_books();

/**
 * @brief Menampilkan buku-buku yang tersedia dan sudah dihalamankan.
 *      Pada function ini terdapat pilihan untuk melakukan berbagai macam hal
 *      terutama untuk mengurutkan buku, memasuki halaman selanjutnya, dan sebagainya.
 * 
 * @param header    pesan header sebelum menampilkan pilihan
 * @param on_select function yang akan dieksekusi pada saat user memilih sebuah buku
 */
void showbooks(wchar_t* header, void (*on_select)(book_t* target));

// -                                               - //
// ---------------------- End ---------------------- //
// -                                               - //

// local libraries
#include "utils.h"

#endif  // ! __COMMON_H__