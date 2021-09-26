#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void swap(void* a, void* b, size_t size) {
    char* p = (char*)a;
    char* q = (char*)b;

    for (size_t i = 0; i < size; i++) {
        char temp = p[i];

        p[i] = q[i];
        q[i] = temp;
    }
}

void* arr_elem(void* base, size_t index, size_t size) {
    return (base + (index * size));
}

void randomize(void* base, size_t nums, size_t size) {
    for (size_t i = 0; i < nums; i++) {
        size_t p = rand() % nums;
        size_t q = rand() % nums;

        if (p == q)
            continue;

        swap(arr_elem(base, p, size), arr_elem(base, q, size), size);
    }
}

size_t __partition(void* base, size_t low, size_t high, size_t size, int (*cmpfunc)(const void*, const void*)) {
    void* pivot = arr_elem(base, high, size);
    size_t idx = (low - 1);

    for (size_t i = low; i < high; i++) {
        void* a = arr_elem(base, i, size);

        if (cmpfunc(pivot, a) > 0) {
            idx++;

            void* b = arr_elem(base, idx, size);
            swap(a, b, size);
        }
    }

    idx++;

    void* a = arr_elem(base, idx, size);
    void* b = arr_elem(base, high, size);

    swap(a, b, size);

    return idx;
}

void __handlesort(void* base, size_t low, size_t high, size_t size, int (*cmpfunc)(const void*, const void*)) {
    if (low < high) {
        size_t pi = __partition(base, low, high, size, cmpfunc);

        __handlesort(base, low, pi - 1, size, cmpfunc);
        __handlesort(base, pi + 1, high, size, cmpfunc);
    }
}

void quicksort(void* base, size_t nums, size_t size, int (*cmpfunc)(const void*, const void*)) {
    if (nums == 0)
        return;

    randomize(base, nums, size);
    __handlesort(base, 0, nums - 1, size, cmpfunc);
}

int compare(const void* a, const void* b) {
    int p = *(int*)a;
    int q = *(int*)b;

    return p - q;
}

void printarr(int arr[], size_t size) {
    for (size_t i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    srand(time(NULL));

    int len = 30;
    int arr[len];

    for (int i = 0; i < len; i++)
        arr[i] = i + 1;

    printarr(arr, len);
    randomize(arr, len, sizeof(int));
    printarr(arr, len);

    // for (int i = 0; i < 100; i++) {
    //     printf("%d\n", rand() % 15);
    // }

    return 0;
}