#include <stdio.h>

unsigned long long int gcd(unsigned long long int  num1, unsigned long long int num2) {
    // Uses the Euclidean Algorithm to get the GCD of 2 numbers quickly (ref: https://www.youtube.com/watch?v=yHwneN6zJmU&t=695s)
    unsigned long long int quotient, remainder;

    // Find bigger number between the two numbers
    unsigned long long int a = num1 > num2 ? num1 : num2;
    unsigned long long int b = num1 < num2 ? num1 : num2;

    // Repeat until b becomes zero, then return a
    while(b > 0) {
        // Get the remainder and quotient
        remainder = a % b;
        quotient = a / b;

        // Shift numbers
        a = b;
        b = remainder;
    }

    return a;
}

int main (void) {
    unsigned long long int num1 = 9811;
    unsigned long long int num2 = 31427928;

    unsigned long long int result = gcd(num1,num2);

    printf("\nGCD of %llu and %llu is %llu\n\n", num1, num2, result);

    return 0;
}