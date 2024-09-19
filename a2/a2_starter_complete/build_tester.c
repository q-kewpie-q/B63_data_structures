#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "minheap.h"

void generateWorstCaseInput(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
}

double measureTime(MinHeap* (*buildHeapFunc)(int[], int), int* arr, int size) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    MinHeap* heap = buildHeapFunc(arr, size);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Free the heap after use
    deleteHeap(heap);

    return cpu_time_used;
}

int main() {
    int sizes[] = {10, 100, 1000, 5000};
    // int sizes[] = {10, 100, 1000, 5000, 10000, 50000, 100000, 500000, 1000000}; 
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    double times_Sajad[numSizes];
    double times_Elaheh[numSizes];

    for (int i = 0; i < numSizes; i++) {
        int size = sizes[i];
        int* arr = (int*)malloc(size * sizeof(int));
        generateWorstCaseInput(arr, size);

        double total_time_Sajad = 0.0;
        double total_time_Elaheh = 0.0;

        for (int j = 0; j < 100; j++) {
            total_time_Sajad += measureTime(buildHeap_Sajad, arr, size);
            total_time_Elaheh += measureTime(buildHeap_Elaheh, arr, size);
        }

        times_Sajad[i] = total_time_Sajad / 100;
        times_Elaheh[i] = total_time_Elaheh / 100;

        free(arr);
    }

    // Print the results
    printf("Size\tTime_Sajad\tTime_Elaheh\n");
    for (int i = 0; i < numSizes; i++) {
        printf("%d\t%f\t%f\n", sizes[i], times_Sajad[i], times_Elaheh[i]);
    }

    return 0;
}
