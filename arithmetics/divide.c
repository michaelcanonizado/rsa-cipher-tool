#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

    void printArray(int* arr, int len) {
        for (int i = 0; i < len; i++) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }

int compare(int* arr1, int* arr2, int len1) {
    for (int i = 0; i < len1; i++) {
        if (arr1[i] < arr2[i]) return -1;
        if (arr1[i] > arr2[i]) return 1;
    }
    return 0;
}

char* divideLargeNumbers(const char* dividend, const char* divisor) {
    // Convert the dividend and divisor to integers
    int dividendLen = strlen(dividend);
    int divisorLen = strlen(divisor);
    int* dividendArr = (int*)malloc(dividendLen * sizeof(int));
    int* divisorArr = (int*)malloc(divisorLen * sizeof(int));
    
    for (int i = 0; i < dividendLen; i++) {
        dividendArr[i] = dividend[i] - '0';
        printf("%d,", dividendArr[i]);
    }

    printf("\n");

    for (int i = 0; i < divisorLen; i++) {
        divisorArr[i] = divisor[i] - '0';
        printf("%d,", divisorArr[i]);
    }
    
    printf("\n");

    // Perform long division
    int quotientLen = dividendLen - divisorLen + 1;
    int* quotientArr = (int*)malloc(quotientLen * sizeof(int));
    memset(quotientArr, 0, quotientLen * sizeof(int));
    
    int i, j;
   
    for (i = 0; i <= dividendLen - divisorLen; i++) {
        while (compare(&dividendArr[i], divisorArr, divisorLen) >= 0) {
            for (j = 0; j < divisorLen; j++) {
                dividendArr[i + j] -= divisorArr[j];
                if (dividendArr[i + j] < 0) {
                    dividendArr[i + j] += 10;
                    dividendArr[i + j + 1]--;
                }
                printf("After subtraction:\n");
                printArray(dividendArr, dividendLen);
            }
            quotientArr[i]++;
            printf("Quotient after increment:\n");
            printArray(quotientArr, dividendLen - divisorLen + 1);
        }
    }
    
    // Convert the quotient array to a string
    char* quotient = (char*)malloc((quotientLen + 1) * sizeof(char));
    
    for (int i = 0; i < quotientLen; i++) {
        quotient[i] = quotientArr[i] + '0';
    }
    
    quotient[quotientLen] = '\0';
    
    // Free memory
    free(dividendArr);
    free(divisorArr);
    free(quotientArr);
    
    return quotient;
}

int main() {
    // Start timer
    clock_t begin = clock();

    const char* dividend = "12345678901234567890";
    const char* divisor = "12345";
    
    char* quotient = divideLargeNumbers(dividend, divisor);
    
    printf("Quotient: %s\n", quotient);

    free(quotient);

    // End timer
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("ELAPSED TIME CALCULATING: %lf seconds\n\n", time_spent);
    
    return 0;
}