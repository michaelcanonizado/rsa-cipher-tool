#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum num;

    printf("\n......................INITIALIZEING BIGNUMS.................\n");

    initBignum(&num);

    printf("\n\n\n......................ADDRESS OF BIGNUMS....................\n");

    printf("\nnum %p.%p", &num, num.digits);
    
    printf("\n\n\n......................SETTING OF BIGNUMS.....................\n");

    printf("\n\n\n....................PERFORMING OPERATIONS....................\n");

    unsigned long long int primeLength = 20;

    generatePrimeBignum(&num, primeLength);

    printf("\nnum: ");
    printBignumExtended(&num, primeLength + 5);

    printf("\n\n\n......................FREEING BIGNUMS.........................\n");

    printBignumNodeList();

    freeAllBignums();

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
