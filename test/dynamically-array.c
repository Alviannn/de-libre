#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef struct person_t {
//     char name[21];
//     int age;
// } person_t;

// person_t* list = NULL;
// int len = 0;

// void printperson(person_t person) {
//     printf("{name=%s, age=%d}\n", person.name, person.age);
// }

// void printlist() {
//     for (int i = 0; i < len; i++) {
//         printf("%d - ", i + 1);
//         printperson(list[i]);
//     }
// }

// void addperson(char name[], int age) {
//     person_t tmp;
//     memset(&tmp, 0, sizeof(person_t));

//     strcpy(tmp.name, name);
//     tmp.age = age;

//     len++;
//     safe_alloc(list, len, sizeof(person_t));
//     list[len - 1] = tmp;
// }

// void removeperson(int idx) {
//     len--;

//     if (idx != len)
//         memcpy((list + idx), (list + idx + 1), sizeof(person_t) * (len - idx));

//     safe_alloc(list, len, sizeof(person_t));
// }

void printbytes(void* a, size_t size) {
    char* lo = (char*)a;
    for (size_t i = 0; i < size; i++) {
        printf("%d ", lo[i]);
    }
    printf("\n");
}

struct buku {
    char* title;
    int pages;
};

int main() {
    struct buku a;

    a.title = malloc(sizeof(char) * 10);
    strcpy(a.title, "aksjda");

    // printf("Starting...\n");

    // int* a = safe_alloc(NULL, 3, sizeof(int));
    // printf("%p\n", a);

    // a[0] = 15;
    // a[1] = 32;
    // a[2] = 87;

    // safe_alloc(a, 5, sizeof(int));
    // printf("%p\n", a);
    // a[3] = 22;
    // a[4] = 23;

    // for (int i = 0; i < 5; i++) {
    //     printf("%d ", a[i]);
    // }

    return 0;
}

// int main() {
//     list = calloc(0, sizeof(person_t));
//     len = 0;

//     addperson("Alvian", 19); // 0
//     addperson("Gafi", 19);   // 1
//     addperson("Alex", 18);   // 2
//     addperson("Fajar", 18);  // 3

//     printlist();
//     printf("\n");

//     removeperson(3);

//     printlist();
//     printf("\n");

//     person_t* a = malloc(sizeof(person_t));

//     // free(list);
//     char* lo = (char*)list;

//     for (int i = 0; i < (int)sizeof(person_t) * (len + 1); i++) {
//         printf("%d ", lo[i]);
//         if ((i + 1) % (int)sizeof(person_t) == 0)
//             printf("\n");
//     }

//     printf("\n");
//     // printperson((list + len - 1)[0]);

//     return 0;
// }