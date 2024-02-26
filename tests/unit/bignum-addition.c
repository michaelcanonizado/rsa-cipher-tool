#include <stdio.h>
#include "../../bignum.h"

int main(void) {
    long long int integer1 = -12345;
    long long int integer2 = -67890;
    long long int integerResult = integer1 + integer2;

    Bignum bignum1 = initBignum(); 
    Bignum bignum2 = initBignum(); 
    Bignum bignumResult = initBignum();
    intToBignum(&bignum1, 12345, negative);
    intToBignum(&bignum2, 67890, negative);
    addBignum(&bignumResult, &bignum1, &bignum2);

    printBignumCenter(&bignum1, 10);
    printf("+");
    printBignumCenter(&bignum2, 10);
    printf("=");
    printBignumCenter(&bignumResult, 10);
    printf("~ %lld | ", integerResult);

    if (integerResult == bignumToInt(&bignumResult)) {
        printf("MATCH");
    } else {
        printf("MISMATCH");
    }

    printf("\n\n");

    return 0;
}