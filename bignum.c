// OPERATIONS NEEDED TO BE IMPLEMENTED:
// Greater Than - Done
// Less Than - Done
// Equal To - Done
// Addition - Ongoing
// Subtraction - Ongoing
// Greatet Than Equal To - Not yet started
// Less Than Equal To - Not yet started
// Increment - Not yet started
// Decrement - Not yet started
// Multiplication - Not yet started
// Division - Not yet started
// Modulus - Not yet started
// Exponents - Not yet started

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "bignum.h"

int getLengthOfInteger(long long int num) {
    if (num == 0) {
        return 1;
    } else if (num < 0) {
        num = num * -1;
    }

    return (int)log10((double)num) + 1;
}

Bignum initBignum() {
    // Initialize Bignum values. This is needed some of the arithmetic function need to know if the Bignum has already been set, and get rid of garbage values.
    Bignum num;
    num.length = 0;
    num.sign = positive;
    return num;
}

void setBignum(Bignum *numStruct, char numStr[], BIGNUM_SIGN sign) {
    int temp;

    // Store sign enum (1 = negative || 0 = positive)
    numStruct->sign = sign;

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

    // Trim result. Removing any possible leading 0s. E.g. "0000123" will be [3,2,1]
    trimBignum(numStruct);
}

void intToBignum(Bignum *numStruct, unsigned long long int integer, BIGNUM_SIGN sign) {
    // Unsigned long long int is used and a separate sign parameter is used to increase the integer range of the function. You will just have to conditionally input the sign when calling this function, when needed.

    // If integer passed is 0, set numStruct digits to [0], length to 1, and sign to positive.
    if (integer == 0) {
        numStruct->digits[0] = 0;
        numStruct->length = 1;
        numStruct->sign = positive;
        return;
    }

    // Use a counter to track indexes and length. Int is used as the unsigned longlong int data type's max digit count is 20
    int count = 0;

    // Push remainder of the modulo of the last digit of the integer to the Bignum struct, then divide the integer by 10 to shift the integer. Do this untill all digits have been pushed.
    while(integer > 0) {
        numStruct->digits[count] = integer % 10;
        integer = integer / 10;
        count++;
    }

    // Store the count in Bignum.length
    numStruct->length = count;
    numStruct->sign = sign;
}

long long int bignumToInt(Bignum *num) {
    int maxNumOfDigits = (int)log10((double)MAX_VALUE_OF_LONG_LONG_INT) + 1;

    if (num->length > maxNumOfDigits) {
        return 0;
    }

    long long int result = 0;
    long long int multiplier = num->sign == negative ? -1 : 1;

    for (int i = 0; i < num->length; i++) {
        result += num->digits[i] * multiplier;
       
        multiplier *= 10;
    }

    return result;
}

void copyBignum(Bignum *result, Bignum *num) {
    memcpy(result->digits, num->digits, sizeof(int) * num->length);
    result->length = num->length;
    result->sign = num->sign;
}

