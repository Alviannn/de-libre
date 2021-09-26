#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printarr(int* arr, int len) {
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int len = 10;
    int arr[len];

    for (int i = 0; i < len; i++)
        arr[i] = i + 1;

    printf("Before: ");
    printarr(arr, len);

    int idx = 0;
    printf("Index: %d\n", idx);
    printf("Value: %d\n", arr[idx]);

    if (idx + 1 == len) {
        arr[idx] = 0;
    } else {
        memcpy((arr + idx), (arr + idx + 1), sizeof(int) * (len - idx - 1));
        arr[len - 1] = 0;
    }

    printf("After : ");
    printarr(arr, len);
    return 0;
}