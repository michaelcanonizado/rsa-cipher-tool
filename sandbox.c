#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum num;
    Bignum x;
    Bignum y;
    Bignum z;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&num);
    initBignum(&x);
    initBignum(&y);
    initBignum(&z);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum %p.%p", &num, num.digits);
    printf("\nx %p.%p", &x, x.digits);
    printf("\ny %p.%p", &y, y.digits);
    printf("\nz %p.%p", &z, z.digits);
    
    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    setBignum(&x, "560", positive);
    setBignum(&y, "560", positive);
    // setBignum(&x, "560", positive);
    // setBignum(&y, "2", positive);

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    unsigned long long int primeLength = 20;

    generatePrimeBignum(&num, primeLength);

    printf("\nnum: ");
    printBignumExtended(&num, primeLength + 5);
    printf("\n\n\nx: ");
    printBignum(&x);
    printf("\n\n\ny: ");
    printBignum(&y);
    printf("\n\n\nz: ");
    printBignum(&z);

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();
    freeAllBignums();
    printBignumNodeList();

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
