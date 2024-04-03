#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../bignum.h"

int bignumToBinary(Bignum *result, Bignum *num) {

    int remainderTemp;
    unsigned long long int binaryLength = 0;

    Bignum numTemp = initBignum(); 
    copyBignum(&numTemp, num);

    Bignum remainder = initBignum(); 
    Bignum two = initBignum(); 
    intToBignum(&two, 2, positive);

    while(!isBignumZero(&numTemp)) {
        if (numTemp.digits[0] % 2 == 0) {
            result->digits[binaryLength++] = 0;
        } else {
            result->digits[binaryLength++] = 1;
        }

        halfBignum(&numTemp, &numTemp);
    }

    result->length = binaryLength;
    return 0;
}

int main() {
    clock_t start = clock();

    Bignum num = initBignum(); 
    Bignum res = initBignum();

    setBignum(&num, "1792274785367970752769521623194341971299269644227478536797075276936797075276952162319434", positive);

    bignumToBinary(&res, &num);
    
    printf("\n");
    printBignum(&num);
    printf("  ->  ");
    printBignum(&res);
    printf("\n\n Binary Length: %llu", res.length);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
