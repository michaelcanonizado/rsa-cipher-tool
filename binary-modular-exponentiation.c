#include <stdio.h>
#include <math.h>

unsigned long long decimalToBinary(unsigned long long num, int result[]);

int main(void) {
    // Example of tried keys and n:
    // e (Public Key) : 3001
    // d (Private Key) : 841
    // n (P * Q) : 421999
    // Encrypt ex. : H (72) -> 351180
    unsigned long long int exponent = 3001;
    unsigned long long int base = 72;
    unsigned long long int divisor = 421999;

    // Other examples. Correct results are given
    //
    // Ex. 1
    // unsigned long long int exponent = 2312397232412112312;
    // unsigned long long int base = 72;
    // unsigned long long int divisor = 421999;
    // RESULT: 159604
    //
    // Ex. 2
    // unsigned long long int exponent = 262;
    // unsigned long long int base = 240;
    // unsigned long long int divisor = 14;
    // RESULT: 2

    int binaryExponent[10000];

    unsigned long long binaryExponentLength = decimalToBinary(exponent, binaryExponent);

    // Print Exponent Decimal to Binary Result.
    printf("\n%llu in binary (%llu digits):\n", exponent, binaryExponentLength);
    for (int i = 0; i < binaryExponentLength; i++) {
        printf("%d", binaryExponent[i]);
    }

    // Start Calculating Powers
    unsigned long long int remainder = base;
    for (int i = 0; i < binaryExponentLength - 1; i++) {
        if (binaryExponent[i + 1] == 0) {
            remainder = (remainder * remainder) % divisor;
        } else {
            unsigned long long int tempSquare = (remainder * remainder) % divisor;
            remainder = (tempSquare * base) % divisor;
        }
    }

    printf("\n\nRESULT: %llu\n\n", remainder);

    return 0;
}

unsigned long long decimalToBinary(unsigned long long num, int binary[]) {
    int remainder;
    unsigned long long binaryLength = 0;

    // Get remainders (1 or 0) then push to array. Array will be in reverse order.
    while (num != 0) {
        remainder = num % 2;
        num /= 2;
        binary[binaryLength++] = remainder;
    }

    // Reverse array to get the right order.
    for (int i = 0; i < binaryLength / 2; i++) {
        int temp = binary[i];
        binary[i] = binary[binaryLength - 1 - i];
        binary[binaryLength - 1 - i] = temp;
    }

    // Return the number of digits of the binary.
    return binaryLength;
}