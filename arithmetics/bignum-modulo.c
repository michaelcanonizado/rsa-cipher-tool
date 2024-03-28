#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h> 
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

// NOTE: This function was copied from multiplyBignumShiftLeft() in ../bignum.c. Rename multiplyBignumShiftLeft() to bignumShiftLeft as it can be used in other bignum.c functions other than multiplyBignum().
int bignumShiftLeft(Bignum *result, Bignum *num, unsigned long long int shiftPlaces) {  
    if (shiftPlaces < 0) {
        printf("Shifting Bignum by negative value/s.\n");
        return -1;
    }

    unsigned long long int resultLength = shiftPlaces;

    memset(result->digits, 0, sizeof(int) * shiftPlaces);

    for (unsigned long int i = shiftPlaces, j = 0; j < num->length; i++, j++) {
        result->digits[i] = num->digits[j];
        
        resultLength++;
    }

    result->length = resultLength;
}

int getAverage(Bignum *result, Bignum *num1, Bignum *num2) {
    Bignum num1PlusNum2 = initBignum();

    addBignum(&num1PlusNum2, num1, num2);

    int carry = 0;

    for (int i = num1PlusNum2.length - 1; i >= 0; i--) {
        result->digits[i] = (num1PlusNum2.digits[i] / 2) + carry;

        if (num1PlusNum2.digits[i] % 2 != 0) {
            carry = 5;
        } else {
            carry = 0;
        }
    }

    result->length = num1PlusNum2.length;
    trimBignum(result);
}

unsigned long long int moduloInt(unsigned long long int dividend, unsigned long long int divisor) {
    // Function will use repeated multiplication to find the quotient of the dividend and divisor. dividend - (quotient * divisor) will then give the remainder/modulo
    // 111 / 20
    // 20 * 1 = 20
    // 20 * 2 = 40
    // 20 * 3 = 60
    // 20 * 4 = 80
    // 20 * 5 = 100
    // 111 - 100 = 11 <- remainder/modulo
    // But it will use binary search to look for the quotient for faster calculations.
    //The left and right values limits will start from the min and max number of digits of the quotient. The estimated number of digits of the quotient can be determined by: num of digits of divident - num of digits of divisor.

    unsigned long long int multiplyResult = 0, count;

    // Get left and right limits
    unsigned long long int countLowerLimit = pow(10, getLengthOfInteger(dividend) - (getLengthOfInteger(divisor) + 1));
    unsigned long long int countUpperLimit = pow(10, getLengthOfInteger(dividend) - (getLengthOfInteger(divisor) - 1));
    
    // Perform binary search to find the quotient
    while(1) {
        // Get the middle value. This will be used as a multiple to the divisor
        count = (countUpperLimit + countLowerLimit) / 2;

        printf("L: %llu | R: %llu | Mid: %llu", countLowerLimit, countUpperLimit, count);

        // Get product
        multiplyResult = divisor * count;
        printf("\ndivisor: %llu * count: %llu = %llu | dividend: %llu\n\n", divisor, count, multiplyResult, dividend);

        // CHeck if product is in the left or right half. Adjust the limits accordingly
        if (multiplyResult > dividend) {
            countUpperLimit = count;
        } else if (multiplyResult < dividend) {
            countLowerLimit = count;
        }

        // Check if the product is fit to be a quotient.
        // I.e: (Dividend - Product) < Divisor && (Dividend - Product) >= 0
        if (
            ((dividend - multiplyResult) < divisor) && 
            (
                (dividend - multiplyResult) >= 0
            )) {
            printf("\n\nLAST COUNT: %llu\n", count);
            break;
        }
    }

    return dividend - (count * divisor);
}

