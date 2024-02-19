#include <stdio.h>

#define MAX_DIGITS 1000

void subtract(int result[], int minuend[], int subtrahend[], int num_digits) {
    int borrow = 0;

    for (int i = 0; i < num_digits; i++) {
        int diff = minuend[i] - subtrahend[i] - borrow;

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result[i] = diff;
    }

    // If there's a borrow left after subtraction, we have negative result
    if (borrow == 1) {
        printf("Error: Result is negative\n");
    }
}

int main() {
    int minuend[MAX_DIGITS] = {4, 3, 2, 1};
    int subtrahend[MAX_DIGITS] = {1, 2, 3, 4};
    int result[MAX_DIGITS];

    int num_digits = 4;

    subtract(result, minuend, subtrahend, num_digits);

    printf("Result of subtraction: ");
    for (int i = num_digits - 1; i >= 0; i--) {
        printf("%d", result[i]);
    }
    printf("\n");

    return 0;
}
