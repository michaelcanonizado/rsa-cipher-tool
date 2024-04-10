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
    Bignum res2;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&num1);
    initBignum(&num2);
    initBignum(&res1);
    initBignum(&res2);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum1 %p.%p", &num1, num1.digits);
    printf("\nnum2 %p.%p", &num2, num2.digits);
    printf("\nres1 %p.%p", &res1, res1.digits);
    printf("\nres1 %p.%p", &res2, res2.digits);
    

    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    setBignum(&num1, "98761231231231233123123124123123123123", positive);
    setBignum(&num2, "2", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    divideBignum(&res1, &num1, &num2);
    halfBignum(&res2, &num1);

    printf("\n\n");
    printBignum(&num1);
    printf(" ~ ");
    printBignum(&num2);
    printf(" = ");
    printBignum(&res1);

    printf("\n\n");
    printBignum(&num1);
    printf(" ~ ");
    printBignum(&num2);
    printf(" = ");
    printBignum(&res2);
    printf("\n");

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();

    freeAllBignums();


    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
