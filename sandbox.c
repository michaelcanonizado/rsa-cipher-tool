#include <stdio.h>
#include "bignum.h"

int main() {
    int num = -1230;

    int res = getLengthOfInteger(num);

    printf("length of num: %d\n", res);

    printf("\n");

    return 0;
}
