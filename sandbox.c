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

    printf("\nnum1 %p.%p", &num1, num1.digits);
    printf("\nnum2 %p.%p", &num2, num2.digits);
    printf("\nres1 %p.%p", &res1, res1.digits);
    

    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    setBignum(&num1, "987", positive);
    setBignum(&num2, "123", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    powerBignum(&res1, &num1, &num2);

    printf("\nnum1 = ");
    printBignum(&num1);
    printf("\nnum2 = ");
    printBignum(&num2);
    printf("\nres1 = ");
    printBignum(&res1);

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();

    freeAllBignums();


    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
