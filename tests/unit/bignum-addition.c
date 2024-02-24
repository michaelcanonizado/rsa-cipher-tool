#include <stdio.h>
#include "../../bignum.h"

int main(void) {
    // Addition prints wrong
    Bignum num1 = initBignum(); 
    Bignum num2 = initBignum(); 
    Bignum result = initBignum();
    
    setBignum(&num1, "16", positive);
    setBignum(&num2, "007", negative);

    addBignum(&result, &num1, &num2);

    printf("\nsgn: %d | len: %d | num 1: ", num1.sign, num1.length);
    for (int i = num1.length - 1; i >= 0 ; i--) {
        printf("%d", num1.digits[i]);
    }
    printf("\nsgn: %d | len: %d | num 2: ", num2.sign, num2.length);
    for (int i = num2.length - 1; i >= 0 ; i--) {
        printf("%d", num2.digits[i]);
    }
    printf("\nsgn: %d | len: %d | result: ", result.sign, result.length);
    for (int i = result.length - 1; i >= 0 ; i--) {
        printf("%d", result.digits[i]);
    }
    
    printf("\n\nnum 1 > num 2 = %d", isGreaterThanBignum(&num1, &num2));
    printf("\nnum 1 < num 2 = %d", isLessThanBignum(&num1, &num2));
    printf("\nnum 1 == num 2 = %d", isEqualToBignum(&num1, &num2));
    
    printf("\n\n");

    return 0;
}