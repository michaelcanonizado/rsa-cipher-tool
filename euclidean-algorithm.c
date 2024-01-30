#include <stdio.h>

unsigned long long int gcd(unsigned long long int  num1, unsigned long long int num2) {
    unsigned long long int quotient, remainder;

    unsigned long long int a = num1 > num2 ? num1 : num2;
    unsigned long long int b = num1 < num2 ? num1 : num2;

    while(b > 0) {
        remainder = a % b;
        quotient = a / b;

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