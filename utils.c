#include "common.h"
#include "utils.h"

void await_enter() {
    printf("Press enter to continue...");
    getchar();
    fflush(stdin);
}

void sleep(long millis) {
    clock_t expected = clock() + millis;

    while (clock() < expected) {
        // do nothing
    }
}

void clearscreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

double scan_number(char* message) {
    double input = 0;
    int scanresult = 0;

    do {
        if (message != NULL)
            printf(message);

        scanresult = scanf("%lf", &input);
        fflush(stdin);

        if (scanresult != 0)
            break;

        // printf("This input scanning only accepts numbers!\n");
        printf("Input ini hanya dapat menerima angka!\n");
    } while (1);

    return input;
}

void scan_string(char* message, char* dest, int size) {
    if (message != NULL)
        printf(message);

    fgets(dest, size + 1, stdin);
    if (dest[strlen(dest) - 1] == '\n')
        dest[strlen(dest) - 1] = 0;

    fflush(stdin);
}

void makeline(char* dest, size_t length) {
    strcpy(dest, "");
    memset(dest, 196, length);
}

void getpass(char* dest, size_t size) {
    char key = 0;

    size_t count = 0;
    strcpy(dest, "");

    // as long as it's not the enter key, keep going
    while ((key = _getch()) != 13) {
        // does what backspace does
        if (key == 8) {
            count--;
            dest[count] = 0;
            continue;   
        }

        // if it's arrow key, skip
        if (key == 0)
            _getch();
        // if the key is invalid or has reached the max size, skip
        if (key < 32 || key > 126 || count >= size - 1)
            continue;

        dest[count] = key;
        count++;
    }

    // string ends with a NULL character, so here it is
    dest[size - 1] = 0;
}

void* elem_from_bytes(void* base, int idx, size_t size_elem) {
    return (base + (size_elem * idx));
}

void __swap(const void* a, const void* b, size_t size) {
    char *p = (char*)a,
         *q = (char*)b;

    for (size_t i = 0; i < size; i++) {
        char temp = p[i];

        p[i] = q[i];
        q[i] = temp;
    }
}

int __partition(void* base, size_t size_elem, int low, int high, cmpfunc_t __cmp_func) {
    int idx = low - 1;
    void* pivot = elem_from_bytes(base, high, size_elem);

    for (int i = low; i <= high - 1; i++) {
        void* temp = elem_from_bytes(base, i, size_elem);

        if (__cmp_func(pivot, temp) > 0) {
            idx++;

            void *p = elem_from_bytes(base, i, size_elem),
                 *q = elem_from_bytes(base, idx, size_elem);

            __swap(p, q, size_elem);
        }
    }

    idx++;

    void *p = elem_from_bytes(base, idx, size_elem),
         *q = elem_from_bytes(base, high, size_elem);

    __swap(p, q, size_elem);

    return idx;
}

void __handle_quicksort(void* base, size_t size_elem, int low, int high, cmpfunc_t __cmp_func) {
    if (low < high) {
        int pi = __partition(base, size_elem, low, high, __cmp_func);

        __handle_quicksort(base, size_elem, low, pi - 1, __cmp_func);
        __handle_quicksort(base, size_elem, pi + 1, high, __cmp_func);
    }
}

void quicksort(void* base, size_t num_elems, size_t size_elem, cmpfunc_t __cmp_func) {
    __handle_quicksort(base, size_elem, 0, num_elems - 1, __cmp_func);
}