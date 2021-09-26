#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
    char name[21];
    int age;
} person;

person newpers(char* name, int age) {
    person p;

    strcpy(p.name, name);
    p.age = age;

    return p;
}

void* safe_alloc(void* mem, size_t num_elems, size_t elem_size) {
    void* tmp = NULL;

    // free memory yg ditargetkan jika kosong
    if (num_elems == 0) {
        if (mem != NULL) {
            free(mem);
            mem = NULL;
        }

        return NULL;
    }

    if (mem == NULL)
        tmp = malloc(num_elems * elem_size);
    else
        tmp = realloc(mem, num_elems * elem_size);

    // jika proses alokasi memory gagal, maka kita akan hentikan program
    // karena kegagalan ini hanya bisa terjadi karena tidak memiliki memory
    if (tmp == NULL) {
        wprintf(
            L"\n"
            L"Error: Failed to reallocate memory for %p!\n"
            L"       This is most likely caused because there's no memory available!\n",
            mem);

        if (mem != NULL)
            free(mem);

        exit(EXIT_FAILURE);
    } else {
        mem = tmp;
    }

    return mem;
}

int main() {
    person* list = NULL;
    list = safe_alloc(list, 1, sizeof(person));
    list[0] = newpers("Alvian", 10);
    list = safe_alloc(list, 0, sizeof(person));
    list = safe_alloc(list, 1, sizeof(person));
    list[0] = newpers("Alvian", 10);
    printf("Success\n");
}

// int main() {
//     int a[10];

//     for (int i = 0; i < 10; i++)
//         a[i] = i + 1;

//     memset(a + 5, 0, 5 * sizeof(int));
//     printf("%d\n", (a + 4)[0]);

//     for (int i = 0; i < 10; i++) {
//         printf("%d ", a[i]);
//     }
//     printf("\n");

//     return 0;
// }