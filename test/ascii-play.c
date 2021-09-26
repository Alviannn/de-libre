#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void set_utf8(FILE* file) {
    _setmode(_fileno(file), _O_U8TEXT);
}

int main() {
    // printf(
    //     "%c%c%c\n"
    //     "%c %c\n"
    //     "%c%c%c\n", 218, 196, 191, 179, 179, 192, 196, 217);

    // printf("UTF8: %d\n", _O_U8TEXT);

    // set_utf8(stdout);

    // wchar_t line[101];

    // FILE* file = fopen64("ascii.txt", "r");
    // _setmode(_fileno(file), _O_U8TEXT);

    // while (fgetws(line, 101, file) != NULL) {
    //     wprintf(L"%ls", line);
    // }

    // fclose(file);
    // wprintf(L"ο Δικαιοπολις εν αγρω εστιν\n");

    // printf("·\n");


    // wchar_t line[116];
    // wcscpy(line, L"──────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

    // wprintf(line);

    set_utf8(stdout);
    wprintf(L"█\n");

    printf("hello\n");

    // set_utf8(stdout);
    // wprintf(L"ο Δικαιοπολις εν αγρω εστιν\n");
    // _setmode(_fileno(stdout), _O_TEXT);
    // printf("hello world\n");
    return 0;
}