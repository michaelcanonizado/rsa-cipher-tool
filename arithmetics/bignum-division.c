#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../bignum.h"

void divideBignum(Bignum *quotient, Bignum *dividend, Bignum *divisor) {
    
    // Checks if the divisor or the dividend is zero. If it is, the quotient is 0.
    if (isBignumZero(divisor)) {
        intToBignum(quotient, 0, positive);
        return;
    }
    if (isBignumZero(dividend)) {
        intToBignum(quotient, 0, positive);
        return;
    } 
    
    // Check the signs of the dividend and divisor
    BIGNUM_SIGN dividendSign = dividend->sign;
    BIGNUM_SIGN divisorSign = divisor->sign;

//    printf("Dividend sign: %d\n", dividendSign);
//    printf("Divisor sign: %d\n", divisorSign);

    // The sign of the quotient is determined by the signs of the dividend and divisor. If they are the same, the quotient is positive. If they are different, the quotient is negative.
    BIGNUM_SIGN quotientSign = dividendSign == divisorSign ? positive : negative;

//    printf("Quotient sign: %d\n", quotientSign);

    // Make dividend and divisor positive because the sign does not affect how the division is done.
    dividend->sign = positive;
    divisor->sign = positive;

    // Check if the dividend is less than the divisor. If it is, the quotient is 0 since the result is less than 1 and it will truncate down.
    if (isLessThanBignum(dividend, divisor)) {
        intToBignum(quotient, 0, positive);
        return;
    }

    // Get the lengths of the dividend and divisor
    int dividendLen = dividend->length;
    printf("Dividend length: %d\n", dividendLen);
    int divisorLen = divisor->length;
    printf("Divisor length: %d\n", divisorLen);

    // Create copies of the dividend and divisor.
    // Copying is necessary because the dividend and divisor will be modified during the division process. It allows to perform division without changing the original dividend and divisor.
    Bignum dividendCopy;
    copyBignum(&dividendCopy, dividend);
    Bignum divisorCopy;
    copyBignum(&divisorCopy, divisor);
    
    printf("Dividend copy: ");
    printBignum(&dividendCopy);
    printf("\n");
    printf("Divisor copy: ");
    printBignum(&divisorCopy);
    printf("\n");

    // The length of the quotient is the length of the dividend minus the length of the divisor plus 1.
    int quotientLen = dividendLen - divisorLen + 1;

    Bignum  quotientCopy = initBignum();
    
    int i, j;
   
    // Perform long division

    // While the dividend is greater than or equal to the divisor, subtract the divisor from the dividend
    while (isGreaterThanBignum(&dividendCopy, &divisorCopy) >= 0) {
        // Subtracts divisor from the dividend and the result becomes the new dividend
        printBignum(&dividendCopy);
        printf("\n");
        subtractBignum(&dividendCopy, &dividendCopy, &divisorCopy);
        printBignum(&dividendCopy);
        printf("\n");
        // Increment the quotient
        incrementBignum(&quotientCopy, 1);
        printBignum(&quotientCopy);
        printf("\n");
        // Check if the dividend is less than the divisor. If it is, break out of the loop.
        if (isLessThanBignum(&dividendCopy, &divisorCopy) != 0) break;
    }

    copyBignum(quotient, &quotientCopy);

    // Adjust the sign of the quotient
    quotient->sign = quotientSign;

    // Restore the signs of dividend and divisor
    dividend->sign = dividendSign;
    divisor->sign = divisorSign;
}

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