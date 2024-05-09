#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bignum.h"


int main() {
    printf("\n\n");




    Bignum x;
    initBignum(&x);

    generatePrimeBignum(&x, 20);

    printf("x prime = ");
    printBignum(&x);



    






    printf("\n\n\n\n\n");
    return 0;
}
