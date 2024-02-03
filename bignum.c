#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INT_LENGTH 10000

typedef struct {
    int digits[MAX_INT_LENGTH];
    int length;
} Bignum;

void initBignum(Bignum *numStruct, char numStr[]) {
    int temp;

    // Store number length
    numStruct->length = strlen(numStr);

    // Load numbers into Bignum.digits
    for (int i = 0; i < numStruct->length; i++) {
        numStruct->digits[i] = numStr[i] - '0';
    }

    // Reverse Bignum.digits for easier operations to be done
    for (int i = 0; i < numStruct->length / 2; i++) {
        temp = numStruct->digits[i];
        numStruct->digits[i] = numStruct->digits[numStruct->length - i - 1];
        numStruct->digits[numStruct->length - i - 1] = temp;
    }
}

void addBignum(Bignum *result, Bignum *num1, Bignum *num2) {
    int sum;
    int carry = 0;
    int resultLength = 0;
    unsigned long long int maxLength;

    if (num1->length > num2->length) {
        maxLength = num1->length;
    } else if (num2->length > num1->length) {
        maxLength = num2->length;
    } else {
        maxLength = num1->length;
    }

    for (int i = maxLength - 1; i >= 0; i--) {
        sum = carry;
        sum += num1->digits[i] + num2->digits[i];
        carry = sum >= 10 ? 1 : 0;
        sum = sum % 10;
        printf("\n#1: %d | #2: %d | sum %d | carry %d",num1->digits[i], num2->digits[i], sum, carry);
        result->digits[resultLength] = sum;

        resultLength++;
    }
    printf("\n\n");
    for (int i = 0; i < resultLength; i++) {
        printf("%d", result->digits[i]);
    }
}

int main(void) {
    Bignum num1, num2, result;

    initBignum(&num1, "123456789");

    printf("\nNum Length: %d\nNum (reverse): ", num1.length);

    for (int i = 0; i < num1.length; i++) {
        printf("%d", num1.digits[i]);
    }

    return 0;
}