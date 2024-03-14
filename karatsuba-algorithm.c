#include <stdio.h>
#include <math.h>
#include <string.h>
#include "bignum.h"

// BIGNUM FUNCTIONS NEEDED:
// 1. (N/A) - Bignum to Integer
// 2. (N/A) - Cut Bignum in half to get a,b,c,d (left and right parts of the multiplicand and multiplier)
// 3. (Done) - Bignum Addition
// 4. (Done) - Bignum Subtraction
// 5. (N/A) - Shift Bignum with 0s | [2,1] -> [0,0,2,1]

int get_size(long value){
    // Count number of digits of number
    int count = 0;
    while (value > 0) {
        count++;
        value /= 10;
    }
    return count;
}

long custom_pow(long base, long exponent) {
    long result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

long karatsuba1(long X, long Y){
    // Base case 1
    if (X < 10 && Y < 10) {
        return X * Y;
    }
   
    // Get the maximum length of the two numbers
    int size = fmax(get_size(X), get_size(Y));

    // Base case 2
    if(size < 10) {
        return X * Y;
    }
   
    // Round up the maximum length
    size = (size/2) + (size%2);

    // Get the multiplier to be used for the current pair of numbers
    long multiplier = pow(10, size);

    // Get values needed for Karatsuba multiplication

    // REFACTOR: THIS ALGORITHM CURRENTLY DIVIDES THE NUMBER BY THE MULTIPLIER TO GET THE VALUE OF B AND D. THIS FUNCTION SHOULD NOT CONTAIN ANY DIVISION. FIND A WAY TO GET THE VALUES WITHOUT DIVIDING.

    long b = X/multiplier;
    long a = X - (b * multiplier);
    long d = Y / multiplier;
    long c = Y - (d * size);
    long a_plus_b_times_c_plus_d = karatsuba1(a + b, c + d);

    // Recursive call
    long ac = karatsuba1(a, c);
    long bd = karatsuba1(b, d);

    // Collect recursed results
    return ac + ((a_plus_b_times_c_plus_d - ac - bd) * multiplier) + (bd * (long)(pow(10, 2 * size)));
}

long karatsuba2(long x, long y) {
    // Base case
    if (x < 10 || y < 10) {
        return x * y;
    }

    // Get max length of the 2 integers
    int n = fmax(get_size(x), get_size(y));

    // Get n/2
    long half = floor((double)n / 2.0);

    // Get 10^(n/2)
    long multiplier = custom_pow(10, half);

    // Split the integers
    long a = floor(x / multiplier);
    long b = x % multiplier;
    long c = floor(y / multiplier);
    long d = y % multiplier;

    printf("\nn/2: %ld", half);
    printf("\nmultiplier: %ld", multiplier);
    printf("\na: %ld | b: %ld | c: %ld | d: %ld", a,b,c,d);
    printf("\n-------------------------------\n");

    // Recursive calls
    long ac = karatsuba2(a,c);
    long bd = karatsuba2(b,d);
    long ad_plus_bc = karatsuba2(a+b,c+d)-ac-bd;

    printf("\nac: %ld", ac);
    printf("\nbd: %ld", bd);
    printf("\nad+bc: %ld", ad_plus_bc);

    // Collect results
    long result = (ac * custom_pow(10, 2 * half)) + (ad_plus_bc * multiplier) + bd;

    return result;
}

int karatsubaBignumGetLeftHalf(Bignum *result, Bignum *num, unsigned long long int shiftPlaces) {
    if (shiftPlaces < 0) {
        printf("Shifting Bignum by negative value.\n");
        return -2;
    }

    if (num->length < shiftPlaces) {
        printf("Bignum length: %llu | shifting by: %llu\n", num->length, shiftPlaces);
        printf("Shifting Bignum by place that will go out of bounds.\n");
        return -1;
    }

    memset(result->digits, 0, sizeof(int) * shiftPlaces);

    for (unsigned long int i = shiftPlaces; i < num->length; i++) {
        result->digits[i] = num->digits[i];
        shiftPlaces++;
    }

    result->length = shiftPlaces;
}

int main(){
    long x = 456;
    long y = 123;

    Bignum num = initBignum();
    Bignum leftHalf = initBignum();

    setBignum(&num, "123456", positive);

    karatsubaBignumGetLeftHalf(&leftHalf, &num, 3);
    printf("\nBignum: ");
    printBignum(&leftHalf);
    printf("\n\n");

    long resKA1 = karatsuba1(x, y);
    long resKA2 = karatsuba2(x, y);

    printf("\n\nExpected Result: %ld * %ld = %ld", x, y, x * y);
    printf("\nKA1 Result: %ld * %ld = %ld", x, y, resKA1);
    printf("\nKA2 Result: %ld * %ld = %ld\n\n", x, y, resKA2);

    return 0;
}