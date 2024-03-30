#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum.h"

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

    // The sign of the quotient is determined by the signs of the dividend and divisor. If they are the same, the quotient is positive. If they are different, the quotient is negative.
    BIGNUM_SIGN quotientSign = dividendSign == divisorSign ? positive : negative;

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
    int divisorLen = divisor->length;

    // Create copies of the dividend and divisor.
    // Copying is necessary because the dividend and divisor will be modified during the division process. It allows to perform division without changing the original dividend and divisor.
    Bignum dividendCopy;
    copyBignum(&dividendCopy, dividend);
    Bignum divisorCopy;
    copyBignum(&divisorCopy, divisor);

    // The length of the quotient is the length of the dividend minus the length of the divisor plus 1.
    int quotientLen = dividendLen - divisorLen + 1;

    // Creates an array to store the quotient, which will be converted to a Bignum later.
    int* quotientArr = (int*)malloc(quotientLen * sizeof(int));
    memset(quotientArr, 0, quotientLen * sizeof(int));
    
    int i, j;
   
    // Perform long division

    // Iterate through the dividend from the MSD to the LSD
    for (i = 0; i <= dividendLen - divisorLen; i++) {
        // While the dividend is greater than or equal to the divisor, subtract the divisor from the dividend
        while (isGreaterThanBignum(&dividendCopy, &divisorCopy) >= 0) {
            for (j = 0; j < divisorLen; j++) {
                dividendCopy.digits[i + j] -= divisorCopy.digits[j];
                // If the digit of the dividend is negative, borrow from the next digit. This happens when the current digit of the dividend is less than the current digit of the divisor.
                if (dividendCopy.digits[i + j] < 0) {
                    // Add 10 to the current digit of the dividend and subtract 1 from the next digit of the dividend.
                    dividendCopy.digits[i + j] += 10;
                    dividendCopy.digits[i + j + 1]--;
                }

            }
            // Increment the quotient
            quotientArr[i]++;
            // Check if the dividend is less than the divisor. If it is, break out of the loop.
            if (isLessThanBignum(&dividendCopy, &divisorCopy) != 0) {
                break;
            }
           
        }        
    } 
                
    // Convert the quotient array to a Bignum
    for ( i = 0; i < quotientLen; i++) {
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
}

int main() {

 Bignum num1 = initBignum();
 Bignum num2 = initBignum(); 
 Bignum res = initBignum(); 
 Bignum i = initBignum(); 
 setBignum(&num1, "12", negative);
 setBignum(&num2, "56", positive);
 

//  divideBignum(&res, &num1, &num2);
    
    for (setBignum(&i, "0", positive); isLessThanBignum(&num1, &num2); incrementBignum(&num1, 2)) {
        // printBignum(&num1);
        for (int i = 3 ; i >= 0; i--) {
            printf("%d", num1.digits[i]);
        }
        printf("-");
    }

  printf("\n");
  printBignum(&num1);
  printf(" * ");
  printBignum(&num2);
  printf(" = ");
  printBignum(&res);


  return 0;
}
