#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum.h"
#include <time.h>

int main() {
    clock_t start = clock();

    Bignum num1 = initBignum();
    Bignum num2 = initBignum(); 
    Bignum num3 = initBignum();
    Bignum num4 = initBignum(); 
    Bignum res1 = initBignum();
    Bignum res2 = initBignum();

    setBignum(&num1, "123", negative);
    setBignum(&num2, "123", negative);
    setBignum(&num3, "123", negative);
    setBignum(&num4, "987", negative);


    divideBignum(&res1, &num1, &num2);
    divideBignum(&res2, &num3, &num4);

    printf("\n\nn1: %d | n2:  %d | n3:  %d | n4: %d\n\n", num1.sign, num2.sign, num3.sign, num4.sign);
    
    printf("\n");
    printBignum(&num1);
    printf(" / ");
    printBignum(&num2);
    printf(" = ");
    printBignum(&res1);
    printf("\n");
    printf("\n");
    printBignum(&num3);
    printf(" / ");
    printBignum(&num4);
    printf(" = ");
    printBignum(&res2);
    printf("\n");

   

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU time used: %f seconds\n\n", cpu_time_used);

    return 0;
}
