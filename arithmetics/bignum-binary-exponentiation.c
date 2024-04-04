#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../bignum.h"

int main(void) {
    clock_t start = clock();

    Bignum base = initBignum(); 
    Bignum exponent = initBignum(); 
    Bignum binaryExponent = initBignum(); 
    Bignum result = initBignum();

    setBignum(&base, "123", positive);
    setBignum(&exponent, "123", positive);
    
    printf("\n\n\n");
    printBignum(&exponent);
    printf(" in binary: ");
    printBignum(&binaryExponent);
    printf("\nBinary Length: %llu", binaryExponent.length);

    printf("\n\n--------------------------------------\n----------------RESULT----------------\n--------------------------------------\n\n");
    printBignum(&base);
    printf("  ^  ");
    printBignum(&exponent);
    printf("  =  ");
    printBignum(&result);

    
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}