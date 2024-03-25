#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int multiplyArrayItems(int x, int result[], int size, int *insertedItems) {
    int carry = 0;
    int product;

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
        (*insertedItems)++;
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
    // Start timer
    clock_t begin = clock();

    // int x = 256312;
    // int n = 43;
    int x = 72;
    int n = 2;
    
    int size = 0;
    // Dynamically allocate memory to array
    int *result = malloc(sizeof(int) * 100000000);
    int temp = x;
    int itemsInserted = 0;
    int i;

    // Load each digit of base(x) into array backwards
    while(temp != 0) {
        result[size++] = temp % 10;
        temp = temp / 10;
        itemsInserted++;
    }

    // Print loaded array and exponent
    printf("Base in array: ");
    for (i = 0; i < size; i++) {
        printf("%d,", result[i]);
    }
    printf("\nexponent: %d", n);
    printf("\nNum of digits in base: %d", itemsInserted);

    printf("\n\nMultiplying digits...");
    for (i = 2; i <= n; i++) {
        size = multiplyArrayItems(x, result, size, &itemsInserted);
    }
    printf("\nGetting power done...");

    printf("\n\nx^n result: ");
    for (i = size-1; i >= 0; i--) {
        printf("%d", result[i]);
    }
    printf("\nNum of digits in result: %d", itemsInserted);
    
    // GET MODULO
    printf("\n\nGetting modulo...");
    int divisor = 421999;
    int moduloResult = arrayModulo(result, size, divisor);

    // Display the result
    printf("\nModulo result: %d\n\n", moduloResult);

    // Free malloc'ed array
    free(result);

    // End timer
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("ELAPSED TIME CALCULATING: %lf seconds\n\n", time_spent);

    return 0;
}