int moduloBignum(Bignum *result, Bignum *dividend, Bignum *divisor) {
    unsigned long long int countInt;

    if (isLessThanBignum(dividend, divisor)) {
        copyBignum(result, dividend);
        return 0;
    }

    Bignum tempOne = initBignum();
    Bignum counterLeftIndex = initBignum();
    Bignum counterRightIndex = initBignum();
    Bignum counterMiddleIndex = initBignum();

    setBignum(&tempOne, "1", positive);

    unsigned long long int leftShiftBy = 0;
    unsigned long long int rightShiftBy = dividend->length - (divisor->length - 1);

    // Check if dividend.length - (divisor.length + 1) is a negative number. I.e: The dividend and divisor is of the same digits. If yes, keep leftShiftBy as 0, as the quotient will be only 1 digit long.
    if ((divisor->length + 1) < dividend->length) {
        leftShiftBy = dividend->length - (divisor->length + 1);
    }
    printf("\nleft shift->  %lld\nright shift->  %lld", leftShiftBy, rightShiftBy);

    printf("\nLSB: %llu | RSB: %llu", leftShiftBy, rightShiftBy);

    bignumShiftLeft(&counterLeftIndex, &tempOne, leftShiftBy);
    bignumShiftLeft(&counterRightIndex, &tempOne, rightShiftBy);

    unsigned long long int countLowerLimit = pow(10, dividend->length - (divisor->length + 1));
    unsigned long long int countUpperLimit = pow(10, dividend->length - (divisor->length - 1));
    
    Bignum multiplyResult = initBignum();
    setBignum(&multiplyResult, "0", positive);

    while(1) {
        multiplyResult = initBignum();
        getAverage(&counterMiddleIndex, &counterLeftIndex, &counterRightIndex);

        printf("\n\nL:");
        printBignum(&counterLeftIndex);
        printf(" ---- M:");
        printBignum(&counterMiddleIndex);
        printf(" ---- R:");
        printBignum(&counterRightIndex);
        printf("\n");

        multiplyBignum(&multiplyResult, divisor, &counterMiddleIndex);


        printf("\nMultiply Result: ");
        printBignum(&multiplyResult);

        if (isGreaterThanBignum(&multiplyResult, dividend)) {
            printf("\nMR is greater than dividend...");
            copyBignum(&counterRightIndex, &counterMiddleIndex);
        } else if (isLessThanBignum(&multiplyResult, dividend)) {
            printf("\nMR is less than dividend...");
            copyBignum(&counterLeftIndex, &counterMiddleIndex);
        }

        Bignum dividendMinusMultiplyResult = initBignum();
        subtractBignum(&dividendMinusMultiplyResult, dividend, &multiplyResult);

        printf("\n\n---------------------------------------------\n");

        if (
            (isLessThanBignum(&dividendMinusMultiplyResult, divisor)) && 
            dividendMinusMultiplyResult.sign == positive
            ) {
            printf("\n\nLAST COUNT/QUOTIENT: %llu\n", counterMiddleIndex);
            break;
        }

    }
    
    subtractBignum(result, dividend, &multiplyResult);
}

int bignumModulo(Bignum *result, Bignum *dividend, Bignum *divisor) {
    Bignum remainder = initBignum();
    Bignum ten = initBignum();
    setBignum(&remainder, "0", positive);
    setBignum(&ten, "10", positive);

    // Iterate through the array from left to right
    for (int i = dividend->length - 1; i >= 0; i--) {
        printf("\ntest 1\n");
        // Calculate the current result including the next digit
        // int currentResult = remainder * 10 + dividend->digits[i];
        Bignum remainderTimesTen = initBignum();
        Bignum currentResult = initBignum();
        Bignum currentDividendDigit = initBignum();
        printf("\ntest 2\n");
        // bignumShiftLeft(&remainderTimesTen, &remainder, 1);
        multiplyBignum(&remainderTimesTen, &remainder, &ten);
        printf("\ntest 3\n");
        intToBignum(&currentDividendDigit, dividend->digits[i], positive);
        printf("\ntest 4\n");
        addBignum(&currentResult, &remainderTimesTen, &currentDividendDigit);
        printf("\ntest 5\n");

        printf("\nr: ");
        printBignum(&remainder);
        printf(" * 10 + ");
        printBignum(&currentDividendDigit);
        printf(" = ");
        printBignum(&currentResult);
        printf("\n\n");
        printBignum(&currentResult);
        printf("  mod  ");
        printBignum(divisor);

        // printf("\nr: %d * %d + curr_val: %d = cur_r: %d", remainder, 10, dividend->digits[i], currentResult);
        // Update the remainder for the next iteration
        // remainder = currentResult % divisor;
        moduloBignum(&remainder, &currentResult, divisor);
        printf("\ntest 6\n");
        // printf("\ncur_r: %d %% %d = upd_r: %d\n\n", currentResult, divisor, remainder);
    }

    copyBignum(result, &remainder);

    // The final remainder is the result of the modulo operation
    // return remainder;
}

