#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct person_t {
    char name[51];
    int age;
} person_t;

person_t newperson(char* name, int age) {
    person_t tmp;

    strcpy(tmp.name, name);
    tmp.age = age;

    return tmp;
}

int main() {
    person_t* a = calloc(3, sizeof(person_t));
    person_t* b = calloc(3, sizeof(person_t));

    a[0] = newperson("Alvian", 19);
    a[1] = newperson("Bobi", 21);
    a[2] = newperson("Cacing", 3);

    b = a;
    strcpy(a[0].name, "Honeyworks");

    for (int i = 0; i < 3; i++) {
        printf("%s - %s\n", a[i].name, b[i].name);
    }

    return 0;
}