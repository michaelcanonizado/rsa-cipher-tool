#include <stdio.h>
#include <stdlib.h>

// Function to calculate the number of digits in an integer
int countDigits(int num) {
    int count = 0;
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

// Function to spread digits of an integer into an array
int* spreadDigits(int num, int* size) {
    *size = countDigits(num);
    
    // Allocate memory for the array dynamically
    int* arr = (int*)malloc(*size * sizeof(int));

    // Iterate through each digit of the number
    for (int i = *size - 1; i >= 0; i--) {
        arr[i] = num % 10;  // Extract the last digit
        num /= 10;          // Remove the last digit
    }

    return arr;
}

int main() {
    int num = 211;  // Replace this with your desired integer
    int size;
    
    // Call the function to spread digits into the array
    int* arr = spreadDigits(num, &size);

    printf("size of array: %d\n", size);

    // Display the array elements
    printf("Array after spreading digits: ");
    for (int i = 0; i < size; i++) {
        printf("%d,", arr[i]);
    }

    // Free the dynamically allocated memory
    free(arr);

    return 0;
}