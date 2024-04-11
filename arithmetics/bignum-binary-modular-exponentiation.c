#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    copyBignum(&numTemp, num);

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

    return 0;
}

int bignumBinaryModularExponentiation(Bignum *result, Bignum *base, Bignum *binaryExponent, Bignum *divisor) {

    Bignum remainder;
    initBignum(&remainder);
    copyBignum(&remainder, base);

    printf("\n");

    Bignum remainderSquared;
    Bignum remainderSquaredModDivisor;
    initBignum(&remainderSquared);
    initBignum(&remainderSquaredModDivisor);

    Bignum tempRemainder;
    initBignum(&tempRemainder);

    Bignum remainderSquaredModDivisorTimesBase;
    Bignum remainderSquaredModDivisorTimesBaseModDivisor;
    initBignum(&remainderSquaredModDivisorTimesBase);
    initBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

    for (int i = binaryExponent->length - 1; i >= 0; i--) {
        resetBignum(&remainderSquared);
        resetBignum(&remainderSquaredModDivisor);

        if (binaryExponent->digits[i - 1] == 0) {
            resetBignum(&tempRemainder);
            copyBignum(&tempRemainder, &remainder);

            multiplyBignum(&remainderSquared, &remainder, &remainder);
            moduloBignum(&remainderSquaredModDivisor, &remainderSquared, divisor);

            copyBignum(&remainder, &remainderSquaredModDivisor);

            printf("\n 0.0 -> ");
            printBignum(&remainder);
            printf(" = ");
            printBignum(&tempRemainder);
            printf("^2 mod ");
            printBignum(divisor);

        } else if (binaryExponent->digits[i - 1] == 1) {
            resetBignum(&remainderSquaredModDivisorTimesBase);
            resetBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

            multiplyBignum(&remainderSquared, &remainder, &remainder);
            moduloBignum(&remainderSquaredModDivisor, &remainderSquared, divisor);

            printf("\n 1.0 -> ");
            printBignum(&remainderSquaredModDivisor);
            printf(" = ");
            printBignum(&remainder);
            printf("^2 mod ");
            printBignum(divisor);
            
            multiplyBignum(&remainderSquaredModDivisorTimesBase, &remainderSquaredModDivisor, base);
            moduloBignum(&remainderSquaredModDivisorTimesBaseModDivisor, &remainderSquaredModDivisorTimesBase,divisor);

            printf("\n 1.1 -> ");
            printBignum(&remainderSquaredModDivisorTimesBaseModDivisor);
            printf(" = ");
            printBignum(&remainderSquaredModDivisorTimesBase);
            printf(" mod ");
            printBignum(divisor);

            copyBignum(&remainder, &remainderSquaredModDivisorTimesBaseModDivisor);
        }
        // FEAT: CHAIN ANOTHER CONDITION ABOVE TO CHECK IF THE NEXT NUMBER IS NEITHER A 1 OR 0. IF TRUE, RETURN AN ERROR AS THIS INDICATES THAT THE PASSED binaryExponent IS NOT VALID BINARY
        printf("\n----------------------------");
    }

    copyBignum(result, &remainder);

    freeBignum(&remainder);

    freeBignum(&remainderSquared);
    freeBignum(&remainderSquaredModDivisor);

    freeBignum(&tempRemainder);

    freeBignum(&remainderSquaredModDivisorTimesBase);
    freeBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

    return 0;
}

int bignumBinaryModularExponentiationCompressed(Bignum *result, Bignum *base, Bignum *binaryExponent, Bignum *divisor) {

    Bignum remainder;
    initBignum(&remainder);
    copyBignum(&remainder, base);

    Bignum remainderSquared;
    Bignum remainderSquaredModDivisor;
    initBignum(&remainderSquared);
    initBignum(&remainderSquaredModDivisor);

    Bignum tempRemainder;
    initBignum(&tempRemainder);

    Bignum remainderSquaredModDivisorTimesBase;
    Bignum remainderSquaredModDivisorTimesBaseModDivisor;
    initBignum(&remainderSquaredModDivisorTimesBase);
    initBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

    for (int i = binaryExponent->length - 1; i >= 0; i--) {
        resetBignum(&remainderSquared);
        resetBignum(&remainderSquaredModDivisor);

        if (binaryExponent->digits[i - 1] == 0) {
            resetBignum(&tempRemainder);
            copyBignum(&tempRemainder, &remainder);

            multiplyBignum(&remainderSquared, &remainder, &remainder);
            moduloBignum(&remainderSquaredModDivisor, &remainderSquared, divisor);

            copyBignum(&remainder, &remainderSquaredModDivisor);
        } else if (binaryExponent->digits[i - 1] == 1) {
            resetBignum(&remainderSquaredModDivisorTimesBase);
            resetBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

            multiplyBignum(&remainderSquared, &remainder, &remainder);
            moduloBignum(&remainderSquaredModDivisor, &remainderSquared, divisor);
            
            multiplyBignum(&remainderSquaredModDivisorTimesBase, &remainderSquaredModDivisor, base);
            moduloBignum(&remainderSquaredModDivisorTimesBaseModDivisor, &remainderSquaredModDivisorTimesBase,divisor);

            copyBignum(&remainder, &remainderSquaredModDivisorTimesBaseModDivisor);
        }
    }

    copyBignum(result, &remainder);

    freeBignum(&remainder);

    freeBignum(&remainderSquared);
    freeBignum(&remainderSquaredModDivisor);

    freeBignum(&tempRemainder);

    freeBignum(&remainderSquaredModDivisorTimesBase);
    freeBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

    return 0;
}

int main() {
    clock_t start = clock();

    Bignum base; 
    Bignum exponent; 
    Bignum binaryExponent; 
    Bignum divisor; 
    Bignum result;

    initBignum(&base); 
    initBignum(&exponent); 
    initBignum(&binaryExponent); 
    initBignum(&divisor); 
    initBignum(&result);

    setBignum(&base, "8393718393712309371230148391483937123", positive);
    setBignum(&exponent, "2313937183937123093712139371371230114839237123093712139371371230168", positive);
    setBignum(&divisor, "2313937183937123093712139371371230114839237123093712139371371230179", positive);

    bignumToBinary(&binaryExponent, &exponent);
    
    printf("\n\n\n");
    printBignum(&exponent);
    printf(" in binary (%llu digits):\n", exponent.length);
    printBignum(&binaryExponent);
    printf("\n\nBinary Length: %llu", binaryExponent.length);

    bignumBinaryModularExponentiationCompressed(&result, &base, &binaryExponent, &divisor);

    printf("\n\n\nRESULT: ");
    printBignum(&result);

    freeAllBignums();

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
