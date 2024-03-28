#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../../bignum.h"

void reverseBignum(Bignum* num) {
    int start = 0;
    int end = num->length - 1;
    while (start < end) {
        int temp = num->digits[start];
        num->digits[start] = num->digits[end];
        num->digits[end] = temp;
        start++;
        end--;
    }
}

void divideBignum(Bignum *quotient, Bignum *dividend, Bignum *divisor) {
    
    // Checks if the divisor is zero. If it is, the function will return 0.
    if (isBignumZero(divisor)) {
        printf("Division by zero is not allowed.\n");
        return;
    }

    // Checks if the dividend is zero. If it is, the function will return.
    if (isBignumZero(dividend)) {
        intToBignum(quotient, 0, positive);
        return;
    } 
    
    // Check the signs of the dividend and divisor
    BIGNUM_SIGN dividendSign = dividend->sign;
    BIGNUM_SIGN divisorSign = divisor->sign;
    BIGNUM_SIGN quotientSign = dividendSign == divisorSign ? positive : negative;

    //printf("Dividend sign: %d\n", dividendSign);
    //printf("Divisor sign: %d\n", divisorSign);
    //printf("Quotient sign: %d\n", quotientSign);

    // Make dividend and divisor positive because the sign does not affect how the division is done
    dividend->sign = positive;
    divisor->sign = positive;

    // Check if the dividend is less than the divisor
    // If it is, the quotient is 0
    if (isLessThanBignum(dividend, divisor)) {
        intToBignum(quotient, 0, positive);
        return;
    }

    // Get the lengths of the dividend and divisor
    int dividendLen = dividend->length;
    //printf("Dividend length: %d\n", dividendLen);
    int divisorLen = divisor->length;
    //printf("Divisor length: %d\n", divisorLen);

    // Create copies of the dividend and divisor
    Bignum dividendCopy;
    copyBignum(&dividendCopy, dividend);
    Bignum divisorCopy;
    copyBignum(&divisorCopy, divisor);

    //Reverse the Bignum
    reverseBignum(&dividendCopy);
    reverseBignum(&dividendCopy);
/*
    printf("Dividend copy: ");
    printBignum(&dividendCopy);
    printf("\n");
    printf("Divisor copy: ");
    printBignum(&divisorCopy);
    printf("\n");
*/

    // Perform long division
    int quotientLen = dividendLen - divisorLen + 1;
    int* quotientArr = (int*)malloc(quotientLen * sizeof(int));
    memset(quotientArr, 0, quotientLen * sizeof(int));
    
    int i, j;
   
    for (i = 0; i <= dividendLen - divisorLen; i++) {
        while (isGreaterThanBignum(&dividendCopy, &divisorCopy) >= 0) {
            for (j = 0; j < divisorLen; j++) {
                    //printf("dividendCopy.digits[%d]  = (%d) \n", i + j, dividendCopy.digits[i + j]);
                dividendCopy.digits[i + j] -= divisorCopy.digits[j];
                    //printf("- divisorCopy.digits[%d] = (%d)\n", j, divisorCopy.digits[j]);
                    //printf("\tdividendCopy.digits[%d] = (%d) \n", i + j, dividendCopy.digits[i + j]);
                if (dividendCopy.digits[i + j] < 0) {
                    dividendCopy.digits[i + j] += 10;
                    dividendCopy.digits[i + j + 1]--;
                }

            }
            quotientArr[i]++;
            if (isLessThanBignum(&dividendCopy, &divisorCopy) != 0) {
                break;
            }
           
        }        
    }
       
                
    // Convert the quotient array to a Bignum
    for (int i = 0; i < quotientLen; i++) {
        quotient->digits[i] = quotientArr[i];
    }
    quotient->length = quotientLen;

    // Adjust the sign of the quotient
    quotient->sign = quotientSign;

    // Restore the signs of dividend and divisor
    dividend->sign = dividendSign;
    divisor->sign = divisorSign;

    // Free memory
    free(quotientArr);
    
    /*
    printf("Quotient sign: %d\n", quotientSign);

    printf("Quotient: ");
    for (int i = 0; i < quotient->length; i++) {
        printf("%d", quotient->digits[i]);
    }
    printf("\n");
    */
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

int main() {

    printf("\n%7s   / %7s   = %7s    ~ %10s | %s\n", "BN1", "BN2", "RESULT", "EXPECTED RESULT", "STATUS");
    printf("\n-----------------------------------------------------\n\n");

    testSign(positive, positive, 5);
    testSign(negative, negative, 5);
    testSign(positive, negative, 5);
    testSign(negative, positive, 5);
    //testZero(5);
    
return 0;
}