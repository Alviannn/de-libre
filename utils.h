#ifndef __UTILS_H__
#define __UTILS_H__

typedef int (*cmpfunc_t)(const void* a, const void* b);

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
 * @brief Membuat sebuah garis
 * 
 * @param dest   string yang dituju
 * @param length panjang garis
 */
void makeline(char* dest, size_t length);

/**
 * @brief Membaca string dari input seperti membaca password (tidak di echo hasil ketikannya)
 * 
 * @param dest string yang dituju
 * @param size panjang string
 */
void getpass(char* dest, size_t size);

/**
 * @brief Mengurutkan sebuah array dengan metode quick sort (bisa digunakan pada semua macam array, kecuali array of pointer)
 * 
 * @param base       array yang ingin disort
 * @param num_elems  banyak elemen pada array
 * @param size_elem  besar bytes sebuah elemen (contoh: `sizeof(int)`)
 * @param __cmp_func function comparator yang akan digunakan untuk membandingkan elemen-elemen dari array
 */
void quicksort(void* base, size_t num_elems, size_t size_elem, cmpfunc_t __cmp_func);

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
 * @param file file yang ditarget
 */
void set_utf8_encoding(FILE* file);

void set_default_encoding(FILE* file);

#endif  // ! __UTILS_H__