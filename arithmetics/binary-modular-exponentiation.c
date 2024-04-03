#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

unsigned long long decimalToBinary(unsigned long long num, int result[]);

int main(void) {
    clock_t start = clock();

    // Example of tried keys and n:
    // e (Public Key) : 3001
    // d (Private Key) : 841
    // n (P * Q) : 421999
    // Encrypt ex. : H (72) -> 351180
    // unsigned long long int exponent = 3001;
    // unsigned long long int base = 72;
    // unsigned long long int divisor = 421999;
    // unsigned long long int exponent = 123;
    // unsigned long long int base = 123;
    // unsigned long long int divisor = 45; // 7151692
    // Random Prime. P: 2417 | Q: 3121
    // N: 7543457
    // Phi of N: 7537920
    // e (Public Key): 673
    // GCD of e and Phi of N: 1
    // d (Private Key): 2284897
    // (d * e) mod Phi of N: 1

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

    unsigned long long int exponent = 23123972324121123;
    unsigned long long int base = 3123972324121;
    unsigned long long int divisor = 421999;

    int binaryExponent[10000];

    unsigned long long binaryExponentLength = decimalToBinary(exponent, binaryExponent);


    // Print Exponent Decimal to Binary Result.
    printf("\n%llu in binary (%llu digits):\n", exponent, binaryExponentLength);
    for (int i = 0; i < binaryExponentLength; i++) {
        printf("%d", binaryExponent[i]);
    }

    printf("\n");
    // Start Calculating Powers
    unsigned long long int remainder = base;
    for (int i = 0; i < binaryExponentLength - 1; i++) {
        if (binaryExponent[i + 1] == 0) {
            printf("\n 0.0 -> %llu = %llu^2 mod %llu", (remainder * remainder) % divisor, remainder, divisor);

            remainder = (remainder * remainder) % divisor;
        } else {
            printf("\n 1.0 -> %llu = %llu^2 mod %llu", (remainder * remainder) % divisor, remainder, divisor);

            printf("\n 1.1 -> %llu = %llu mod %llu", (((remainder * remainder) % divisor) * base) % divisor,(((remainder * remainder) % divisor) * base), divisor);

            unsigned long long int tempSquare = (remainder * remainder) % divisor;
            remainder = (tempSquare * base) % divisor;
        }
        printf("\n----------------------------");
    }

    printf("\n\nRESULT: %llu\n\n", remainder);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nCPU time used: %f seconds\n\n", cpu_time_used);

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