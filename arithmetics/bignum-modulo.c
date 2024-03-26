#include <stdio.h>
#include "../bignum.h"

// Function to calculate modulo of an array of digits
int arrayModulo(int arr[], int size, int divisor) {
    int remainder = 0;

    // Iterate through the array from left to right
    for (int i = 0; i < size; i++) {
        
        // Calculate the current result including the next digit
        int currentResult = remainder * 10 + arr[i];
        printf("\nr: %d * %d + curr_val: %d = cur_r: %d", remainder, 10, arr[i], currentResult);
        // Update the remainder for the next iteration
        remainder = currentResult % divisor;
        printf("\ncur_r: %d %% %d = upd_r: %d\n\n", currentResult, divisor, remainder);
    }

    // The final remainder is the result of the modulo operation
    return remainder;
}

int bignumShiftLeft(Bignum *result, Bignum *num, unsigned long long int shiftPlaces) {
    // Function that shifts a Bigum with the ampunt of 0s specified (x * pow(10, n)).
    // E.g: Integer: 123 -> 12300
    // E.g: Bignum: [3,2,1] -> [0,0,3,2,1]
    
    if (shiftPlaces < 0) {
        printf("Shifting Bignum by negative value/s.\n");
        return -1;
    }

    unsigned long long int resultLength = shiftPlaces;

    // Set the LSD (least significant digit) of result.digit to the amount of 0s specified.
    // REFACTOR: THIS EXPRESSION CAN BE REMOVED AS initBignum() SETS ALL ELEMENTS OF Bignum.digits[] TO 0. IF THIS REFACTOR FAVTOR IS BEING APPLIED, DONT FORGET TO RETEST multiplyBignum()
    memset(result->digits, 0, sizeof(int) * shiftPlaces);

    // Copy the rest of num.digits to result.digits
    for (unsigned long int i = shiftPlaces, j = 0; j < num->length; i++, j++) {
        result->digits[i] = num->digits[j];
        
        resultLength++;
    }

    result->length = resultLength;
}

int bignumModulo(Bignum *num, int divisor) {
    int remainder = 0;

    // Iterate through the array from left to right
    for (int i = num->length - 1; i >= 0; i--) {
        
        // Calculate the current result including the next digit
        int currentResult = remainder * 10 + num->digits[i];
        printf("\nr: %d * %d + curr_val: %d = cur_r: %d", remainder, 10, num->digits[i], currentResult);
        // Update the remainder for the next iteration
        remainder = currentResult % divisor;
        printf("\ncur_r: %d %% %d = upd_r: %d\n\n", currentResult, divisor, remainder);
    }

    // The final remainder is the result of the modulo operation
    return remainder;
}

int main() {
    // Example: Number 256 represented as [2, 5, 6]
    int numberArray[] = {6,0,4,3,6,8,9,1};
    int divisor = 56;
    // int numberArray[] = {6,0,4,3,3,0,5,2,8,8,2,7,6,1,1,8,4,3};
    int arraySize = 4;

    Bignum bignumDividend = initBignum();
    Bignum bignumDivisor = initBignum();
    Bignum bignumResult = initBignum();

    setBignum(&bignumDividend, "60436891", positive);

    printf("\n");
    for(int i = 0; i < sizeof(numberArray)/sizeof(numberArray[0]); i++) {
        printf("%d", numberArray[i]);
    }
    printf(" modulo %d is:\n", divisor);

    // int moduloResult = arrayModulo(numberArray, arraySize, divisor);
    int bignumModuloResult = bignumModulo(&bignumDividend, divisor);

    // Display the result
    // printf("Modulo result: %d\n", moduloResult);
    printf("Bignum Modulo result: %d\n", bignumModuloResult);

    return 0;
}