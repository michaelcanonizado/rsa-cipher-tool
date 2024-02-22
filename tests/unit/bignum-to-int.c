#include <stdio.h>
#include "../../bignum.h"

int main(void) {
    Bignum num = initBignum();
    
    setBignum(&num, "9223372036854775807", negative);

    printf("\nsgn: %d | len: %d | num 1: ", num.sign, num.length);
    for (int i = num.length - 1; i >= 0 ; i--) {
        printf("%d", num.digits[i]);
    }
    
    long long int result = bignumToInt(&num);
    printf("\nResult: %lld", result);
    printf("\n\n");

    return 0;
}