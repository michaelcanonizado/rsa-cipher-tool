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

    // Create copies of the dividend and divisor.
    Bignum dividendCopy;
    copyBignum(&dividendCopy, dividend);
    Bignum divisorCopy;
    copyBignum(&divisorCopy, divisor);

    Bignum  quotientCopy = initBignum();
    setBignum(&quotientCopy, "0", positive);

    // Perform long division
    // While the dividend is greater than or equal to the divisor, subtract the divisor from the dividend
    while (isGreaterThanBignum(&dividendCopy, &divisorCopy) >= 0) {
        // Subtracts divisor from the dividend and the result becomes the new dividend
        subtractBignum(&dividendCopy, &dividendCopy, &divisorCopy);
        // Increment the quotient
        incrementBignum(&quotientCopy, 1);
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

int main() {

Bignum num1 = initBignum();
Bignum num2 = initBignum(); 
Bignum res = initBignum(); 
Bignum i = initBignum(); 
setBignum(&num1, "1200", negative);
setBignum(&num2, "56", positive);
 

divideBignum(&res, &num1, &num2);

// divideBignumPrototype(&res, &num1, &num2);

    
for (setBignum(&i, "0", positive); isLessThanBignum(&num1, &num2); incrementBignum(&num1, 2)) {
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
