#ifndef __UTILS_H__
#define __UTILS_H__

typedef long long ll;
typedef int (*cmpfunc_book_t)(const book_t* a, const book_t* b);
typedef int (*cmpfunc_user_t)(const user_t* a, const user_t* b);

/**
 * @brief Menunggu user untuk menekan 'enter'
 */
void await_enter();

/**
 * @brief Delay program atau menghentikan proses program dalam jangka waktu tertentu
 * 
 * @param millis lama delay program dalam milliseconds
 */
void sleep(long millis);

/**
 * @brief Membersihkan layar console
 */
void clearscreen();

/**
 * @brief Mengambil input berupa angka
 * 
 * @param message pesan sebelum input (contoh: "Masukkan angka: ")
 * @return angka yang didapatkan dari input
 */
double scan_number(char* message);

/**
* @brief Mengambil string atau text lengkap dari input
*
* @param message pesan sebelum input (contoh: "Input nama: ")
* @param dest    string yang dituju
* @param size    panjang string
*/
void scan_string(char* message, char* dest, size_t size);

/**
 * @brief Membaca string dari input seperti membaca password (tidak di echo hasil ketikannya)
 * 
 * @param dest string yang dituju
 * @param size panjang string
 */
void getpass(char* dest, size_t size);

/**
 * @brief Mengalokasikan sebuah memory dengan aman,
 *      kamu dapat menggunakan function ini untuk menginisialisasi sebuah memory
 *      atau juga mengalokasikan kembali memory yang diinginkan.
 * 
 *      dan jika kamu ketemu dengan sesuatu seperti Out Of Memory (kehabisan memory) 
 *      maka function ini akan menghentikan program kita.
 *       
 *      NOTE: Disarankan bahwa kamu menggunakan nilai return dari function ini
 *          karena ada kemungkinan jika pada parameter `mem` tidak memiliki alamat memory
 *          maka hasil alokasi memory tersebut terambil.
 * 
 * @param mem       variable pointer (atau memory) yang dituju (bisa hanya mengisi NULL)
 * @param num_elems banyak elemen pada tujuan memory (kalau ingin dipakai untuk 1 variable, isikan `1` saja)
 * @param elem_size besar bytes sebuah elemen (contoh: `sizeof(int)`)
 * @return hasil alokasi memory yang dilakukan
 */
void* safe_alloc(void* mem, size_t num_elems, size_t elem_size);

/**
 * @brief Menerapkan encoding UTF-8 ke dalam sebuah target file
 * 
 * @param file file yang ditargetkan
 */
void set_utf8_encoding(FILE* file);

/**
 * @brief Menerapkan encoding default C ke dalam sebuah target file
 * 
 * @param file file yang ditargetkan
 */
void set_default_encoding(FILE* file);

/**
 * @brief Mengurutkan array user dengan menggunakan metode quick sort
 * 
 * @param base array user
 * @param length panjang array user
 * @param __cmpfunc function comparator khusus untuk user array
 */
void quicksort_user(user_t* base, size_t length, cmpfunc_user_t __cmpfunc);

/**
 * @brief Mengurutkan array book dengan menggunakan metode quick sort
 * 
 * @param base array book
 * @param length panjang array book
 * @param __cmpfunc function comparator khusus untuk book array
 */
void quicksort_book(book_t* base, size_t length, cmpfunc_book_t __cmpfunc);

/**
 * @brief Menentukan apakah file itu adalah sebuah folder atau bukan
 * 
 * @param path alamat file
 * @return true jika file yang ditentui adalah sebuah folder, false jika sebaliknya
 */
bool isdir(char* path);

/**
 * @brief Menghapus sebuah folder atau file secara rekursif
 * 
 * @param path alamat file yang ingin dihapuskan
 */
void delete_recursively(char* path);

#endif  // ! __UTILS_H__