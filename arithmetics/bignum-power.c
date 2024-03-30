#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../bignum.h"

int multiplyArrayItems(int x, int result[], int size, int *insertedItems) {
    int carry = 0;
    int product;

    // Multiply n with each digit of result[]
    for (int i = 0; i < size; i++) {
        product = result[i] * x + carry;
        result[i] = product % 10;
        carry = product / 10;
    }

    // Count how many digits in array
    while(carry) {
        result[size] = carry % 10;
        carry = carry / 10;
        size++;
        (*insertedItems)++;
    }

    return size;
}

int powerBignum(int x, int result[], int size, int *insertedItems) {
    int carry = 0;
    int product;

    // Multiply n with each digit of result[]
    for (int i = 0; i < size; i++) {
        product = result[i] * x + carry;
        result[i] = product % 10;
        carry = product / 10;
    }

    // Count how many digits in array
    while(carry) {
        result[size] = carry % 10;
        carry = carry / 10;
        size++;
        (*insertedItems)++;
    }

    return size;
}

int main(void) {
    // Start CPU timer
    clock_t begin = clock();

    Bignum x = initBignum();
    Bignum n = initBignum();
    Bignum result = initBignum();

    setBignum(&x, "12", positive);
    setBignum(&n, "000000", positive);

    Bignum i = initBignum();
    setBignum(&i, "0", positive);

    for(; isLessThanBignum(&i, &x); incrementBignum(&i)) {
        printBignum(&i);
        printf(" - ");
    }

    printf("\n\nBase: ");
    printBignum(&x);
    printf("\nExponent: ");
    printBignum(&n);
    printf("\nResult: ");
    printBignum(&result);

    // End CPU timer and print elapsed time
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nELAPSED TIME CALCULATING: %lf seconds\n\n", time_spent);

    return 0;
}

