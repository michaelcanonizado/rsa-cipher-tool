#include <stdio.h>
#include <math.h>
#include "../../bignum.h"

int main(void) {
    printf("\n\n--------------------------------------------------------------------------------------");
    printf("\n| %27s            || %27s              |", "GENERATED BIGNUM", "BIGNUM TO INT");
    printf("\n--------------------------------------------------------------------------------------");
    printf("\n| %5s  | %6s | %17s    || %18s     | %6s | %5s  |", "SIGN", "LENGTH", "BIGNUM STRUCT", "LONG LONG INT", "LENGTH", "SIGN");
    printf("\n--------------------------------------------------------------------------------------");

    Bignum num = initBignum();
    
    intToBignum(&num, 92, negative);

    printf("\n| %5s  | %6d | ", num.sign == positive ? "POS" : "NEG", num.length);
    for (int i = num.length; i < 20; i++) {
        printf(" ");
    }
    for (int i = num.length - 1; i >= 0 ; i--) {
        printf("%d", num.digits[i]);
    }

    long long int result = bignumToInt(&num);
    printf(" ||   %20lld | %6d | %5s  |", result, getNumOfDigitsOfInteger(result), result > 0 ? "POS" : "NEG");


    printf("\n\n\n");

    return 0;
}