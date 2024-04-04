#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int bignumBinaryModularExponentiation(Bignum *result, Bignum *base, Bignum *binaryExponent, Bignum *divisor) {

    Bignum remainder = initBignum();
    copyBignum(&remainder, base);

    printf("\n");

    for (int i = binaryExponent->length - 1; i >= 0; i--) {
        Bignum remainderSquared = initBignum();
        Bignum remainderSquaredModDivisor = initBignum();

        if (binaryExponent->digits[i - 1] == 0) {
            Bignum tempRemainder = initBignum();
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
            Bignum remainderSquaredModDivisorTimesBase = initBignum();
            Bignum remainderSquaredModDivisorTimesBaseModDivisor = initBignum();

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

            ;

        }
        // FEAT: CHAIN ANOTHER CONDITION ABOVE TO CHECK IF THE NEXT NUMBER IS NEITHER A 1 OR 0. IF TRUE, RETURN AN ERROR AS THIS INDICATES THAT THE PASSED binaryExponent IS NOT VALID BINARY
        printf("\n----------------------------");

    }

    copyBignum(result, &remainder);

}

int main() {
    clock_t start = clock();

    Bignum base = initBignum(); 
    Bignum exponent = initBignum(); 
    Bignum binaryExponent = initBignum(); 
    Bignum divisor = initBignum(); 
    Bignum result = initBignum();

    setBignum(&base, "312397232412141211232397232412", positive);
    setBignum(&exponent, "231239723241211232397232412112323123972324121123239723241211232312397232412112323972324121123", positive);
    setBignum(&divisor, "4121123239723241211", positive);

    bignumToBinary(&binaryExponent, &exponent);
    bignumBinaryModularExponentiation(&result, &base, &binaryExponent, &divisor);
    
    printf("\n\n\n");
    printBignum(&exponent);
    printf(" in binary (%llu digits):\n", exponent.length);
    printBignum(&binaryExponent);
    printf("\n\nBinary Length: %llu", binaryExponent.length);

    printf("\n\n\nRESULT: ");
    printBignum(&result);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\n\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
