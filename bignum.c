// OPERATIONS NEEDED TO BE IMPLEMENTED:
// Greater Than - Done & Documented
// Less Than - Done & Documented
// Equal To - Done & Documented
// Addition - Done & Tested & Documented
// Subtraction - Done & Tested & Documented
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

int getLengthOfInteger(long long int integer) {
    // Function will count the number of digits of positive or negative integer using log based solution. If integer is negative then convert it to positive as it the logarithm function only works with positive real numbers.
    if (integer == 0) {
        return 1;
    } else if (integer < 0) {
        integer = integer * -1;
    }

    return (int)log10((double)integer) + 1;
}

Bignum initBignum() {
    // Function to initialize Bignum values. Get rid of garbage values and initialize bignum. Some arithmetic function may need to know if the Bignum has already been set.
    Bignum num;
    memset(num.digits, 0, sizeof(int) * MAX_INT_LENGTH);
    num.length = 0;
    num.sign = positive;
    return num;
}

void setBignum(Bignum *numStruct, char numStr[], BIGNUM_SIGN sign) {
    // Main Function to set a Bignum. This function takes an integer represented as a string, as strings dont have a limit to how long it can be. The each character of the string will then be converted to an integer by offsetting its ASCII value, and will be pushed to Bignum.digits[].

    // Integer will be stored in Bignum.digits[] in reverse for the following reasons: (1) Most operations usually start from the LSD (least significant digit), making it easier to perform operations. (2) The result will usually be greater than or less than the 2 integers being operated on. Eg: 999 (3 digits) + 999 (3 digits) = 1998 (4 digits) & 999 (3 digits) * 999 (3 digits) = 998,001 (6 digits). Hence, the resulting Bignum is free to shrink and grow infinitely. 

    // FEAT: Cases where the strings contain non-numerical characters haven't been set. It will be implemented soon. Eg: "123a678".

    int temp;

    // Store sign enum (0 = positive || 1 = negative)
    numStruct->sign = sign;

    // Store numStr length
    numStruct->length = strlen(numStr);

    // REFACTOR: Since the integer will be stored in reverse in Bignum.digits[], you can start to iterate through the string starting from the last character using strlen(numStr). Thus only needing 1 for-loop to convert numStr.

    // Load numbers into Bignum.digits[]
    for (int i = 0; i < numStruct->length; i++) {
        // Offset ASCII value of the character by the ASCII value of '0'
        // Eg: '3' - '0' = 3  ->  51 - 48 = 3 
        numStruct->digits[i] = numStr[i] - '0';
    }

    // Reverse Bignum.digits
    for (int i = 0; i < numStruct->length / 2; i++) {
        temp = numStruct->digits[i];
        numStruct->digits[i] = numStruct->digits[numStruct->length - i - 1];
        numStruct->digits[numStruct->length - i - 1] = temp;
    }

    // Trim result. Removing any possible leading 0s. E.g. "[3,2,1,0,0,0]" will be [3,2,1]
    trimBignum(numStruct);
}

