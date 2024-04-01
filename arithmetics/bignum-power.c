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

int multiplyBignumArray(Bignum *result, Bignum *base) {
    Bignum ten  = initBignum();
    setBignum(&ten, "10", positive);

    Bignum carry = initBignum();
    setBignum(&carry, "0", positive);
    Bignum product = initBignum();

    // Multiply n with each digit of result[]
    // for (int i = 0; i < size; i++) {
    //     product = result[i] * x + carry;
    //     result[i] = product % 10;
    //     carry = product / 10;
    // }

    printf("\n\n ->");

    for (int i = 0; i < base->length; i++) {
        Bignum currDigit = initBignum();
        Bignum currBaseDigitTimesBase = initBignum();

        intToBignum(&currDigit, base->digits[i], positive);
        multiplyBignum(&currBaseDigitTimesBase, &currDigit, base);
        addBignum(&product, &currBaseDigitTimesBase, &carry);
        printf("\n");
        printBignum(&product);
        printf(" = ");
        printBignum(&currDigit);
        printf(" * ");
        printBignum(base);
        printf(" + ");
        printBignum(&carry);

        Bignum productModTen = initBignum();
        moduloBignum(&productModTen, &product, &ten);
        int temp = bignumToInt(&productModTen);
        result->digits[i] = temp;
        printf("\n");
        printBignum(&currDigit);
        printf(" = ");
        printBignum(&product);
        printf(" %% %d", 10);

        divideBignumPrototype(&carry, &product, &ten);
        printf("\n");
        printBignum(&carry);
        printf(" = ");
        printBignum(&product);
        printf(" / %d\n----------------------------------\n", 10);
    }

    printf("\n\n");

    // Count how many digits in array
    // while(carry) {
    //     result[size] = carry % 10;
    //     carry = carry / 10;
    //     size++;
    //     (*insertedItems)++;
    // }

    // unsigned long long int size = base->length;
    // unsigned long long int insertedItems = base->length;

    while(!isBignumZero(&carry)) {
        Bignum carryModTen = initBignum();
        Bignum carryDivideTen = initBignum();

        moduloBignum(&carryModTen, &carry, &ten);
        result->digits[base->length] = bignumToInt(&carryModTen);

        divideBignumPrototype(&carryDivideTen, &carry, &ten);
        copyBignum(&carry, &carryDivideTen);
        base->length++;
    }

    // return size;
}

int main(void) {
    // Start CPU timer
    clock_t begin = clock();

    Bignum two = initBignum();
    setBignum(&two, "2", positive);

    Bignum x = initBignum();
    Bignum n = initBignum();
    Bignum result = initBignum();

    setBignum(&x, "123", positive);
    setBignum(&n, "4", positive);

    Bignum i = initBignum();
    for(setBignum(&i, "2", positive); isLessThanBignum(&i, &n) || isEqualToBignum(&i, &n); incrementBignum(&i, 1)) {
        printf("\n\n");
        printBignum(&i);
        printf(" interation: \n");

        multiplyBignumArray(&result, &x);
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

