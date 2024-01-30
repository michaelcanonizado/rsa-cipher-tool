#include <stdio.h>

unsigned long long int gcd(unsigned long long int  num1, unsigned long long int num2) {
    // Uses the Euclidean Algorithm to get the GCD of 2 numbers quickly (ref: https://www.youtube.com/watch?v=yHwneN6zJmU&t=695s)
    unsigned long long int quotient, remainder;

    unsigned int count = 0;

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

        count++;
    }

    printf("\nFinding GCD took #%u rounds.", count);

    return a;
}

unsigned long long int modInverse(unsigned long long int  num, unsigned long long int divisor) {
    // Uses the Extended Euclidean Algorithm to get the mod inverse of 2 numbers quickly (ref: https://www.youtube.com/watch?v=lq285DDdmtw)
    unsigned long long int quotient, remainder;
    long long int t;
    long long int t1 = 0;
    long long int t2 = 1;

    unsigned int count = 0;

    // Find bigger number between the two numbers
    unsigned long long int a = num > divisor ? num : divisor;
    unsigned long long int b = num < divisor ? num : divisor;

    // Repeat until b becomes zero, then return a
    while(b > 0) {
        // Get the remainder, quotient, and t
        remainder = a % b;
        quotient = a / b;
        t = t1 - (t2 * quotient);

        // Shift numbers
        a = b;
        b = remainder;
        t1 = t2;
        t2 = t;

        count++;
    }

    printf("\nFinding Mod Inverse took #%u rounds.", count);

    // Check if t1 is the negative mod inverse. If true, return the positive inverse by offsetting by the divisor
    if (t1 < 0) {
        return t1 + divisor;
    } else {
        return t1;
    }
}

int main (void) {
    unsigned long long int num1 = 24481;
    unsigned long long int divisor = 30783412;

    unsigned long long int gcdResult = gcd(num1,divisor);
    unsigned long long int modInverseResult = modInverse(num1,divisor);

    printf("\n\nGCD of %llu and %llu is %llu", num1, divisor, gcdResult);

    printf("\nMod Inverse of %llu mod %llu is %lld\n\n", num1, divisor, modInverseResult);

    return 0;
}