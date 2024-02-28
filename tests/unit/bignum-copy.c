#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../bignum.h"

int main(void) {
    Bignum num = initBignum();
    Bignum result = initBignum();

    setBignum(&num, "12345", positive);
    copyBignum(&result, &num);

    printBignum(&num);
    printf(" sign: %d \n", num.sign);
    printBignum(&result);
    printf(" sign: %d \n", result.sign);

    return 0;
}