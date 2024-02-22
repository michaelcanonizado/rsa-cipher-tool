#include <stdio.h>
#include "../../bignum.h"

int main(void) {
    Bignum num1 = initBignum();
    
    setBignum(&num1, "9223372036854775807", negative);

    printf("\nsgn: %d | len: %d | num 1: ", num1.sign, num1.length);
    for (int i = num1.length - 1; i >= 0 ; i--) {
        printf("%d", num1.digits[i]);
    }
    
    long long int result = bignumToInt(&num1);
    printf("\nResult: %lld", result);
    printf("\n\n");

    return 0;
}