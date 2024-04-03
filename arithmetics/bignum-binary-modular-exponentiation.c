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

    Bignum base = initBignum(); 
    Bignum exponent = initBignum(); 
    Bignum divisor = initBignum(); 
    Bignum result = initBignum();

    setBignum(&base, "7151692", positive);
    setBignum(&exponent, "76890119807477", positive);
    setBignum(&divisor, "7543457", positive);

    bignumToBinary(&result, &exponent);
    
    printf("\n");
    printBignum(&exponent);
    printf("  ->  ");
    printBignum(&result);
    printf("\n\n Binary Length: %llu", result.length);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
