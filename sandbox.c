#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum num1;
    Bignum num2;
    Bignum res1;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&num1);
    initBignum(&num2);
    initBignum(&res1);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum1 %p", &num1);
    printf("\nnum2 %p", &num2);
    printf("\nres1 %p", &res1);
    

    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    setBignum(&num1, "9876234267", positive);
    setBignum(&num2, "12", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    multiplyBignum(&res1, &num1, &num2);

    printf("\n\n");
    printBignum(&num1);
    printf(" ~ ");
    printBignum(&num2);
    printf(" = ");
    printBignum(&res1);
    printf("\n");

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    freeAllBignums();

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
