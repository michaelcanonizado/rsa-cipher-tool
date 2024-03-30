#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../bignum.h"

int main(void) {
    clock_t begin = clock();

    printf("Hello World!\n");

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nELAPSED TIME CALCULATING: %lf seconds\n\n", time_spent);

    return 0;
}

