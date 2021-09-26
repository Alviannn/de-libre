#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ANSI_RED "\e[91m"
#define ANSI_WHITE "\e[97m"
#define ANSI_YELLOW "\e[33m"
#define ANSI_RESET "\e[0m"
#define ANSI_BLACK "\e[30m"
#define ANSIBG_WHITE "\e[47m"

int main() {
    system("cls");
    
    // FILE* file = fopen("color.txt", "w");
    FILE* file = fopen("color.txt", "r");

    char line[101];
    fgets(line, 101, file);
    printf(line);

    // printf(ANSI_RED "Indo" ANSI_WHITE "nesia" ANSI_RESET "\n");
    // fprintf(file, ANSI_RED "Indo" ANSI_WHITE "nesia" ANSI_RESET "\n");

    fclose(file);
    return 0;
}