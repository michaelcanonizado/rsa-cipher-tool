#include <stdio.h>
#include "bignum.h"

int main() {
    Bignum num1 = initBignum();
    
    intToBignum(&num1, 0123, positive);

    printBignum(&num1);

    int num2 = 0123;

    printf("\nInteger: %d", num2);

    printf("\n");

    return 0;
}
