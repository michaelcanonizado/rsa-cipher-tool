#include <stdio.h>
#include <math.h>
#include <string.h>
#include "bignum.h"

// BIGNUM FUNCTIONS NEEDED:
// 1. (N/A) - Bignum to Integer
// 2. (N/A) - Cut Bignum in half to get a,b,c,d (left and right parts of the multiplicand and multiplier)
// 3. (Done) - Bignum Addition
// 4. (Done) - Bignum Subtraction
// 5. (Done) - Shift Bignum with 0s | [2,1] -> [0,0,2,1]

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
    printf("\n------------------------------");
    printf("\n\nK2 for x: %ld and y: %ld\n", x, y);

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

    printf("\nn/2: %ld", half);



    // Split the integers
    printf("\nGetting a,b,c,d...\n\n", half);
    long a = floor(x / multiplier);
    long b = x % multiplier;
    long c = floor(y / multiplier);
    long d = y % multiplier;

    printf("\na: %ld | b: %ld | c: %ld | d: %ld", a,b,c,d);
    printf("\n-------------------------------\n");



    // Recursive calls
    printf("\nGetting ac and bd...\n\n");
    long ac = karatsuba2(a,c);
    long bd = karatsuba2(b,d);

    printf("\nac: %ld", ac);
    printf("\nbd: %ld", bd);


    printf("\nGetting ad+bc...\n\n");
    int a_plus_b = a + b;
    int c_plus_d = c + d;
    long a_plus_b_times_c_plus_d = karatsuba2(a_plus_b,c_plus_d);
    long ad_plus_bc = a_plus_b_times_c_plus_d-ac-bd;

    printf("\na+b: %ld", a_plus_b);
    printf("\nc+d: %ld", c_plus_d);
    printf("\n\na+b * c+d: : %ld", a_plus_b_times_c_plus_d);
    printf("\nad+bc: %ld", ad_plus_bc);

    // Collect results
    long result = (ac * custom_pow(10, 2 * half)) + (ad_plus_bc * multiplier) + bd;
    
    printf("\nShifting ac and ad+bc...\n\n");
    printf("\n\nac shift left: %ld", ac * custom_pow(10, 2 * half));
    printf("\nad+bc shift left: %ld", ad_plus_bc * multiplier);
    printf("\nbd: %ld", bd);
    printf("\nac shift left + ad+bc shift left: %ld", (ac * custom_pow(10, 2 * half)) + (ad_plus_bc * multiplier));
    printf("\nac shift left + ad+bc shift left + bd: %ld", (ac * custom_pow(10, 2 * half)) + (ad_plus_bc * multiplier) + bd);
    printf("\nresult: %ld", result);

    return result;
}

long karatsuba2Compressed(long x, long y) {
    if (x < 10 || y < 10) {
        return x * y;
    }

    int n = fmax(get_size(x), get_size(y));
    long half = floor((double)n / 2.0);
    long multiplier = custom_pow(10, half);

    long a = floor(x / multiplier);
    long b = x % multiplier;
    long c = floor(y / multiplier);
    long d = y % multiplier;

    long ac = karatsuba2Compressed(a,c);
    long bd = karatsuba2Compressed(b,d);

    int a_plus_b = a + b;
    int c_plus_d = c + d;
    long a_plus_b_times_c_plus_d = karatsuba2Compressed(a_plus_b,c_plus_d);
    long ad_plus_bc = a_plus_b_times_c_plus_d-ac-bd;

    long result = (ac * custom_pow(10, 2 * half)) + (ad_plus_bc * multiplier) + bd;

    return result;
}

int karatsubaBignumShiftLeft(Bignum *result, Bignum *num, unsigned long long int shiftPlaces) {
    if (shiftPlaces < 0) {
        printf("Shifting Bignum by negative value/s.\n");
        return -1;
    }

    unsigned long long int resultLength = shiftPlaces;

    memset(result->digits, 0, sizeof(int) * shiftPlaces);

    for (unsigned long int i = shiftPlaces, j = 0; j < num->length; i++, j++) {
        result->digits[i] = num->digits[j];
        
        resultLength++;
    }

    result->length = resultLength;
}

int karatsubaBignumGetLeftHalf(Bignum *result, Bignum *num, unsigned long long int splitIndex) {
    unsigned long long int resultLength = 0;

    for (unsigned long int i = splitIndex, j = 0; i < num->length; i++, j++) {
        result->digits[j] = num->digits[i];
        resultLength++;
    }

    result->length = resultLength;
}