void intToBignum(Bignum *numStruct, unsigned long long int integer, BIGNUM_SIGN sign) {
    // Function will convert as passed integer to Bignum, instead of passing an integer represented as a string in setBignum().

    // FEAT: This function doesn't take into consideration the octal represenation behavior in C. Where integers prefixed with a 0. Eg: 0123 will be 83. 

    // Unsigned long long int and a separate sign parameter is used to increase the integer range of the function. You will have to conditionally input the sign enum when calling this function.

    // If integer passed is 0, set numStruct digits to [0], length to 1, and sign to positive.
    // REFACTOR: setBignum() can be used to set it to 0, instead of settings the struct members manually.
    if (integer == 0) {
        numStruct->digits[0] = 0;
        numStruct->length = 1;
        numStruct->sign = positive;
        return;
    }

    // Use a counter to track indexes and length
    int count = 0;

    // Determine the LSD (least significant digit) by moduloing the integer by 10. Then push it to Bignum.digits[]. Divide the number by 10 to shift the integer. Do this until all digits have been pushed.
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
    // Function will convert a Bignum to an integer.

    // Get maximum number of digits of long long int. 
    int maxNumOfDigits = (int)log10((double)MAX_VALUE_OF_LONG_LONG_INT) + 1;

    // REFACTOR: THIS MUST THROW A PROPER ERROR. A BIGNUM WITH Bignum.digits[] = [0] and Bignum.length = 1 IS A VALID BIGNUM THAT CAN BE CONVERTED TO AN INTEGER. INSTEAD OF RETURNING THE RESULT, USE A POINTER PARAMETER TO POINT TO THE RESULT VARIABLE, AND THE RETURN SHOULD ONLY BE ERROR CODES.

    // If Bignum is too long to be converted to an integer, throw an error.
    // FIX: THIS DOESN'T COMPLETELY HANDLE ALL OVER FLOW CASES AS IT ONLY CHECKS Bignum.length. I.E UULONG_MAX IS: 18,446,744,073,709,551,615 (20 digits) BUT IF  A BIGNUM WITH THE DIGITS OF 99,999,999,999,999,999,999 (also 20 digits) IS PASSED, IT WILL NOT THROW AN ERROR. A SIMPLE SOLUTION WOULD BE TO USE >= INSTEAD OF ONLY > TO ONLY ACCEPT A MAXIMUM BIGNUM LENGTH OF 19 DIGITS
    if (num->length > maxNumOfDigits) {
        return 0;
    }

    long long int result = 0;
    // Determine multiplier to achieve proper sign of result
    long long int multiplier = num->sign == negative ? -1 : 1;

    // Bignum.digits will be in reverse so go through each digit, multiplying it by the multiplier, then adding to result. Lastly, multiply multiplier to move place value.
    // Eg: [3,2,1] 
    // 3 * 1 = 3, 0 + 3 = 3, 1 * 10 ->
    // 2 * 10 = 20, 3 + 20 = 23, 10 * 10 = 100 ->
    // 1 * 100 = 100, 100 * 10 = 1000, 23 + 100 = 123 ->
    // result = 123 
    for (int i = 0; i < num->length; i++) {
        result += num->digits[i] * multiplier;
       
        multiplier *= 10;
    }

    return result;
}

void copyBignum(Bignum *result, Bignum *num) {
    // Function to copy the contents of a Bignum to another Bignum
    memcpy(result->digits, num->digits, sizeof(int) * num->length);
    result->length = num->length;
    result->sign = num->sign;
}

void printBignum(Bignum *num) {
    // Function to print Bignum.digits[]
    // This won't print any \n or spaces, just the Bignum
    if (num->sign == negative) {
        printf("-");
    }
    for (int i = num->length - 1; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
}

void printBignumCenter(Bignum *num, unsigned int requiredWidth) {
    // Function to print a Bignum but with a specified width and center aligns it.
    // It prioritizes the required width over centeredness to stay consistent when used in formatted prints. Bignums with odd lenghts wont be perfectly centered, so it either needs to have equal spaces on both sides (prioritizing centeredness) or 1 side having less space but meet the required width.
    // Eg: Bignum = -1234 with the required with of 10 spaces, will be __-1234___ not ___-1234___.

    // If bignum overflows the required width, just print bignum.
    if (requiredWidth <= num->length) {
        printBignum(num);
        return;
    }

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
    printBignum(num);
    for (int i = 0; i < rightWidth; i++) {
        printf(" ");
    }
}

int isGreaterThanBignum(Bignum *num1, Bignum *num2) {
    // Function that will compare two Bignums, and determine which is greater.

    // Check sign difference. If num1 is negative and num2 is positive, immediately return false(0), and vice-versa.
    if (num1->sign == negative && num2->sign == positive) {
        return 0;
    }
    if (num1->sign == positive && num2->sign == negative) {
        return 1;
    }
    // Check length difference. If num1 is shorter than num2, immediately return false(0), and vice-versa.
    if (num1->length < num2->length) {
        return 0;
    }
    if (num1->length > num2->length) {
        return 1;
    }

    // Worst-case, both Bignums have the same sign and length. Go through each digit of the two Bignums, starting from the MSD(most significant digit), comparing them. Do this until difference is found.
    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] > num2->digits[i]) {
            return 1;
        }

        if (num1->digits[i] < num2->digits[i]) {
            return 0;
        }
    }

    // If no difference is found, it means that the two Bignums are equal. Return false(0)
    return 0;
}

