#include <stdio.h>

int main(void) {

    int base = 2;
    int exponent = 4;
    int product = 1;

    int tempExpo = exponent;

    if (exponent < 0) {
        printf("Exponent is less than zero...");
        return 1;
    }

    if (exponent == 0) {
        printf("Exponent is zero. %d to the power of %d is: %d", base, exponent, base);
        return 0;
    }

    while(exponent != 0) {
        product = product * base;
        exponent--;
    }

    printf("%d to the power of %d is: %d", base,  tempExpo, product);

    return 0;
}