int karatsubaBignumGetRightHalf(Bignum *result, Bignum *num, unsigned long long int splitIndex) {
    if (splitIndex < 0) {
        printf("Shifting Bignum by negative value.\n");
        return -2;
    }

    if (num->length < splitIndex) {
        printf("Bignum length: %llu | shifting by: %llu\n", num->length, splitIndex);
        printf("Shifting Bignum by place that will go out of bounds.\n");
        return -1;
    }

    for (unsigned long int i = 0; i < splitIndex; i++) {
        result->digits[i] = num->digits[i];
    }

    result->length = splitIndex;
}

int karatsuba3(Bignum *result, Bignum *x, Bignum *y) {
    printf("\n------------------------------");
    printf("\n\nK3 for x: ");
    printBignum(x);
    printf(" and y: ");
    printBignum(y);
    printf("\n");

    // Base case
    if (x->length == 1 || y->length == 1) {
        long long int xInt = bignumToInt(x);
        long long int yInt = bignumToInt(y);
        intToBignum(result, xInt * yInt, positive);
        return 0;
    }

    // int n = fmax(get_size(x), get_size(y));
    unsigned long long int n = fmax(x->length, y->length);
    unsigned long long int half = floor(n / 2.0);

    printf("\nn/2: %llu", half);



    Bignum a = initBignum();
    Bignum b = initBignum();
    Bignum c = initBignum();
    Bignum d = initBignum();

    Bignum ac = initBignum();
    Bignum bd = initBignum();

    Bignum a_plus_b = initBignum();
    Bignum c_plus_d = initBignum();
    Bignum ac_minus_bd = initBignum();
    Bignum a_plus_b_times_c_plus_d = initBignum();
    Bignum a_plus_b_times_c_plus_d_minus_ac = initBignum();
    Bignum ad_plus_bc = initBignum();

    Bignum ac_left_shift = initBignum();
    Bignum ad_plus_bc_left_shift = initBignum();
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift = initBignum();
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd = initBignum();
    Bignum zero = initBignum();
    setBignum(&zero, "0", positive);

    printf("\nGetting a,b,c,d...\n\n");
    karatsubaBignumGetLeftHalf(&a, x, half);
    karatsubaBignumGetRightHalf(&b, x, half);
    karatsubaBignumGetLeftHalf(&c, y, half);
    karatsubaBignumGetRightHalf(&d, y, half);
    
    printf("\na: ");
    printBignum(&a);
    printf(" b: ");
    printBignum(&b);
    printf(" c: ");
    printBignum(&c);
    printf(" d: ");
    printBignum(&d);


    printf("\nGetting ac and bd...\n\n");
    karatsuba3(&ac, &a, &c);
    karatsuba3(&bd, &b, &d);

    printBignum(&d);
    printf("\n\nac: ");
    printBignum(&ac);
    printf("\nbd: ");
    printBignum(&bd);



    printf("\nGetting ad+bc...\n\n");
    addBignum(&a_plus_b, &a, &b);
    addBignum(&c_plus_d, &c, &d);
    karatsuba3(&a_plus_b_times_c_plus_d, &a_plus_b, &c_plus_d);
    subtractBignum(&a_plus_b_times_c_plus_d_minus_ac, &a_plus_b_times_c_plus_d, &ac);
    subtractBignum(&ad_plus_bc, &a_plus_b_times_c_plus_d_minus_ac, &bd);
    // long ad_plus_bc = karatsuba2(a+b,c+d)-ac-bd;

    printf("\na+b: ");
    printBignum(&a_plus_b);
    printf("\nc+d: ");
    printBignum(&c_plus_d);
    printf("\na+b * c+d: ");
    printBignum(&a_plus_b_times_c_plus_d);
    printf("\nad+bc: ");
    printBignum(&ad_plus_bc);



    printf("\nShifting ac and ad+bc...\n\n");
    karatsubaBignumShiftLeft(&ac_left_shift, &ac, half * 2);
    karatsubaBignumShiftLeft(&ad_plus_bc_left_shift, &ad_plus_bc, half);

    printf("\n\nac left shift: ");
    printBignum(&ac_left_shift);
    printf("\nad+bc left shift: ");
    printBignum(&ad_plus_bc_left_shift);



    // long result = (ac * custom_pow(10, 2 * half)) + (ad_plus_bc * multiplier) + bd;
    printf("\nCompilig results...\n\n");
    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift, &ac_left_shift, &ad_plus_bc_left_shift);
    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &ac_left_shift_plus_ad_plus_bc_left_shift, &bd);
    addBignum(result, &ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &zero);

    if ((x->sign == positive && y->sign == positive) || x->sign == negative && y->sign == negative) {
        result->sign = positive;
    } else {
        result->sign = negative;
    }

    printf("\nbd: ");
    printBignum(&bd);
    printf("\nac shift left + ad+bc shift left:  ");
    printBignum(&ac_left_shift_plus_ad_plus_bc_left_shift);
    printf("\nac shift left + ad+bc shift left + bd: ");
    printBignum(&ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd);
    printf("\nresult: ");
    printBignum(result);



    // long a = floor(x / multiplier);
    // long b = x % multiplier;
    // long c = floor(y / multiplier);
    // long d = y % multiplier;
    
    // printf("\n-------------------------------\n");

    // long ac = karatsuba2(a,c);
    // long bd = karatsuba2(b,d);
    // long ad_plus_bc = karatsuba2(a+b,c+d)-ac-bd;

    // printf("\nac: %ld", ac);
    // printf("\nbd: %ld", bd);
    // printf("\nad+bc: %ld", ad_plus_bc);

    // return result;
}