int isLessThanBignum(Bignum *num1, Bignum *num2) {
    // Function that will compare two Bignums, and determine which is less.

    // Check sign difference. If num1 is positive and num2 is negative, immediately return false(0), and vice-versa.
    if (num1->sign == positive && num2->sign == negative) {
        return 0;
    }
    if (num1->sign == negative && num2->sign == positive) {
        return 1;
    }

    // Check length difference. If num1 is longer than num2, immediately return false(0), and vice-versa.
    if (num1->length > num2->length) {
        return 0;
    }
    if (num1->length < num2->length) {
        return 1;
    }

    // Worst-case, both Bignums have the same sign and length. Go through each digit of the two Bignums, starting from the MSD(most significant digit), comparing them. Do this until difference is found.
    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] < num2->digits[i]) {
            return 1;
        }

        if (num1->digits[i] > num2->digits[i]) {
            return 0;
        }
    }

    // If no difference is found, it means that the two Bignums are equal. Return false(0)
    return 0;
}

int isEqualToBignum(Bignum *num1, Bignum *num2) {
    // Function that will compare two Bignums, determining whether they are equal.

    // If they have different signs, return false(0)
    if (num1->sign != num2->sign) {
        return 0;
    }

    // If they have different lengths, return false(0)
    if (num1->length != num2->length) {
        return 0;
    }

    // Worst-case, both Bignums have the same sign and length. Go through each digit of the two Bignums, starting from the MSD(most significant digit), comparing them. Do this until difference is found.
    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] != num2->digits[i]) {
            return 0;
        }
    }

    // If no difference is found, it means that the two Bignums are equal. Return true(1)
    return 1;
}

void trimBignum(Bignum *num) {
    // Function to trim leading 0s of Bignum. This function works by counting leading 0s encapsulated by the current Bignum.length, and once a non-zero is found, will update the length of Bignum by subtracting the count of leading 0s.

    unsigned long long int numOfZeros = 0;

    // Start from the most significant digit, looking for 0s, and keep track of the number of 0s found.
    for (int i = num->length - 1; i >= 0; i--) {
        if (num->digits[i] == 0) {
            numOfZeros++;
        } 
        // If a non-zero integer is found, it has found the MSB(most significant digit), exit the loop and update Bignum.length.
        else {
            break;
        }
    }

    // If 0s were found, trim the Bignum by adjusting the length.
    if (numOfZeros != 0) {
        num->length = num->length - numOfZeros;
    }
}

int isBignumZero(Bignum *num) {
    // Function to determine if a Bignum is zero.

    // A zero can be determined if it is 1 digit long, and the first digit is 0.

    // REFACTOR: ADD MORE CONDITIONS TO VERIFY IS BIGUM IS 0. SUCH AS:
    //    - CHECKING THE OTHER INDEXES FOR NON-ZEROS (Bignum.digits[0] might be 0, but the following indexes might have non-zeros)
    //    - A ZERO HAS THE SIGN OF POSITIVE IN Bignum STRUCT DEFINITION.
    //    - A BIGNUM THAT HASN'T BEEN PROPERLY INITIALIZED, (Ie: initBignum() was not used) WILL BE CONSIDERED AS A ZERO. DECIDE WHETHER THIS IS A WANTED BEHAVIOR

    if (num->length <= 1 && num->digits[0] == 0) {
        return 1;
    } else {
        return 0;
    }
}

