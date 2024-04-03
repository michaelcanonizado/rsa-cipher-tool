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

    printf("\n\n\n\n FOR LOOP ----->\n");

    for (int i = 0; i < result->length; i++) {
        resetBignum(&product);
        Bignum currDigit = initBignum();
        Bignum currResultDigitTimesBase = initBignum();

        intToBignum(&currDigit, result->digits[i], positive);
        multiplyBignum(&currResultDigitTimesBase, &currDigit, base);
        // printf("\nprdct before: ");
        // printBignum(&product);
        addBignum(&product, &currResultDigitTimesBase, &carry);     

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
        int productModTenInt = bignumToInt(&productModTen);
        result->digits[i] = productModTenInt;
        printf("\n%d", result->digits[i]);
        printf(" = ");
        printBignum(&product);
        printf(" %% %d", 10);

        resetBignum(&carry);
        divideBignum(&carry, &product, &ten);
        printf("\n");
        printBignum(&carry);
        printf(" = ");
        printBignum(&product);
        printf(" / %d\n----------------------------------\n", 10);
    }

    printf("\n\n WHILE LOOP ----->\n");
    while(!isBignumZero(&carry)) {
        Bignum carryModTen = initBignum();
        Bignum carryDivideTen = initBignum();
    
        moduloBignum(&carryModTen, &carry, &ten);
        result->digits[result->length] = bignumToInt(&carryModTen);

        printf("\n\n");
        printBignum(&carryModTen);
        printf(" = ");
        printBignum(&carry);
        printf(" %% %d", 10);

        divideBignum(&carryDivideTen, &carry, &ten);
        copyBignum(&carry, &carryDivideTen);

        printf("\n");
        printBignum(&carryDivideTen);
        printf(" = ");
        printBignum(&carry);
        printf(" / %d", 10);

        result->length++;
        printf("\nsize++ = %d", base->length);
        printf("\ninsertedItems++ = %d", base->length);
    }
}


int multiplyBignumArrayCompressed(Bignum *result, Bignum *base) {
    Bignum ten  = initBignum();
    setBignum(&ten, "10", positive);

    Bignum carry = initBignum();
    setBignum(&carry, "0", positive);
    Bignum product = initBignum();

    for (int i = 0; i < result->length; i++) {
        product = initBignum();
        Bignum currDigit = initBignum();
        Bignum currResultDigitTimesBase = initBignum();

        intToBignum(&currDigit, result->digits[i], positive);
        multiplyBignum(&currResultDigitTimesBase, &currDigit, base);
        addBignum(&product, &currResultDigitTimesBase, &carry);

        Bignum productModTen = initBignum();
        moduloBignum(&productModTen, &product, &ten);
        int productModTenInt = bignumToInt(&productModTen);
        result->digits[i] = productModTenInt;

        carry = initBignum();
        divideBignum(&carry, &product, &ten);
    }

    while(!isBignumZero(&carry)) {
        Bignum carryModTen = initBignum();
        Bignum carryDivideTen = initBignum();
    
        moduloBignum(&carryModTen, &carry, &ten);
        result->digits[result->length] = bignumToInt(&carryModTen);

        divideBignum(&carryDivideTen, &carry, &ten);
        copyBignum(&carry, &carryDivideTen);

        result->length++;
    }
}

int main(void) {
    // Start CPU timer
    clock_t begin = clock();

    Bignum two = initBignum();
    setBignum(&two, "2", positive);

    Bignum x = initBignum();
    Bignum n = initBignum();
    Bignum result = initBignum();

    setBignum(&x, "72", positive);
    setBignum(&n, "100193", positive);

    printf("\n\nMultiplying ");
    printBignum(&n);
    printf(" digits...");

    copyBignum(&result, &x);
    Bignum i = initBignum();
    for(setBignum(&i, "2", positive); isLessThanOrEqualToBignum(&i, &n); incrementBignum(&i, 1)) {
        printf("\n");
        printBignum(&i);
        printf(" iteration...");
        multiplyBignumArrayCompressed(&result, &x);
    }

    // copyBignum(&result, &x);
    // Bignum i = initBignum();
    // for(setBignum(&i, "2", positive); isLessThanOrEqualToBignum(&i, &n); incrementBignum(&i, 1)) {
    //     printf("\n\n--------------------------------------------\n--------------------------------------------\n\n");
    //     printBignum(&i);
    //     printf(" interation: \n");

    //     printf("\n\ncurrent result: ");
    //     printBignum(&result);
    //     printf("\nNum of digits in result: %d", result.length);

    //     multiplyBignumArray(&result, &x);
    // }

    printf("\n\nBase: ");
    printBignum(&x);
    printf("\nExponent: ");
    printBignum(&n);
    printf("\nResult: ");
    printBignum(&result);
    printf("\nResult length: %llu", result.length);

    // End CPU timer and print elapsed time
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nELAPSED TIME CALCULATING: %lf seconds\n\n", time_spent);

    return 0;
}

