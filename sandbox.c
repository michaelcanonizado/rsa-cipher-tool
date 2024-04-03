#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"

int main() {
    clock_t start = clock();

    Bignum num1 = initBignum();
    Bignum num2 = initBignum(); 
    Bignum num3 = initBignum();
    Bignum num4 = initBignum(); 

    Bignum res1 = initBignum();
    Bignum res2 = initBignum();

    setBignum(&num1, "60260995677123112", positive);
    setBignum(&num2, "2", positive);

    divideBignum(&res1, &num1, &num2);
    
    printf("\n");
    printBignum(&num1);
    printf(" %% ");
    printBignum(&num2);
    printf(" = ");
    for (int i = res1.length; i >= 0; i--) {
            printf("%d", res1.digits[i]);
        }
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
