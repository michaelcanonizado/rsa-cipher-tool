#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../bignum.h"

int bignumToBinary(Bignum *result, Bignum *num) {

    int remainderTemp;
    unsigned long long int binaryLength = 0;

    Bignum numTemp = initBignum(); 
    copyBignum(&numTemp, num);

    Bignum remainder = initBignum(); 
    Bignum two = initBignum(); 
    intToBignum(&two, 2, positive);

    while(!isBignumZero(&numTemp)) {
        if (numTemp.digits[0] % 2 == 0) {
            result->digits[binaryLength++] = 0;
        } else {
            result->digits[binaryLength++] = 1;
        }

        halfBignum(&numTemp, &numTemp);
    }

    result->length = binaryLength;
    return 0;
}

int main(void) {
    clock_t start = clock();

    Bignum base = initBignum(); 
    Bignum exponent = initBignum(); 
    Bignum binaryExponent = initBignum(); 
    Bignum result = initBignum();

    setBignum(&base, "123", positive);
    setBignum(&exponent, "123", positive);

    bignumToBinary(&binaryExponent, &exponent);
    
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