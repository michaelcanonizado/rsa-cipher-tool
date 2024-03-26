#include <stdio.h>
#include "../bignum.h"

// Function to calculate modulo of an array of digits
int arrayModulo(int arr[], int size, int divisor) {
    int remainder = 0;

    // Iterate through the array from left to right
    for (int i = 0; i < size; i++) {
        
        // Calculate the current result including the next digit
        int currentResult = remainder * 10 + arr[i];
        printf("\nr: %d * %d + arr_val: %d = cur_r: %d", remainder, 10, arr[i], currentResult);
        // Update the remainder for the next iteration
        remainder = currentResult % divisor;
        printf("\ncur_r: %d / divisor: %d = upd_r: %d\n\n", currentResult, divisor, remainder);
    }

    // The final remainder is the result of the modulo operation
    return remainder;
}

int main() {
    // Example: Number 256 represented as [2, 5, 6]
    int numberArray[] = {6,0,4,3};
    int divisor = 56;
    // int numberArray[] = {6,0,4,3,3,0,5,2,8,8,2,7,6,1,1,8,4,3};
    int arraySize = 4;

    printf("\n");
    for(int i = 0; i < sizeof(numberArray)/sizeof(numberArray[0]); i++) {
        printf("%d", numberArray[i]);
    }
    printf(" modulo %d is:\n", divisor);

    // Modulo operation with divisor 10
    int moduloResult = arrayModulo(numberArray, arraySize, divisor);

    // Display the result
    printf("Modulo result: %d\n", moduloResult);

    return 0;
}