void printBignum(Bignum *num) {
    for (int i = num->length - 1; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
}

void printBignumCenter(Bignum *num, unsigned int requiredWidth) {
    // If bignum overflows the required width, just print bignum.
    if (requiredWidth <= num->length) {
        printBignum(num);
        return;
    }

    // This function prioritizes the required width over centeredness to stay consistent when used in formatted prints. Bignums with odd lenghts wont be perfectly centered, so it either needs to have equal spaces on both sides (prioritizing centeredness) or 1 side having less space but meet the required width.
    // Eg: Bignum = -1234 with the required with of 10 spaces, will be __-1234___ not ___-1234___.

    // Calculate left and right widths.
    unsigned int leftWidth = (requiredWidth - num->length) / 2;
    unsigned int rightWidth = (requiredWidth - num->length) / 2;
    // Calculate total width to determine if we have exceeded or receded the required width.
    unsigned int totalWidth = leftWidth + rightWidth + num->length;

    // if bignum is negative, decrement left width as it will be printed later on.
    if (num->sign == negative) {
        leftWidth--;
    }

    // If total width exceeds or recedes required width, decrement or increment respectively.
    if (totalWidth > requiredWidth) {
        leftWidth--;
    } else if (totalWidth < requiredWidth) {
        leftWidth++;
    }

    // Print bignum
    for (int i = 0; i < leftWidth; i++) {
        printf(" ");
    }
    if (num->sign == negative) {
        printf("-");
    }
    printBignum(num);
    for (int i = 0; i < rightWidth; i++) {
        printf(" ");
    }
}

int isGreaterThanBignum(Bignum *num1, Bignum *num2) {
    if (num1->sign == negative && num2->sign == positive) {
        return 0;
    }
    if (num1->sign == positive && num2->sign == negative) {
        return 1;
    }

    if (num1->length > num2->length) {
        return 1;
    }
    if (num1->length < num2->length) {
        return 0;
    }

    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] > num2->digits[i]) {
            return 1;
        }

        if (num1->digits[i] < num2->digits[i]) {
            return 0;
        }
    }

    return 0;
}

int isLessThanBignum(Bignum *num1, Bignum *num2) {
    if (num1->sign == negative && num2->sign == positive) {
        return 1;
    }
    if (num1->sign == positive && num2->sign == negative) {
        return 0;
    }

    if (num1->length > num2->length) {
        return 0;
    }
    if (num1->length < num2->length) {
        return 1;
    }

    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] < num2->digits[i]) {
            return 1;
        }

        if (num1->digits[i] > num2->digits[i]) {
            return 0;
        }
    }

    return 0;
}

int isEqualToBignum(Bignum *num1, Bignum *num2) {
    if (num1->sign != num2->sign) {
        return 0;
    }

    if (num1->length != num2->length) {
        return 0;
    }

    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] != num2->digits[i]) {
            return 0;
        }
    }

    return 1;
}

void trimBignum(Bignum *num) {
    unsigned long long int numOfZeros = 0;

    // Start from the most significant digit, looking for 0s, and keep track of the number of 0s found.
    for (int i = num->length - 1; i >= 0; i--) {
        if (num->digits[i] == 0) {
            numOfZeros++;
        } else {
            break;
        }
    }

    // If 0s were found, trim the Bignum by adjusting the length.
    if (numOfZeros != 0) {
        num->length = num->length - numOfZeros;
    }
}

int isBignumZero(Bignum *num) {
    if (num->length <= 1 && num->digits[0] == 0) {
        return 1;
    } else {
        return 0;
    }
}

