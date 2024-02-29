#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../../bignum.h"

int main(void) {
    Bignum num = initBignum();

    setBignum(&num, "46779607198166158226869682071981661598166158226869682071989816615822686968207198166159816", positive);

    bignumToInt(&num);

    return 0;
}