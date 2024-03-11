#include <stdio.h>
#include <math.h>

int get_size(long value){
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