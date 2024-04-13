#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum base;
    Bignum exponent;
    Bignum divisor;
    Bignum result;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&base);
    initBignum(&exponent);
    initBignum(&divisor);
    initBignum(&result);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum1 %p.%p", &base, base.digits);
    printf("\nnum2 %p.%p", &exponent, exponent.digits);
    printf("\nres1 %p.%p", &divisor, divisor.digits);
    printf("\nres1 %p.%p", &result, result.digits);
    

    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    // setBignum(&base, "83", positive);
    // setBignum(&exponent, "2312", positive);
    // setBignum(&divisor, "2313", positive);
    setBignum(&base, "839371", positive);
    setBignum(&exponent, "231393718393712309371213937137122", positive);
    setBignum(&divisor, "231393718393712309371213937137123", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    modularExponentiationBignum(&result, &base, &exponent, &divisor);
    // powerBignum(&result, &base, &exponent);

    printf("\n");
    printBignum(&base);
    printf("\n^\n");
    printBignum(&exponent);
    printf("\nmod\n");
    printBignum(&divisor);
    printf("\n=\n");
    printBignum(&result);

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();

    freeAllBignums();

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
