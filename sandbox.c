#include <stdio.h>
#include "bignum.h"

int karatsubaBignumGetRightHalf(Bignum *result, Bignum *num, unsigned long long int splitIndex) {
    if (splitIndex < 0) {
        printf("Shifting Bignum by negative value.\n");
        return -2;
    }

    if (num->length < splitIndex) {
        printf("Bignum length: %llu | shifting by: %llu\n", num->length, splitIndex);
        printf("Shifting Bignum by place that will go out of bounds.\n");
        return -1;
    }

    for (unsigned long int i = 0; i < splitIndex; i++) {
        result->digits[i] = num->digits[i];
    }

    result->length = splitIndex;
}

int main() {
    Bignum num1 = initBignum();
    Bignum num2 = initBignum();
    Bignum result = initBignum();

    setBignum(&num1, "123", positive);
    setBignum(&num2, "8", positive);

    karatsubaBignumGetRightHalf(&result, &num1, 1);

    printBignum(&result);

    printf("\n");

    return 0;
}
