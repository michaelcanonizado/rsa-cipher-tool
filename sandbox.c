#include <stdio.h>
#include "bignum.h"

int main() {
    Bignum num1 = initBignum();
    Bignum num2 = initBignum();
    Bignum result = initBignum();

    setBignum(&num1, "1234", positive);
    setBignum(&num2, "8", positive);

    addBignum(&result, &num1, &num2);

    printBignum(&result);

    printf("\n");

    return 0;
}
