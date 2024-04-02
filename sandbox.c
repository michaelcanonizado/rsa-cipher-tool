#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum.h"
#include <time.h>

void divideBignum(Bignum *quotient, Bignum *dividend, Bignum *divisor) {
    printf("START\n");
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

    // The sign of the quotient is determined by the signs of the dividend and divisor. If they are the same, the quotient is positive. If they are different, the quotient is negative.
    BIGNUM_SIGN quotientSign = dividendSign == divisorSign ? positive : negative;

    // Make dividend and divisor positive because the sign does not affect how the division is done.
    dividend->sign = positive;
    divisor->sign = positive;

    // Check if the dividend is less than the divisor. If it is, the quotient is 0 since the result is less than 1 and it will truncate down.
    if (isLessThanBignum(dividend, divisor)) {
        printf("Dividend is less than divisor\n");
        intToBignum(quotient, 0, positive);
        return;
    }

    // Create copies of the dividend and divisor.
    Bignum dividendCopy;
    copyBignum(&dividendCopy, dividend);
    Bignum divisorCopy;
    copyBignum(&divisorCopy, divisor);

    Bignum  quotientCopy = initBignum();
    setBignum(&quotientCopy, "0", positive);

    printf("Created copies\n");

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
    }

    printf("Finished long division\n");

    copyBignum(quotient, &quotientCopy);

    // Adjust the sign of the quotient
    quotient->sign = quotientSign;

    // Restore the signs of dividend and divisor
    dividend->sign = dividendSign;
    divisor->sign = divisorSign;
}

int main() {
    clock_t start = clock();

    Bignum num1 = initBignum();
    Bignum num2 = initBignum(); 
    Bignum res = initBignum(); 
    Bignum i = initBignum(); 
    setBignum(&num1, "120000", negative);
    setBignum(&num2, "2", positive);
    

 Bignum num1 = initBignum();
 Bignum num2 = initBignum(); 
 Bignum res = initBignum(); 
 Bignum i = initBignum(); 
 setBignum(&num1, "12", negative);
 setBignum(&num2, "5", positive);
 

 divideBignum(&res, &num1, &num2);
    
    // for (setBignum(&i, "0", positive); isLessThanBignum(&num1, &num2); incrementBignum(&num1, 2)) {
    //     // printBignum(&num1);
    //     for (int i = 3 ; i >= 0; i--) {
    //         printf("%d", num1.digits[i]);
    //     }
    //     printf("-");
    // }

  printf("\n");
  printBignum(&num1);
  printf(" / ");
  printBignum(&num2);
  printf(" = ");
  printBignum(&res);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU time used: %f seconds\n", cpu_time_used);
  return 0;
}
