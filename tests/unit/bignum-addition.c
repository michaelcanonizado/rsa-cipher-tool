#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../bignum.h"

void test(BIGNUM_SIGN integer1Sign, BIGNUM_SIGN integer2Sign, int numOfInterations) {
    srand(time(NULL));

    for (int i = 0; i < numOfInterations; i++) {

        long long int integer1, integer2;
    
        if (integer1Sign == positive) {
            integer1 = rand() % 9999 + 1000;
        } else {
            integer1 = rand() % 9000 - 9999;
        }

        if (integer2Sign == positive) {
            integer2 = rand() % 9999 + 1000;
        } else {
            integer2 = rand() % 9000 - 9999;
        }

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
        printf("\n");
    }

    printf("\n");
}

int main(void) {

    test(positive, positive, 5);
    test(negative, negative, 5);
    test(positive, negative, 5);
    test(negative, positive, 5);

    return 0;
}