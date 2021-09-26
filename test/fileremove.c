#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char target[101] = "kevin14";

    FILE* file = fopen("db.txt", "r");
    FILE* tmp = fopen("tmp.txt", "w");

    char name[101], pass[101];
    char line[257];

    while (1) {
        int sres = fscanf(file, "%[^\n]\n%[^\n]", name, pass);
        if (sres == EOF || fgetc(file) == EOF)
            break;

        if (strcmp(target, name) != 0)
            fprintf(tmp, "%s\n%s\n", name, pass);
    }

    fclose(tmp);
    fclose(file);

    tmp = fopen("tmp.txt", "r");
    file = fopen("db.txt", "w");

    while (fgets(line, 257, tmp) != NULL) {
        fprintf(file, line);
    }

    fclose(file);
    fclose(tmp);

    _sleep(10000);

    remove("tmp.txt");
    printf("Success!\n");

    return 0;
}