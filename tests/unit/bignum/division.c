#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../../bignum.h"

void testSign(BIGNUM_SIGN integer1Sign, BIGNUM_SIGN integer2Sign, int numOfInterations) {
    srand(time(NULL));

    for (int i = 0; i < numOfInterations; i++) {

        long long int integer1, integer2;
    
        if (integer1Sign == positive) {
            integer1 = 100000 + rand() % (999999 - 100000 + 1);
        } else {
            integer1 = -999999 + rand() % (-100000 - -999999 + 1);
        }

        if (integer2Sign == positive) {
            integer2 = 100000 + rand() % (999999 - 100000 + 1);
        } else {
            integer2 = -999999 + rand() % (-100000 - -999999 + 1);
        }

        long long int integerResult = integer1 / integer2;

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

        divideBignum(&bignumResult, &bignum1, &bignum2);

        printBignumCenter(&bignum1, 10);
        printf("/");
        printBignumCenter(&bignum2, 10);
        printf("=");
        printBignumCenter(&bignumResult, 10);
        printf("~ %lld | ", integerResult);

        printf("| %s", integerResult == bignumToInt(&bignumResult) ? "MATCH" : "MISMATCH");
        printf("\n");
    }
    printf("\n");
}

void testZero(int numOfInterations) {
    srand(time(NULL));

    for (int i = 0; i < numOfInterations; i++) {

        long long int integer1, integer2;
    
        integer1 = rand() % 19999 - 9999;
        integer2 = 0;

        long long int integerResult = 0 ;

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
        
        
        divideBignum(&bignumResult, &bignum1, &bignum2);
        if (integer2 != 0) {
            
            integerResult = integer1 / integer2;
        } else {
            integerResult = 0;
        }

        printBignumCenter(&bignum1, 10);
        printf("/");
        printBignumCenter(&bignum2, 10);
        printf("=");
        printBignumCenter(&bignumResult, 10);
        printf("~ %10lld ", integerResult);

        printf("| %s", integerResult == bignumToInt(&bignumResult) ? "MATCH" : "MISMATCH");

        printf("\n");
    }
        printf("\n");

    for (int i = 0; i < numOfInterations; i++) {
        long long int integer1, integer2;
    
        integer1 = 0;
        integer2 = rand() % 19999 - 9999;

        long long int integerResult = 0;

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
    
        divideBignum(&bignumResult, &bignum1, &bignum2);
        if (integer1 != 0) {
            integerResult = integer1 / integer2;
        } else {
            integerResult = 0;
        }

        printBignumCenter(&bignum1, 10);
        printf("/");
        printBignumCenter(&bignum2, 10);
        printf("=");
        printBignumCenter(&bignumResult, 10);
        printf("~ %10lld ", integerResult);

        printf("| %s", integerResult == bignumToInt(&bignumResult) ? "MATCH" : "MISMATCH");

        printf("\n");
    }

    printf("\n");
}

int main() {

    printf("\n%7s   / %7s   = %7s    ~ %10s | %s\n", "BN1", "BN2", "RESULT", "EXPECTED RESULT", "STATUS");
    printf("\n-----------------------------------------------------\n\n");

    testSign(positive, positive, 5);
    testSign(negative, negative, 5);
    testSign(positive, negative, 5);
    testSign(negative, positive, 5);
    testZero(5);
    
return 0;
}