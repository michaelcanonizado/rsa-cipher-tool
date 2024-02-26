#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../bignum.h"

int main(void) {
    srand(time(NULL));

    long long int integer1 = rand() % 19999 - 9999;
    long long int integer2 = rand() % 19999 - 9999;
    long long int integerResult = integer1 + integer2;

    Bignum bignum1 = initBignum(); 
    Bignum bignum2 = initBignum(); 
    Bignum bignumResult = initBignum();

    if (integer1 < 0){
        intToBignum(&bignum1, integer1 * -1, negative);
    } else {
        intToBignum(&bignum1, integer1, positive);
    } 
    
    if (integer2 < 0) {   
        intToBignum(&bignum2, integer2 * -1, negative);
    } else {
        intToBignum(&bignum2, integer2, positive);
    }

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