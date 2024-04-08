#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum num1 = initBignum();
    Bignum num2 = initBignum();
    Bignum res1 = initBignum();

    setBignum(&num1, "12345", positive);
    setBignum(&num2, "98765", positive);

    subtractBignum(&res1, &num1, &num2);
    
    printf("\n\n");
    printBignum(&num1);
    printf(" + ");
    printBignum(&num2);
    printf(" = ");
    printBignum(&res1);
    printf("\n");

    freeBignums();

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