void addBignum(Bignum *result, Bignum *num1, Bignum *num2) {
    int sum;
    int carry = 0;
    int resultLength = 0;
    // maxLength will cap out at around 18,446,744,073,709,551,615 (ref: C docs). Therefore the Bignum num1 and num2 can only have a maximum of 18,446,744,073,709,551,615 digits.
    unsigned long long int maxLength;

    // If the 2 Bignums have different signs. Perform subtraction.
    if (num1->sign != num2->sign) {
        // Keep track of the original signs of the 2 Bignums. As they need to have the same sign to trigger subtraction in the subtractBignum() function. If they have contrasting signs, subtractBignum() will call addBignum() causing an infinite loop.
        BIGNUM_SIGN num1Sign = num1->sign;
        BIGNUM_SIGN num2Sign = num2->sign;

        num1->sign = num2->sign;

        if (isGreaterThanBignum(num1, num2)) {
            subtractBignum(result, num1, num2);
            // The resulting Bignum will have the sign of the bigger number(disregarding signs). E.g. 11 + (-5) = 6.
            result->sign = num1Sign;
            // Bring back the original signs of the two Bignums.
            num1->sign = num1Sign;
            num2->sign = num2Sign;

            return;
        } else if (isLessThanBignum(num1, num2)) {
            subtractBignum(result, num1, num2);
            // The resulting Bignum will have the sign of the bigger number(disregarding signs). E.g. 5 + (-11) = -6.
            result->sign = num2Sign;
            // Bring back the original signs of the two Bignums.
            num1->sign = num1Sign;
            num2->sign = num2Sign;

            return;
        } else if (isEqualToBignum(num1, num2)) {
            // If the 2 Bignums have different signs and are equal to each other. the result will be 0.
            result->digits[0] = 0;
            result->length = 1;
            result->sign = positive;

            return;
        }
    }

    // Find the longest length
    if (num1->length > num2->length) {
        maxLength = num1->length;
    } else if (num2->length > num1->length) {
        maxLength = num2->length;
    } else {
        maxLength = num1->length;
    }

    // Perform addition
    for (int i = 0; i < maxLength; i++) {
        sum = carry;
        sum += num1->digits[i] + num2->digits[i];
        carry = sum >= 10 ? 1 : 0;
        sum = sum % 10;

        result->digits[i] = sum;

        resultLength++;
    }

    // If there is a remaining carry, append to result. e.g. 8 + 9 = 17 NOT 8 + 9 = 7 (the carry in the buffer was not added)
    if (carry == 1) {
        result->digits[resultLength] = carry;
        resultLength++;
    }

    // Store result digit length
    result->length = resultLength;
    // Copy sign of one of the Bignums. E.g. (+1) + (+2) = (+3) & (-1) + (-2) = (-3)
    result->sign = num1->sign;
    // Trim result. Removing any possible leading 0s
    trimBignum(result);
}

