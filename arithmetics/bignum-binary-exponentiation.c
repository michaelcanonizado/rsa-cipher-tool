#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../bignum.h"

int bignumToBinary(Bignum *result, Bignum *num) {

    int remainderTemp;
    unsigned long long int binaryLength = 0;

    Bignum numTemp; 
    Bignum remainder; 
    Bignum two; 

    initBignum(&numTemp); 
    initBignum(&remainder); 
    initBignum(&two); 

    copyBignum(&numTemp, num);

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

    freeBignum(&numTemp); 
    freeBignum(&remainder); 
    freeBignum(&two); 

    return 0;
}

unsigned long long int binaryExponentiation(unsigned long long int base, Bignum *binaryExponent) {
    // Reference: https://www.youtube.com/watch?v=9VEqjAZxmeA&t=387s

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

int bignumBinaryExponentiation(Bignum *result, Bignum *base, Bignum *binaryExponent) {
    // Reference: https://www.youtube.com/watch?v=9VEqjAZxmeA&t=387s

    Bignum remainder;
    Bignum baseCopy;

    initBignum(&remainder);
    initBignum(&baseCopy);

    setBignum(&remainder, "1", positive);
    copyBignum(&baseCopy, base);

    Bignum tempRemainder;
    Bignum tempBase;
    initBignum(&tempRemainder);
    initBignum(&tempBase);

    for (unsigned long long int i = 0; i < binaryExponent->length; i++) {
        resetBignum(&tempRemainder);
        resetBignum(&tempBase);

        setBignum(&tempRemainder, "1", positive);

        if (binaryExponent->digits[i] == 1) {
            multiplyBignum(&tempRemainder, &remainder, &baseCopy);

            printf("\n 1.0 -> ");
            printBignum(&tempRemainder);
            printf(" = ");
            printBignum(&remainder);
            printf(" * ");
            printBignum(&baseCopy);

        }

        // if (i == binaryExponent->length - 1) {
        //     return remainder;
        // }
        if (i == binaryExponent->length - 1) {
            copyBignum(result, &tempRemainder);

            freeBignum(&remainder);
            freeBignum(&baseCopy);
            freeBignum(&tempRemainder);
            freeBignum(&tempBase);

            return 0;
        }
        
        // base = base * base;
        multiplyBignum(&tempBase, &baseCopy, &baseCopy);

        printf("\n 0.0 -> ");
        printBignum(&tempBase);
        printf(" = ");
        printBignum(&baseCopy);
        printf(" * ");
        printBignum(&baseCopy);

        copyBignum(&remainder, &tempRemainder);
        copyBignum(&baseCopy, &tempBase);

        printf("\n----------------------------");
    }

    copyBignum(result, &remainder);

    freeBignum(&remainder);
    freeBignum(&baseCopy);
    freeBignum(&tempRemainder);
    freeBignum(&tempBase);

    return 0;
}

int main(void) {
    clock_t start = clock();

    Bignum base; 
    Bignum exponent; 
    Bignum binaryExponent; 
    Bignum result;

    initBignum(&base); 
    initBignum(&exponent); 
    initBignum(&binaryExponent); 
    initBignum(&result);

    setBignum(&base, "987654321987654321", positive);
    setBignum(&exponent, "12", positive);

    bignumToBinary(&binaryExponent, &exponent);

    printf("\n\n\n");
    printBignum(&exponent);
    printf(" in binary: ");
    printBignum(&binaryExponent);
    printf("\nBinary Length: %llu", binaryExponent.length);

    unsigned long long baseInt = 15;
    unsigned long long exponentInt = 16;

    unsigned long long int resultInt;
    // resultInt = binaryExponentiation(baseInt, &binaryExponent);

    printf("\n\n----------------------------\n\n");

    bignumBinaryExponentiation(&result, &base, &binaryExponent);

    printf("\n\n----------------------------------------------\n--------------------RESULT--------------------\n----------------------------------------------\n\n");
    
    printf("INT:  %llu  ^  %llu  =   %llu", baseInt, exponentInt, resultInt);

    printf("\n\nBIGNUM:  ");
    printBignum(&base);
    printf("  ^  ");
    printBignum(&exponent);
    printf("  =  ");
    printBignum(&result);

    printf("\n\nRESULT LENGTH: %llu\n", result.length);

    printBignumNodeList();

    freeAllBignums();
    
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}