void addBignum(Bignum *result, Bignum *num1, Bignum *num2) {
    // Function to add two Bignums together.
    // Uses basic addition which starts at the LSD (least significant digit) and adds the digits of the addends together, iterating till it reaches the end.

    // NOTE: This function currently iterates and adds 1 digit at a time.

    // REFACTOR: THIS FUNCTION CAN BE REFACTORED IN A WAY THAT DOESN'T ADD 1 DIGIT AT A TIME. YOU CAN ADD THE DIGITS IN GROUPS, USING AND ADHERING TO THE MAXIMUM VALUES OF C. IF THERE ARE ANY EXCESS, CARRY IT TO THE NEXT GROUP.
    //
    // Eg: 1234 + 5678
    // *Adding in groups of 2*
    //                         1
    //   12 | 34              12 | 34
    //  +56 | 78             +56 | 78
    //  ----------    -->   ----------  
    //   68 | 112             69 | 12

    // Use addition rule: if the two Bignums have different signs. Perform subtraction.
    if (num1->sign != num2->sign) {
        // Keep track of the original signs of the two Bignums. As they need to have the same sign to trigger subtraction in the subtractBignum() function. If they have contrasting signs, subtractBignum() will call addBignum() causing an infinite loop.
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

    int sum;
    int carry = 0;
    int resultLength = 0;
    // Use usigned long long int to match the data type of the length property of Bignum struct.
    unsigned long long int maxLength;

    // Find the longest length. This is needed to determine which Bignum.length to use in the for-loop.
    if (num1->length > num2->length) {
        maxLength = num1->length;
    } else if (num2->length > num1->length) {
        maxLength = num2->length;
    } else {
        maxLength = num1->length;
    }

    // Perform addition (this for-loop adds 1 digit at a time. Refer to the REFACTOR note above for more info).
    for (int i = 0; i < maxLength; i++) {
        // FEAT: IF ONE OF THE CURRENT DIGIT IS 0, YOU CAN DIRECTLY SET THE OTHER BIGNUM DIGIT TO CURRENT DIGIT IN RESULT TO SKIP OPERATIONS BEING DONE. 

        // Set sum to the previous carry
        sum = carry;
        // Add the digits of the addends together
        sum += num1->digits[i] + num2->digits[i];
        // If sum is more than 1 digit long, set carry to 1.
        carry = sum >= 10 ? 1 : 0;
        // Modulo sum by 10 to get only 1 digit to be pushed to Bignum result. This works whether sum is more than 1 digit long or not.
        // Eg: 19 % 10 = 9 or 9 % 10 = 9
        sum = sum % 10;
        // Store sum in result.digits[]
        result->digits[i] = sum;
        // Increment resultLength to keep track of lenght of result Bignum
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
    // Function to subtract two Bignums together.
    // Uses basic subtraction which starts at the LSD (least significant digit) and subtracts the digits of the minuend and subtrahend together, iterating till it reaches the end. If a borrow is needed, it will loop through the next digits of the minuend until it finds a digit that can give a borrow.

    // NOTE: This function currently iterates and adds 1 digit at a time.

    // If you are subtracting a Bignum with 0, copy the other Bignum to result.
    if (isBignumZero(num1)) {
        copyBignum(result, num2);
        return;
    }
    if (isBignumZero(num2)) {
        copyBignum(result, num1);
        return;
    }

    // NOTE: FUnction will check signs, length, and compare the two Bignums to determine twhat subtraction rule to follow.

    // Check signs. If signs are different, add the two numbers.

    // Transpose the signs: Make the two Bignums have the same sign, to trigger addition in the addBignum() function. Having different signs in addBignum() will call subtractBignum(), causing an infinite loop. Once addition is complete, bring back the original sign.

    // Store Bignum signs in a temporary variable as transposing of signs will happen, and the original sign is needed after addition.
    BIGNUM_SIGN num1Sign = num1->sign;
    BIGNUM_SIGN num2Sign = num2->sign;

    if (num1->sign == positive && num2->sign == negative) {
        num2->sign = num1Sign;
        addBignum(result, num1, num2);
        num2->sign = num2Sign;
        return;
    }
    if (num1->sign == negative && num2->sign == positive) { 
        num2->sign = num1Sign;
        addBignum(result, num1, num2);
        result->sign = num1Sign;
        num2->sign = num2Sign;
        return;
    }

    // Find minuend and subtrahend. Store in a temporary Bignum as minuend's Bignum.digits will be manipulated due to borrows. This is also needed as minuend can be either of the two Bignums (num1 or num2); If minuend is found. the other number will be the subtrahend.
    Bignum minuend = initBignum();
    Bignum subtrahend = initBignum();

    // Check length. Longer length will be set to the minuend and shorter will be the subtrahend.
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

    // Compare two Bignums. If minuend and subtrahend was not found in the previous conditions, i.e. they're of the same sign and length. Compare the two Bignums.
    // Still chain to the previous condition with else-if to prevent redundant condition checks when a minuend and subtrahend was found earlier.
    else if (isGreaterThanBignum(num1, num2) && minuend.length == 0) {
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

        // If num1 - num2, and both Bignums have the same sign, but num1 is less than num2, the result's sign will be the inverse of the sign of the 2 Bignums.
        // E.g.: (+30) - (+70) = -40 or (-30) - (-70) = 40 
        if (num1->sign == positive) {
            result->sign = negative;
        } else if (num1->sign == negative) {
            result->sign = positive;
        }
    } else if (isEqualToBignum(num1, num2)) {
        // If the two Bignums are equal to each other. Set result to zero.
        result->digits[0] = 0;
        result->length = 1;
        result->sign = positive;

        return;
    }

    // Once miunend and subtrahends are found, perform subtraction.

    // FEAT: A CONDITION TO CHECK IF A MINUEND OR SUBTRAHEND IS ACTUAL FOUND CAN BE IMPLEMENTED TO CONFIRM THAT THE FUNCTION CAN CONTINUE. IF IN ANY CASE IT REACHES THIS POINT BUT SOMEHOW HAVE NOT FOUND A MINUEND AND SUBTRAHEND: THROW AN ERROR.

    // Variable to store the difference of the individual digits of the minuend and subtrahend.
    int difference;
    unsigned long long int resultLength = 0;

    // Start at the LSD (least significant digit). Then iterate through, using the the min length (subtrahend length).
    for (int i = 0; i < subtrahend.length; i++) {
        // If current minuend digit is greater than the current subtrahend digit: No need to borrow.
        if (minuend.digits[i] > subtrahend.digits[i]) {
            // Get difference of the current minuend and subtrahend digits and increment the result's digit length counter.
            result->digits[i] = minuend.digits[i] - subtrahend.digits[i];
            resultLength++;
        } else if (minuend.digits[i] < subtrahend.digits[i]) {
            // If borrowing is needed, start from the next digit after the current index, and traverse through till the MSD (most significant digit or the last digit) of the minuend until you find a digit that can give you a borrow (anything greater than 0 can give a borrow).
            for (int j = i + 1; j < minuend.length; j++) {
                if (minuend.digits[j] > 0) {
                    // If a digit that can give a borrow is found, decrement that digit, then add 10 to the current index that needs a borrow.
                    minuend.digits[j] = minuend.digits[j] - 1;
                    minuend.digits[i] = minuend.digits[i] + 10;

                    // Since a borrow was found. Exit out of loop.
                    break;
                } else if (minuend.digits[j] == 0) {
                    // If the digit is a 0, replace that 0 with 9 and move to the next digit looking for a digit that can give a borrow. We can immediately give it a value of 9, since it is bound to be a 9 as it will be a 10 once a borrow is found, then it will be decremented as it needs to give a borrow to the digit to its right.
                    //
                    // E.g.:
                    // modified minuend:   1,2,9,9,11
                    // -------------------------------
                    // original minuend:   1,3,0,0, 1
                    //       subtrahend: -  , , ,2, 3
                    //                    ------------
                    //           result:    1,2,9,7,8
                    minuend.digits[j] = 9;
                }
            }

            // Once a borrow is found, get difference of the current minuend and subtrahend digits, store the result, and increment the result's digit length counter.
            result->digits[i] = minuend.digits[i] - subtrahend.digits[i];
            resultLength++;
        } else if (minuend.digits[i] == subtrahend.digits[i]) {
            // When minuend's and subtrahend's current digit is equal. Set result's current iteration digit to 0, as subtracting them will result in a 0.
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

    // Store the final length of result.
    result->length = resultLength;
    // Trim result. Removing any possible leading 0s
    trimBignum(result);
}


int multiplyBignumShiftLeft(Bignum *result, Bignum *num, unsigned long long int shiftPlaces) {
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

int multiplyBignumGetLeftHalf(Bignum *result, Bignum *num, unsigned long long int splitIndex) {
    if (splitIndex < 0) {
        printf("\nShifting Bignum by negative value.\n");
        return -2;
    }

    unsigned long long int resultLength = 0;

    for (unsigned long int i = splitIndex, j = 0; i < num->length; i++, j++) {
        result->digits[j] = num->digits[i];
        resultLength++;
    }

    result->length = resultLength;
}

int multiplyBignumGetRightHalf(Bignum *result, Bignum *num, unsigned long long int splitIndex) {
    if (splitIndex < 0) {
        printf("\nShifting Bignum by negative value.\n");
        return -2;
    }
    
    unsigned long long int resultLength = 0;

    for (unsigned long int i = 0; i < splitIndex; i++) {
        result->digits[i] = num->digits[i];
        resultLength++;
    }

    result->length = resultLength;
}

int multiplyBignum(Bignum *result, Bignum *x, Bignum *y) {
    // Function that multiplies 2 Bignums together.
    // Uses the karatsuba multiplication algorithm (https://www.youtube.com/watch?v=yWI2K4jOjFQ&t=6s) that has a time complexity of O(n^1.6). Which is faster than the traditional multiplication algorithm with a time complexity of O(n^2).

    // Base Case
    if (x->length == 1 || y->length == 1) {
        long long int xInt = bignumToInt(x);
        long long int yInt = bignumToInt(y);
        intToBignum(result, xInt * yInt, positive);
        return 0;
    }
    
    // Get maximum length (n) and half (n/2)
    unsigned long long int n = fmax(x->length, y->length);
    unsigned long long int half = floor(n / 2.0);

    // Initialize nessessary Bignums
    Bignum a = initBignum();
    Bignum b = initBignum();
    Bignum c = initBignum();
    Bignum d = initBignum();

    Bignum ac = initBignum();
    Bignum bd = initBignum();

    Bignum a_plus_b = initBignum();
    Bignum c_plus_d = initBignum();
    Bignum ac_minus_bd = initBignum();
    Bignum a_plus_b_times_c_plus_d = initBignum();
    Bignum a_plus_b_times_c_plus_d_minus_ac = initBignum();
    Bignum ad_plus_bc = initBignum();

    Bignum ac_left_shift = initBignum();
    Bignum ad_plus_bc_left_shift = initBignum();
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift = initBignum();
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd = initBignum();
    Bignum zero = initBignum();
    setBignum(&zero, "0", positive);

    // Split the multiplicand and multiplier.
    // E.g: x = 1234 & y = 4567
    // a = 12 | b = 34 | c = 45 | d = 67
    // Custom utility functions for multiplyBignum() are made to split x and y in half using the specified half. This is so that we don't half to use modulo and division in multiplyBignum().
    // 
    // a = x / (pow(10, half))
    multiplyBignumGetLeftHalf(&a, x, half);
    // b = x % (pow(10, half))
    multiplyBignumGetRightHalf(&b, x, half);
    // c = y / (pow(10, half))
    multiplyBignumGetLeftHalf(&c, y, half);
    // d = y % (pow(10, half))
    multiplyBignumGetRightHalf(&d, y, half);
    
    // Recursive call #1
    multiplyBignum(&ac, &a, &c);
    // Recursive call #2
    multiplyBignum(&bd, &b, &d);

    // Recursive call #3
    // ad_plus_bc = multiplyBignum(a+b, c+d) - ac - bd
    addBignum(&a_plus_b, &a, &b);
    addBignum(&c_plus_d, &c, &d);
    multiplyBignum(&a_plus_b_times_c_plus_d, &a_plus_b, &c_plus_d);

    // Collect results
    // result = ac * (pow(10, half * 2)) + (ad_plus_bc * (pow(10, half))) + bd
    subtractBignum(&a_plus_b_times_c_plus_d_minus_ac, &a_plus_b_times_c_plus_d, &ac);
    subtractBignum(&ad_plus_bc, &a_plus_b_times_c_plus_d_minus_ac, &bd);
    multiplyBignumShiftLeft(&ac_left_shift, &ac, half * 2);
    multiplyBignumShiftLeft(&ad_plus_bc_left_shift, &ad_plus_bc, half);
    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift, &ac_left_shift, &ad_plus_bc_left_shift);
    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &ac_left_shift_plus_ad_plus_bc_left_shift, &bd);
    addBignum(result, &ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &zero);

    // Determine sign of result using multiplication rules:
    // +x * +y = +
    // -x * -y = +
    // +x * -y = -
    // -x * +y = -
    if ((x->sign == positive && y->sign == positive) || x->sign == negative && y->sign == negative) {
        result->sign = positive;
    } else {
        result->sign = negative;
    }

    return 0;
}