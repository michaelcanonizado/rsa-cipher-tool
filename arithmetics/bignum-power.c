#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../bignum.h"

int main(void) {
    // Start CPU timer
    clock_t begin = clock();

    Bignum x = initBignum();
    Bignum n = initBignum();
    Bignum result = initBignum();

    setBignum(&x, "123", positive);
    setBignum(&n, "456", positive);

    printf("\n\nBase: ");
    printBignum(&x);
    printf("\nExponent: ");
    printBignum(&n);
    printf("\nResult: ");
    printBignum(&result);

    // End CPU timer and print elapsed time
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nELAPSED TIME CALCULATING: %lf seconds\n\n", time_spent);

    return 0;
}

