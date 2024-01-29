#include <stdio.h>
#include <math.h>

unsigned long long decimalToBinary(unsigned long long num);

int main(void) {
    // Example of tried keys and n:
    // e (Public Key) : 3001
    // d (Private Key) : 841
    // n (P * Q) : 421999
    // Encrypt ex. : H (72) -> 351180

    // long long exponent = 13;
    unsigned long long int exponent = 300121;
    long long int base = 72;
    long long int divisor = 421999;

    unsigned long long res = decimalToBinary(exponent);

    printf("\n%llu -> %llu\n\n", exponent, res);

    return 0;
}

unsigned long long decimalToBinary(unsigned long long num) {
    int remainder;
    unsigned long long result = 0;
    unsigned long long i = 1;

    while (num != 0) {
        remainder = num % 2;
        num /= 2;
        result += remainder * i;
        i *= 10;
    }

    return result;
}