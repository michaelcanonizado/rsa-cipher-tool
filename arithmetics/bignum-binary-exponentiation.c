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

unsigned long long int binaryExponentiation(unsigned long long int base, Bignum *binaryExponent) {

    unsigned long long int remainder = 1;

    for (int i = 0; i < binaryExponent->length; i++) {

        if (binaryExponent->digits[i] == 1) {
            printf("\n 1.0 -> %llu = %llu * %llu", remainder * base,remainder, base);

            remainder = remainder * base;
        }

        if (i == binaryExponent->length - 1) {
            return remainder;
        }
        
        printf("\n 0.0 -> %llu = %llu * %llu", base * base, base, base);
        
        base = base * base;

        printf("\n----------------------------");
    }

    return remainder;

}

int main(void) {
    clock_t start = clock();

    Bignum base = initBignum(); 
    Bignum exponent = initBignum(); 
    Bignum binaryExponent = initBignum(); 
    Bignum result = initBignum();

    setBignum(&base, "12", positive);
    setBignum(&exponent, "12", positive);

    bignumToBinary(&binaryExponent, &exponent);

    unsigned long long baseInt = 12;
    unsigned long long exponentInt = 12;

    unsigned long long int resultInt = binaryExponentiation(baseInt, &binaryExponent);

    printf("\n\n\n");
    printBignum(&exponent);
    printf(" in binary: ");
    printBignum(&binaryExponent);
    printf("\nBinary Length: %llu", binaryExponent.length);

    printf("\n\n--------------------------------------\n----------------RESULT----------------\n--------------------------------------\n\n");
    
    printf("%llu  ^  %llu  =   %llu", baseInt, exponentInt, resultInt);

    // printBignum(&base);
    // printf("  ^  ");
    // printBignum(&exponent);
    // printf("  =  ");
    // printBignum(&result);

    
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}