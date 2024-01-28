#include <stdio.h>

int multiplyArrayItems(int x, int result[], int size) {
    int carry = 0;
    int product;
    // printf("\nx: %d size: %d", x, size);
    // Multiply n with each digit of result[]
    for (int i = 0; i < size; i++) {
        product = result[i] * x + carry;
        result[i] = product % 10;
        carry = product / 10;
    }

    // Count how many digits in array
    while(carry) {
        result[size] = carry % 10;
        carry = carry / 10;
        size++;
    }

    return size;
}

// Function to calculate modulo of an array of digits (FOR LOOP WILL START FROM THE END OF THE ARRAY AS THE ARRAY IS IN REVERSE)
int arrayModulo(int arr[], int size, int divisor) {
    int remainder = 0;

    // Iterate through the array from left to right
    for (int i = size-1; i >= 0; i--) {
        
        // Calculate the current result including the next digit
        int currentResult = remainder * 10 + arr[i];
        // Update the remainder for the next iteration
        remainder = currentResult % divisor;
    }

    // The final remainder is the result of the modulo operation
    return remainder;
}

int main(void) {

    int x = 115;
    int n = 103;
    
    int size = 0;
    int result[100000];
    int temp = x;
    int i;

    // Load each digit of base(x) into array backwards
    while(temp != 0) {
        result[size++] = temp % 10;
        temp = temp / 10;
    }

    // Print loaded array and exponent
    printf("Base in array: ");
    for (i = 0; i < size; i++) {
        printf("%d,", result[i]);
    }
    printf("\nexponent: %d\n\n", n);

    for (i = 2; i <= n; i++) {
        size = multiplyArrayItems(x, result, size);
    }

    printf("\nx^n result: ");
    for (i = size-1; i >= 0; i--) {
        printf("%d", result[i]);
    }
    
    // GET MODULO

    int divisor = 143;

    int moduloResult = arrayModulo(result, size, divisor);

    // Display the result
    printf("\n\nModulo result: %d\n", moduloResult);

    return 0;
}