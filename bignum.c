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

int main(void) {
    Bignum num1;

    initBignum(&num1, "9082209981077575318055135246687515387751187992165488349314423857642777553818111643626003588319126398");

    printf("\nNum Length: %d\n\n", num1.length);

    for (int i = 0; i < num1.length; i++) {
        printf("%d", num1.digits[i]);
    }

    return 0;
}