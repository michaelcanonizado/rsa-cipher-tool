#include <stdio.h>

long long int karatsuba(int multiplicand, int multiplier) {
    return multiplicand * multiplier;
}

int main(void) {
    int num1 = 1234;
    int num2 = 5678;

    int result = karatsuba(num1, num2);

    printf("%d * %d = %d\n", num1, num2, result);

    return 0;
}