int karatsuba3Compressed(Bignum *result, Bignum *x, Bignum *y) {
    if (x->length == 1 || y->length == 1) {
        result->sign = negative;
        long long int xInt = bignumToInt(x);
        long long int yInt = bignumToInt(y);
        intToBignum(result, xInt * yInt, positive);
        return 0;
    }

    unsigned long long int n = fmax(x->length, y->length);
    unsigned long long int half = floor(n / 2.0);

    Bignum a = initBignum();
    Bignum b = initBignum();
    Bignum c = initBignum();
    Bignum d = initBignum();

    Bignum ac = initBignum();
    Bignum bd = initBignum();

    Bignum a_plus_b = initBignum();
    Bignum c_plus_d = initBignum();
    Bignum ac_minus_bd = initBignum();
    Bignum a_plus_b_times_c_plus_d = initBignum();
    Bignum a_plus_b_times_c_plus_d_minus_ac = initBignum();
    Bignum ad_plus_bc = initBignum();

    Bignum ac_left_shift = initBignum();
    Bignum ad_plus_bc_left_shift = initBignum();
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift = initBignum();
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd = initBignum();
    Bignum zero = initBignum();
    setBignum(&zero, "0", positive);

    karatsubaBignumGetLeftHalf(&a, x, half);
    karatsubaBignumGetRightHalf(&b, x, half);
    karatsubaBignumGetLeftHalf(&c, y, half);
    karatsubaBignumGetRightHalf(&d, y, half);
    
    karatsuba3Compressed(&ac, &a, &c);
    karatsuba3Compressed(&bd, &b, &d);

    addBignum(&a_plus_b, &a, &b);
    addBignum(&c_plus_d, &c, &d);
    karatsuba3Compressed(&a_plus_b_times_c_plus_d, &a_plus_b, &c_plus_d);
    subtractBignum(&a_plus_b_times_c_plus_d_minus_ac, &a_plus_b_times_c_plus_d, &ac);
    subtractBignum(&ad_plus_bc, &a_plus_b_times_c_plus_d_minus_ac, &bd);

    karatsubaBignumShiftLeft(&ac_left_shift, &ac, half * 2);
    karatsubaBignumShiftLeft(&ad_plus_bc_left_shift, &ad_plus_bc, half);

    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift, &ac_left_shift, &ad_plus_bc_left_shift);
    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &ac_left_shift_plus_ad_plus_bc_left_shift, &bd);
    addBignum(result, &ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &zero);
    
    if ((x->sign == positive && y->sign == positive) || x->sign == negative && y->sign == negative) {
        result->sign = positive;
    } else {
        result->sign = negative;
    }
}

int main(){
    long long int x = 123;
    long long int y = 123;

    Bignum num1 = initBignum();
    Bignum num2 = initBignum();
    Bignum result = initBignum();

    intToBignum(&num1, x, positive);
    intToBignum(&num2, y, positive);

    // setBignum(&num1, "70083693508915213745197637360864101925485320672729", positive);
    // setBignum(&num2, "54731509709293787933100356408829236967995835556926", positive);

    printf("\nMultiplying ");
    printBignum(&num1);
    printf(" and ");
    printBignum(&num2);

    multiplyBignum(&result, &num1, &num2);

    long resKA2 = karatsuba2Compressed(x, y);

    printf("\n\n%-15s %llu * %llu = %llu","Native Result:", x, y, x * y);
    printf("\n%-15s %llu * %llu = %llu","KA2 Result:", x, y, resKA2);
    printf("\n%-15s ","KA3 Result:");
    printBignum(&num1);
    printf(" * ");
    printBignum(&num2);
    printf(" = ");
    printBignum(&result);

    printf("\nKA3 Result Sign: %d", result.sign);

    printf("\n\n\n");
    return 0;
}