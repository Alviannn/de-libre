#ifndef __UTILS_H__
#define __UTILS_H__

typedef int (*cmpfunc_t)(const void* a, const void* b);

/**
 * @brief Waits for the user to press the enter key
 */
void await_enter();

/**
 * @brief Handles sleeping or delaying the program
 * 
 * @param millis the sleep time in milliseconds
 */
void sleep(long millis);

/**
 * @brief Clears the console screen
 */
void clearscreen();

/**
 * @brief Gets a number value from the input
 */
double scan_number(char* message);

/**
* @brief Gets a full line of string from input stream
*
* @param message the message that we can send
* @param dest the destination string
* @param size the string size
*/
void scan_string(char* message, char* dest, int size);

/**
 * @brief Sorts an array with quick sort method (works for any kind except pointers)
 * 
 * @param base the array
 * @param num_elems the total elements inside the array (or array length)
 * @param size_elem the size of each element
 */
void quicksort(void* base, size_t num_elems, size_t size_elem, cmpfunc_t __cmp_func);

#endif  // ! __UTILS_H__