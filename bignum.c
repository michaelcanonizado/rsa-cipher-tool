#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INT_LENGTH 10000

typedef struct {
    int digits[MAX_INT_LENGTH];
    int length;
} Bignum;

void initBignum(Bignum *numStruct, char numStr[]) {
    numStruct->length = strlen(numStr);

    for (int i = 0; i < numStruct->length; i++) {
        numStruct->digits[i] = numStr[i] - '0';
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

    initBignum(&num1, "10");
    initBignum(&num2, "2");

    printf("\nNum Length: %d\n", num1.length);

    for (int i = 0; i < num1.length; i++) {
        printf("%d", num1.digits[i]);
    }

    addBignum(&result, &num1, &num2);

    return 0;
}