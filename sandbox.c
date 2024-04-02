#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum.h"
#include <time.h>

int main() {
    clock_t start = clock();

    Bignum num1 = initBignum();
    Bignum num2 = initBignum(); 
    Bignum res = initBignum();

    setBignum(&num1, "120000", positive);
    setBignum(&num2, "2", positive);

    divideBignum(&res, &num1, &num2);

    printf("\n");
    printBignum(&num1);
    printf(" / ");
    printBignum(&num2);
    printf(" = ");
    printBignum(&res);
    printf("\n");

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
