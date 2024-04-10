#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum num1;
    Bignum num2;
    Bignum num3;
    Bignum res1;
    Bignum res2;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&num1);
    initBignum(&num2);
    initBignum(&num3);
    initBignum(&res1);
    initBignum(&res2);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum1 %p.%p", &num1, num1.digits);
    printf("\nnum2 %p.%p", &num2, num2.digits);
    printf("\nnum2 %p.%p", &num3, num3.digits);
    printf("\nres1 %p.%p", &res1, res1.digits);
    printf("\nres1 %p.%p", &res2, res2.digits);
    

    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    setBignum(&num1, "9876123123123123312", positive);
    setBignum(&num2, "2", positive);
    setBignum(&num3, "99999999999999912341", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    multiplyBignum(&res1, &num1, &num3);

    printf("\nMultiply = ");
    printBignumExtended(&res1, 50);

    divideBignum(&res1, &num1, &num2);

    printf("\nDivide = ");
    printBignumExtended(&res1, 50);

    halfBignum(&res2, &num1);

    printf("\nHalf = ");
    printBignumExtended(&res2, 50);

    resetBignum(&num1);
    resetBignum(&num2);
    resetBignum(&num3);
    resetBignum(&res1);
    resetBignum(&res2);

    printf("\nnum1 = ");
    printBignumExtended(&num1, 50);
    printf("\nnum2 = ");
    printBignumExtended(&num2, 50);
    printf("\nnum3 = ");
    printBignumExtended(&num3, 50);
    printf("\nres1 = ");
    printBignumExtended(&res1, 50);
    printf("\nres2 = ");
    printBignumExtended(&res2, 50);

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();

    freeAllBignums();


    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