int moduloBignumCompressed(Bignum *result, Bignum *dividend, Bignum *divisor) {
    unsigned long long int countInt;

    if (isLessThanBignum(dividend, divisor)) {
        copyBignum(result, dividend);
        return 0;
    }

    Bignum tempOne = initBignum();
    Bignum counterLeftIndex = initBignum();
    Bignum counterRightIndex = initBignum();
    Bignum counterMiddleIndex = initBignum();

    setBignum(&tempOne, "1", positive);

    unsigned long long int leftShiftBy = 0;
    unsigned long long int rightShiftBy = dividend->length - (divisor->length - 1);

    if ((divisor->length + 1) < dividend->length) {
        leftShiftBy = dividend->length - (divisor->length + 1);
    }

    bignumShiftLeft(&counterLeftIndex, &tempOne, leftShiftBy);
    bignumShiftLeft(&counterRightIndex, &tempOne, rightShiftBy);

    unsigned long long int countLowerLimit = pow(10, dividend->length - (divisor->length + 1));
    unsigned long long int countUpperLimit = pow(10, dividend->length - (divisor->length - 1));
    
    Bignum multiplyResult = initBignum();
    setBignum(&multiplyResult, "0", positive);

    while(1) {
        multiplyResult = initBignum();

        getAverage(&counterMiddleIndex, &counterLeftIndex, &counterRightIndex);

        multiplyBignum(&multiplyResult, divisor, &counterMiddleIndex);

        if (isGreaterThanBignum(&multiplyResult, dividend)) {
            copyBignum(&counterRightIndex, &counterMiddleIndex);
        } else if (isLessThanBignum(&multiplyResult, dividend)) {
            copyBignum(&counterLeftIndex, &counterMiddleIndex);
        }

        Bignum dividendMinusMultiplyResult = initBignum();
        subtractBignum(&dividendMinusMultiplyResult, dividend, &multiplyResult);

        if (
            (isLessThanBignum(&dividendMinusMultiplyResult, divisor)) && 
            dividendMinusMultiplyResult.sign == positive
            ) {
            break;
        }

    }
    Bignum countTimesDivisor = initBignum();
    subtractBignum(result, dividend, &multiplyResult);
}

int bignumModuloCompressed(Bignum *result, Bignum *dividend, Bignum *divisor) {
    Bignum remainder = initBignum();
    Bignum ten = initBignum();
    setBignum(&remainder, "0", positive);
    setBignum(&ten, "10", positive);

    for (int i = dividend->length - 1; i >= 0; i--) {
        Bignum remainderTimesTen = initBignum();
        Bignum currentResult = initBignum();
        Bignum currentDividendDigit = initBignum();
        multiplyBignum(&remainderTimesTen, &remainder, &ten);
        intToBignum(&currentDividendDigit, dividend->digits[i], positive);
        addBignum(&currentResult, &remainderTimesTen, &currentDividendDigit);
        moduloBignumCompressed(&remainder, &currentResult, divisor);
    }

    copyBignum(result, &remainder);
}

int main() {
    Bignum bignumX = initBignum();
    Bignum bignumY = initBignum();
    Bignum bignumRes = initBignum();

    // char x[] = "1";
    // char y[] = "20";
    // char x[] = "111";
    // char y[] = "20";
    char x[] = "5770006211367438645738846923103019093676944772562452020385513091639388860966725675839337369352270554";
    char y[] = "83229083364470435010488347620930684553892670457201390740089766094502884007293211299596918";

    setBignum(&bignumX, x, positive);
    setBignum(&bignumY, y, positive);
    
    clock_t start_time = clock();

    moduloBignumCompressed(&bignumRes, &bignumX, &bignumY);
    // bignumModuloCompressed(&bignumRes, &bignumX, &bignumY);

    clock_t end_time = clock();
    double CPU_TIME_USED = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("\n\nCPU TIME USED: %lf secs", CPU_TIME_USED);

    printf("\n\n%s \nMod\n%s \nRESULT:\n", x, y);
    printBignum(&bignumRes);
    printf("\n\n");

    return 0;
}