void subtractBignum(Bignum *result, Bignum *num1, Bignum *num2) {
    if (isBignumZero(num1)) {
        copyBignum(result, num2);
        return;
    }
    if (isBignumZero(num2)) {
        copyBignum(result, num1);
        return;
    }

    // Compare the 2 integers to determine whether to add or subract (subraction rules) and determine the sign of result.

    // Check signs | If signs are different, add the two numbers.
    if (num1->sign == positive && num2->sign == negative) {
        num2->sign = positive;
        addBignum(result, num1, num2);
        num2->sign = negative;
        return;
    }
    if (num1->sign == negative && num2->sign == positive) {
        num2->sign = negative;
        addBignum(result, num1, num2);
        result->sign = negative;
        num2->sign = positive;
        return;
    }

    // Find minuend and subtrahend | Store in a temp Bignum as minuend's Bignum.digits will be manipulated due to carries. This is also needed as minuend can be either of the two parameters(num1 or num2); If minuend is found. the other number will be the subtrahend.
    Bignum minuend = initBignum();
    Bignum subtrahend = initBignum();

    // Check length | Longer length will automatically be set to the minuend and shorter will be the subtrahend.
    if (num1->length > num2->length) {
        minuend.length = num1->length;
        subtrahend.length = num2->length;

        memcpy(&minuend.digits, num1->digits, sizeof(int) * num1->length);
        memcpy(&subtrahend.digits, num2->digits, sizeof(int) * num2->length);

        result->sign = num1->sign;
    } else if (num1->length < num2->length) {
        minuend.length = num2->length;
        subtrahend.length = num1->length;

        memcpy(&minuend.digits, num2->digits, sizeof(int) * num2->length);
        memcpy(&subtrahend.digits, num1->digits, sizeof(int) * num1->length);

        // If num1 - num2, and bot Bignums have the same sign, and num2 is longer than num1, the result's sign will be the inverse of the sign of the 2 Bignums.
        // E.g.: (+7) - (+10) = -3 or (-7) - (-10) = 3 
        if (num1->sign == positive) {
            result->sign = negative;
        } else if (num1->sign == negative) {
            result->sign = positive;
        }
    }

    // Compare two Bignums | If minuend and subtrahend was not found in the previous conditions, i.e. they're of the same sign and length. Compare the two Bignums.
    if (isGreaterThanBignum(num1, num2) && minuend.length == 0) {
        minuend.length = num1->length;
        subtrahend.length = num2->length;

        memcpy(&minuend.digits, num1->digits, sizeof(int) * num1->length);
        memcpy(&subtrahend.digits, num2->digits, sizeof(int) * num2->length);

        result->sign = num1->sign;
    }else if (isLessThanBignum(num1, num2) && minuend.length == 0) {
        minuend.length = num2->length;
        subtrahend.length = num1->length;

        memcpy(&minuend.digits, num2->digits, sizeof(int) * num2->length);
        memcpy(&subtrahend.digits, num1->digits, sizeof(int) * num1->length);

        // If num1 - num2, and both Bignums have the same sign,  num2 is greater than num1, the result's sign will be the inverse of the sign of the 2 Bignums.
        // E.g.: (+30) - (+70) = -40 or (-30) - (-70) = 40 
        if (num1->sign == positive) {
            result->sign = negative;
        } else if (num1->sign == negative) {
            result->sign = positive;
        }
    } else if (isEqualToBignum(num1, num2)) {
        result->digits[0] = 0;
        result->length = 1;
        result->sign = positive;

        return;
    }

    // Once miunend and subtrahends are found, perform subtraction.
    int difference;
    unsigned long long int resultLength = 0;

    // Start at the least significant digit (index 0). Then iterate through till the min length (subtrahend length).
    for (int i = 0; i < subtrahend.length; i++) {
        // If current minuend digit is greater than the current subtrahend digit: No need to borrow.
        if (minuend.digits[i] > subtrahend.digits[i]) {
            // Get difference of the current minuend and subtrahend digits and increment the result's digit length counter.
            result->digits[i] = minuend.digits[i] - subtrahend.digits[i];
            resultLength++;
        } else if (minuend.digits[i] < subtrahend.digits[i]) {
            // If borrowing is needed, start from the next digit after the current index, and traverse through (till the last digit of the minuend) until you find a digit that can give you a borrow (anything greater than 0).
            for (int j = i + 1; j < minuend.length; j++) {
                if (minuend.digits[j] > 0) {
                    // If a digit that can give a borrow is found. Decrement that digit, then add 10 to the current index that needs a borrow.
                    minuend.digits[j] = minuend.digits[j] - 1;
                    minuend.digits[i] = minuend.digits[i] + 10;

                    // Since a borrow was found. Exit out of loop.
                    // Since a borrow was found. Exit out of loop.
                    break;
                } else if (minuend.digits[j] == 0) {
                    // If the digit is a 0, replace that 0 with 9 and move to the next digit looking for a digit that can give a borrow. We can immediately give it a value of 9, since it is bound to be a 9 as it will be a 10 once a borrow is found, then it will be decremented as it needs to give a borrow to the digit to its right.
                    //
                    // E.g.:
                    // modified minuend:   1,2,9,9,11
                    // original minuend:   1,3,0,0, 1
                    //       subtrahend: -  , , ,2, 3
                    //                    ------------
                    //           result:    1,2,9,7,8
                    minuend.digits[j] = 9;
                }
            }

            // Get difference of the current minuend and subtrahend digits and increment the result's digit length counter.
            result->digits[i] = minuend.digits[i] - subtrahend.digits[i];
            resultLength++;
        } else if (minuend.digits[i] == subtrahend.digits[i]) {
            // When minuend's and subtrahend's current iteration digit is equal. Set result's current iteration digit to 0.
            result->digits[i] = 0;
            resultLength++;
        }
    }

    // If subtrahend is shorter than the minuend, and all neccessary subtractions are finished. Drop down/copy the remaining values of the minuend.
    if (minuend.length > subtrahend.length) {
        for (int i = subtrahend.length; i < minuend.length; i++) {
            result->digits[i] = minuend.digits[i];
            resultLength++;
        }
    }

    result->length = resultLength;
    // Trim result. Removing any possible leading 0s
    trimBignum(result);
}