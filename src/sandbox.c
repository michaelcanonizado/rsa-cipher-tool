#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"


int main() {
    clock_t start = clock();

    Bignum num;
    Bignum divisor;
    Bignum result;
    Bignum x,y,z;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&num);
    initBignum(&divisor);
    initBignum(&result);

    initBignum(&x);
    initBignum(&y);
    initBignum(&z);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum %p.%p", &num, num.digits);
    printf("\nnum %p.%p", &divisor, divisor.digits);
    printf("\nnum %p.%p", &result, result.digits);
    
    printf("\n\n\n......................SETTING BIGNUMS.....................\n");

    // setBignum(&num, "3", positive);
    // setBignum(&divisor, "5", positive);
    setBignum(&num, "44834971845417975103", positive);
    setBignum(&divisor, "5526234165830510182307447364749930980548", positive);


    setBignum(&x, "0", positive);
    setBignum(&y, "1", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    modularInverseBignum(&result, &num, &divisor);
    // subtractBignum(&z, &x, &y);

    printf("\n\nnum: ");
    printBignum(&num);
    printf("\ndivisor: ");
    printBignum(&divisor);
    printf("\nresult: ");
    printBignum(&result);

    printf("\n\nx: ");
    printBignum(&x);
    printf("\ny: ");
    printBignum(&y);
    printf("\nz: ");
    printBignum(&z);

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();
    freeAllBignums();

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
