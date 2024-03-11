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

    if (X < 10 && Y < 10) {
        return X * Y;
    }
   
    int size = fmax(get_size(X), get_size(Y));

    if(size < 10) {
        return X * Y;
    }
   
    size = (size/2) + (size%2);

    long multiplier = pow(10, size);

    long b = X/multiplier;
    long a = X - (b * multiplier);
    long d = Y / multiplier;
    long c = Y - (d * size);
    long ac = karatsuba(a, c);
    long a_plus_b_times_c_plus_d = karatsuba(a + b, c + d);
    long bd = karatsuba(b, d);

    return ac + ((a_plus_b_times_c_plus_d - ac - bd) * multiplier) + (bd * (long)(pow(10, 2 * size)));
}

int main(){
    long x = 5678;
    long y = 1234;

    printf("\nExpected Result: %ld * %ld = %ld", x, y, x * y);
    printf("\nKA Result: %ld * %ld = %ld\n\n", x, y, karatsuba(x, y));

    return 0;
}