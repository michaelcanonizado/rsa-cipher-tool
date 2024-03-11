#include <stdio.h>
#include <math.h>

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

long karatsuba(long X, long Y){
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
    long a_plus_b_times_c_plus_d = karatsuba(a + b, c + d);

    // Recursive call
    long ac = karatsuba(a, c);
    long bd = karatsuba(b, d);

    // Collect recursed results
    return ac + ((a_plus_b_times_c_plus_d - ac - bd) * multiplier) + (bd * (long)(pow(10, 2 * size)));
}

int main(){
    long x = 5678;
    long y = 1234;

    printf("\nExpected Result: %ld * %ld = %ld", x, y, x * y);
    printf("\nKA Result: %ld * %ld = %ld\n\n", x, y, karatsuba(x, y));

    return 0;
}