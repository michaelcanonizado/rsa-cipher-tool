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

    printf("\nbase %p.%p", &base, base.digits);
    printf("\nexponent %p.%p", &exponent, exponent.digits);
    printf("\ndivisor %p.%p", &divisor, divisor.digits);
    printf("\nresult %p.%p", &result, result.digits);
    
    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    // setBignum(&base, "83", positive);
    // setBignum(&exponent, "2312", positive);
    // setBignum(&divisor, "2313", positive);
    setBignum(&base, "8393718", positive);
    setBignum(&exponent, "231393718393712309371213937138", positive);
    setBignum(&divisor, "231393718393712309371213937139", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    modularExponentiationBignum(&result, &base, &exponent, &divisor);

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
