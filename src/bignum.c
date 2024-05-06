#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include "bignum.h"



// -----------------GLOBAL VARIABLES-----------------

// Linked list to track the dynamically allocated Bignum.digits[]
typedef struct nodeBignum {
    Bignum *value;
    struct nodeBignum *next;
} BignumNode;
BignumNode *bignumListHead = NULL;

// Counter to keep track of the number of dynamically allocated Bignum.digits[].
unsigned long long int ALLOCATED_BIGNUMS_COUNT = 0, FREED_BIGNUMS_COUNT = 0;











// -----------------PRIVATE FUNCTIONS-----------------

int bignumShiftLeft(Bignum *result, Bignum *num, unsigned long long int shiftPlaces) {
    // Function that shifts a Bignum with the amount of 0s specified (x * pow(10, n)), without using multiplyBignum(). This has been tested to be faster especially when shifting by large place values.
    // I.e: x * 10^n
    // E.g: Integer: 123 -> 12300
    // E.g: Bignum: [3,2,1] -> [0,0,3,2,1]
    
    if (shiftPlaces < 0) {
        printf("Shifting Bignum by negative value/s.\n");
        return -1;
    }

    unsigned long long int resultLength = shiftPlaces;

    // Set the LSD (least significant digit) of result.digit to the amount of 0s specified.
    // REFACTOR: THIS EXPRESSION CAN BE REMOVED AS initBignum() SETS ALL ELEMENTS OF Bignum.digits[] TO 0. IF THIS REFACTOR FACTOR IS BEING APPLIED, DONT FORGET TO RETEST multiplyBignum()
    memset(result->digits, 0, sizeof(int) * shiftPlaces);

    // Copy the rest of num.digits to result.digits
    for (unsigned long int i = shiftPlaces, j = 0; j < num->length; i++, j++) {
        result->digits[i] = num->digits[j];
        
        resultLength++;
    }

    result->length = resultLength;
}

int multiplyBignumGetLeftHalf(Bignum *result, Bignum *num, unsigned long long int splitIndex) {
    // Function that gets a and c in multiplyBignum() that uses the karatsuba algorithm. This works in conjunction with multiplyBignumGetRightHalf() as they use the same splitIndex.
    
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
    // Function that gets a and c in multiplyBignum() that uses the karatsuba algorithm. This works in conjunction with multiplyBignumGetLeftHalf() as they use the same splitIndex
    
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

int bignumToBinary(Bignum *result, Bignum *num) {
    // Function to convert a Bignum(Base-10) into its Binary(Base-2) representation.
    // Uses simple divide and modulo to get the bits of the number, but uses a for-loop approach to avoid any Bignum arithmetic for maximum efficiency. 

    int remainderTemp;
    unsigned long long int binaryLength = 0;

    Bignum numTemp; 
    Bignum remainder; 
    Bignum two; 

    initBignum(&numTemp); 
    copyBignum(&numTemp, num);

    initBignum(&two); 
    intToBignum(&two, 2, positive);

    initBignum(&remainder); 

    // Iterate through the Bignum till Bignum is 0
    while(!isBignumZero(&numTemp)) {
        // Determine if the LSD (least significant digit) of the Bignum is odd or even. If even, the Bit will be 0, else it will be 1.
        if (numTemp.digits[0] % 2 == 0) {
            result->digits[binaryLength++] = 0;
        } else {
            result->digits[binaryLength++] = 1;
        }

        // Divide bignum by 2 using the halfBignum() function to avoid actual division operations
        halfBignum(&numTemp, &numTemp);
    }

    result->length = binaryLength;

    freeBignum(&numTemp); 
    freeBignum(&remainder); 
    freeBignum(&two); 

    return 0;
}

int millerRabinPrimalityTest(Bignum *num, int iterations) {
    // Function to test for a Bignum's primality using the Miller Rabin Primality Test given a specified number of iterations/tests

    Bignum pOne;
    initBignum(&pOne);
    setBignum(&pOne, "1", positive);
    Bignum two;
    initBignum(&two);
    setBignum(&two, "2", positive);

    Bignum numMinusOne;
    initBignum(&numMinusOne);
    subtractBignum(&numMinusOne, num, &pOne);

    Bignum numMinusOneCopy;
    initBignum(&numMinusOneCopy);
    copyBignum(&numMinusOneCopy, &numMinusOne);

    // STEP 1:
    while(numMinusOneCopy.digits[0] % 2 == 0) {
        halfBignum(&numMinusOneCopy, &numMinusOneCopy);
    }

    Bignum a;
    initBignum(&a);

    Bignum temp, tempCopy;
    initBignum(&temp);
    initBignum(&tempCopy);
    copyBignum(&temp, &numMinusOneCopy);
    
    Bignum mod;
    initBignum(&mod);

    Bignum modSquared;
    initBignum(&modSquared);

    for (int i = 0; i < iterations; i++) {
        resetBignum(&a);

        // STEP 2: Generate a random Bignum witness from: 2 - (num - 1). But in this implementation will be of a lower bit size to for faster tests.
        // This step can be more optimized to standard. Such as: (1) generating small and large numbers, (2) the number being odd, (3) Co-prime with num, and (4) the same random witness being used again for the test.
        unsigned long long int min = 2;
        unsigned long long int max = pow(10,ceil(numMinusOne.length / 7.0)) - 1;
        unsigned long long int aTemp = min + rand() % (max - min + 1);
        intToBignum(&a, aTemp, positive);

        // STEP 3: Perform test
        modularExponentiationBignum(&mod, &a, &temp, num);

        while(
            !isEqualToBignum(&temp, &numMinusOne) &&
            !isEqualToBignum(&mod, &pOne) &&
            !isEqualToBignum(&mod, &numMinusOne)
        ) {

            powerBignum(&modSquared, &mod, &two);
            moduloBignum(&mod, &modSquared, num);

            multiplyBignum(&tempCopy, &temp, &two);

            copyBignum(&temp, &tempCopy);
            resetBignum(&tempCopy);
        }

        // STEP 4: If the Bignum failed a test, immediately exit.
        if (
            !isEqualToBignum(&mod, &numMinusOne) &&
            temp.digits[0] % 2 == 0
        ) {

            freeBignum(&pOne);
            freeBignum(&two);
            freeBignum(&numMinusOne);
            freeBignum(&a);
            freeBignum(&numMinusOneCopy);
            freeBignum(&tempCopy);
            freeBignum(&temp);
            freeBignum(&mod);
            freeBignum(&modSquared);

            return 0;
        }
    }

    freeBignum(&pOne);
    freeBignum(&two);
    freeBignum(&numMinusOne);

    freeBignum(&a);

    freeBignum(&numMinusOneCopy);
    freeBignum(&tempCopy);
    freeBignum(&temp);

    freeBignum(&mod);
    freeBignum(&modSquared);

    return 1;
}












// -----------------PUBLIC FUNCTIONS-----------------

int getLengthOfInteger(long long int integer) {
    // Function will count the number of digits of positive or negative integer using log based solution. If integer is negative then convert it to positive as it the logarithm function only works with positive real numbers.
    if (integer == 0) {
        return 1;
    } else if (integer < 0) {
        integer = integer * -1;
    }

    return (int)log10((double)integer) + 1;
}



BignumNode* createNewBignumNode(Bignum *num) {
    BignumNode *node = (BignumNode*)malloc(sizeof(BignumNode));

    if (node == NULL) {
        printf("\n\nAllocated Bignum.digits[] is NULL!");
        printf("\n\tFunction: createNewBignumNode()");
        printf("\n\tExit code: -1\n\n\n");

        freeAllBignums();

        exit(-1);
    }

    node->value = num;
    node->next = NULL;
    return node;
}

void initBignum(Bignum *num) {
    // Function to initialize a Bignum to its default values and allocate an array for Bignum.digits[].
    // The allocated Bignum.digits[] will be tracked in a linked list, to be freed after using the Bignum.

    // Allocate memory for Bignum.digits[]. Use calloc() to set all indexes of the array to 0.
    BignumNode *bignumNode;
    int *digitsPtr = (int*)calloc(DEFAULT_BIGNUM_LENGTH, sizeof(int));

    if (digitsPtr == NULL) {
        printf("\n\nError allocating Bignum.digits...\n\n");
        exit(-1);
    }
    
    // Set the Bignum members to its default values
    num->digits = digitsPtr;
    num->length = 0;
    num->sign = positive;

    // Add Bignum's address to the linked list to track it
    bignumNode = createNewBignumNode(num);
    bignumNode->next = bignumListHead;
    bignumListHead = bignumNode;

    ALLOCATED_BIGNUMS_COUNT++;

}

void freeAllBignums() {
    // Function to go through linked list of Bignum pointers and free the allocated Bignum.digits[] and the nodes all at once.
    // Function will free nodes starting from the end. I.e: the most recent Bignums initialized.

    // Store head node in a temporary ndoe.
    BignumNode *tempHeadNode = bignumListHead;
    // Go through all nodes in linked list
    while(tempHeadNode != NULL) {

        if (bignumListHead->value == NULL) {
            printf("\nBignum in list is NULL");
            printf("\n\tFunction: freeAllBignums()");
        } else if (bignumListHead->value->digits == NULL) {
            printf("\nBignum.digits[] in list is NULL!\nAddress: %p->%p.%p", bignumListHead, bignumListHead->value, bignumListHead->value->digits);
            printf("\n\tFunction: freeAllBignums()");
        }

        // Point temporary head node to next node
        tempHeadNode = tempHeadNode->next;

        // Free current head node
        if (bignumListHead->value == NULL) {
            printf("\nBignum %p.%p is NULL",bignumListHead->value,bignumListHead->value->digits);
        }
        // Clear Bignum.digits[]
        memset(bignumListHead->value->digits, 0, sizeof(int) * DEFAULT_BIGNUM_LENGTH);
        // Free allcoated memory
        free(bignumListHead->value->digits);
        free(bignumListHead);

        // Store new head to original head node
        bignumListHead = tempHeadNode;

        FREED_BIGNUMS_COUNT++;
    }

}

void freeBignum(Bignum *num) {
    // Function to free a specified Bignum. This function should be used to free Bignums that are nested. Bignum must be freed immediately after use to minimize heap usage, and allow future Bignums to use the heap.

    // If only one node is in the linked list
    if (bignumListHead->next == NULL) {

        if (bignumListHead->value == NULL) {
            printf("\n\nBignum in node list is NULL!");
            printf("\n\tFunction: freeBignum()");
            printf("\n\tExit code: -1\n\n\n");

            freeAllBignums();

            exit(-1);
        } else if (bignumListHead->value != num) {
            printf("\n\nBignum.digits[] in node list doesn't match the target Bignum!");
            printf("\n\tFunction: freeBignum()");
            printf("\n\tExit code: -2\n\n\n");

            freeAllBignums();

            exit(-2);
        } else if (bignumListHead->value->digits == NULL) {
            printf("\n\nBignum.digits[] in node list is NULL!");
            printf("\n\tFunction: freeBignum()");
            printf("\n\tExit code: -3\n\n\n");

            freeAllBignums();

            exit(-3);
        } else if (bignumListHead->value->digits != num->digits) {
            printf("\n\nBignum was found in node list, but Bignum.digits[] addresses don't match!");
            printf("\n\tFunction: freeBignum()");
            printf("\n\tExit code: -4\n\n\n");

            freeAllBignums();

            exit(-4);
        }

        // Store head in a temporary node
        BignumNode *toRemoveNode = bignumListHead;
        // Clear Bignum.digits[]
        memset(toRemoveNode->value->digits, 0, sizeof(int) * DEFAULT_BIGNUM_LENGTH);
        // Free temporary node
        free(toRemoveNode->value->digits);
        toRemoveNode->value->digits = NULL;
        free(toRemoveNode);
        // Set head to NULL
        bignumListHead = NULL;

        FREED_BIGNUMS_COUNT++;

        return;
    }

    // If linked list is more than 1 node, but the target Bignum is at the head
    if (bignumListHead->value == num) {

        if (bignumListHead->value->digits == NULL) {
            printf("\n\nBignum.digits[] in node list is NULL!");
            printf("\n\tFunction: freeBignum()");
            printf("\n\tExit code: -5\n\n\n");

            freeAllBignums();

            exit(-5);
        } else if (bignumListHead->value->digits != num->digits) {
            printf("\n\nBignum was found in node list, but Bignum.digits[] addresses don't match!");
            printf("\n\tFunction: freeBignum()");
            printf("\n\tExit code: -6\n\n\n");

            freeAllBignums();

            exit(-6);
        }

        // Store the head in a temporary node
        BignumNode *toRemoveNode = bignumListHead;
        // Point head to the next node
        bignumListHead = bignumListHead->next;

        // Clear Bignum.digits[]
        memset(toRemoveNode->value->digits, 0, sizeof(int) * DEFAULT_BIGNUM_LENGTH);
        // Free target node
        free(toRemoveNode->value->digits);
        toRemoveNode->value->digits = NULL;
        free(toRemoveNode);

        FREED_BIGNUMS_COUNT++;

        return;
    }


    // If target node is not at the head, search the rest of the linked list. Use a previous node to track the node before the target Bignum, and a temporary node to check if the current node is the target Bignum.
    BignumNode *prevNode = bignumListHead;
    BignumNode *tempNode = prevNode->next;

    while(tempNode != NULL) {
        if (tempNode->value == num) {
            
            if (tempNode->value->digits == NULL) {
                printf("\n\nBignum.digits[] in node list is NULL!");
                printf("\n\tFunction: freeBignum()");
                printf("\n\tExit code: -7\n\n\n");

                freeAllBignums();

                exit(-7);
            } else if (tempNode->value->digits != num->digits) {
                printf("\n\nBignum was found in node list, but Bignum.digits[] addresses don't match!");
                printf("\n\tFunction: freeBignum()");
                printf("\n\tExit code: -8\n\n\n");

                freeAllBignums();

                exit(-8);
            }
            
            // Point previous node to the node that the target Bignum is pointing to
            prevNode->next = tempNode->next;

             // Clear Bignum.digits[]
            memset(tempNode->value->digits, 0, sizeof(int) * DEFAULT_BIGNUM_LENGTH);
            // Free the target node
            free(tempNode->value->digits);
            tempNode->value->digits = NULL;
            free(tempNode);

            FREED_BIGNUMS_COUNT++;

            return;
        }
        prevNode = tempNode;
        tempNode = tempNode->next;
    }

    printf("\n\nBIGNUM %p.%p WAS NOT FOUND IN LIST!\n\n", num, num->digits);
}

void printBignumNodeList() {
    // Function to print the linked list of Bignums

    BignumNode *tempNode = bignumListHead;

    if (bignumListHead == NULL) {
        printf("\n\nNode list empty!\n");
        return;
    }

    printf("\nHEAD -> ");
    while(tempNode != NULL) {
        printf("%p.%p -> ", tempNode->value, tempNode->value->digits);
        tempNode = tempNode->next;

        if (tempNode == NULL) {
            printf("NULL");
        }
    }
}

void setBignum(Bignum *numStruct, char numStr[], BIGNUM_SIGN sign) {
    // Main Function to set a Bignum. This function takes an integer represented as a string, as strings don't have a limit to how long it can be. The each character of the string will then be converted to an integer by offsetting its ASCII value, and will be pushed to Bignum.digits[].

    // Integer will be stored in Bignum.digits[] in reverse for the following reasons: (1) Most operations usually start from the LSD (least significant digit), making it easier to perform operations. (2) The result will usually be greater than or less than the 2 integers being operated on. Eg: 999 (3 digits) + 999 (3 digits) = 1998 (4 digits) & 999 (3 digits) * 999 (3 digits) = 998,001 (6 digits). Hence, the resulting Bignum is free to shrink and grow infinitely.

    // Reset passed Bignum to allow overwrites
    resetBignum(numStruct);

    int temp;

    // Store numStr length
    numStruct->length = strlen(numStr);

    // REFACTOR: Since the integer will be stored in reverse in Bignum.digits[], you can start to iterate through the string starting from the last character using strlen(numStr). Thus, only needing 1 for-loop to convert numStr.

    // Load numbers into Bignum.digits[]
    for (int i = 0; i < numStruct->length; i++) {
        // Offset ASCII value of the character by the ASCII value of '0'
        // Eg: '3' - '0' = 3  ->  51 - 48 = 3 
        int tempDigit =  numStr[i] - '0';

        // Check if an invalid character was passed in numStr. Only number characters '0'-'9' arr valid.
        // setBignum(&x, '123-456', positive); <- invalid
        // setBignum(&x, '-123456', negative); <- invalid
        // setBignum(&x, '123456', negative);  <- valid
        if (tempDigit < 0 || tempDigit > 9) {
            printf("\n\nPassed numStr in setBignum() is invalid. Please make sure the passed numStr contains only number characters '0'-'9'...");
            printf("\n\tFunction: setBignum()");
            printf("\n\tExit code: -1\n\n\n");

            freeAllBignums();

            exit(-1);
        }

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

    // Store sign enum (0 = positive || 1 = negative).
    // If Bignum is 0, and passed sign is negative, set sign to positive.
    // setBignum(&num, "0", negative); -> will be set to positive
    numStruct->sign = positive;
    numStruct->sign = isBignumZero(numStruct) ? positive : sign;
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
    // Function will convert a Bignum to an long long integer (maximum data type as negative Bignums can also be converted).

    // Get maximum number of digits of long long int. 
    int maxNumOfDigits = getLengthOfInteger(LLONG_MAX);

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

int resetBignum(Bignum *num) {
    // Function to reset the contents of a Bignum. Although initBignum() can be used to reset a Bignum, this function resets the Bignum, whilst maintaining the instance of the Bignum and the allocated Bignum.digits[]. initBignum() resets the Bignum by creating a new instance of the Bignum (Therefore another allocation will be made just to reset the Bignum).
    // This function should be used when Bignums are initialized or need to be reset inside of loops to minimize the amount of memory allocated.

    // Reset indexes of Bignum.digits[] to 0 
    memset(num->digits, 0, sizeof(int) * DEFAULT_BIGNUM_LENGTH);
    // Reset length
    num->length = 0;
    // Reset Sign
    num->sign = positive;
    return 0;
}

void copyBignum(Bignum *result, Bignum *num) {
    // Function to copy the contents of a Bignum to another Bignum. It will copy the whole Bignum.digits[] instead of copying uptil the length to overwrite and previous integers stored in Bignum.digits[]
    memcpy(result->digits, num->digits, sizeof(int) * DEFAULT_BIGNUM_LENGTH);
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

void printBignumExtended(Bignum *num, unsigned long long int specifiedLength) {
    // Function to print Bignum.digits[] up to a specified length. This can be used to check if any integers greater than 0 exist past Bignum.length
    if (num->sign == negative) {
        printf("-");
    }
    for (int i = specifiedLength - 1; i >= 0; i--) {
        if (i == num->length - 1) {
            printf("|");
        }
        printf("%d", num->digits[i]);
    }
}

void printBignumCenter(Bignum *num, unsigned int requiredWidth) {
    // Function to print a Bignum but with a specified width and center aligns it.
    // It prioritizes the required width over centeredness to stay consistent when used in formatted prints. Bignums with odd lengths won't be perfectly centered, so it either needs to have equal spaces on both sides (prioritizing centeredness) or 1 side having less space but meet the required width.
    // Eg: Bignum = -1234 with the required with of 10 spaces, will be __-1234___ not ___-1234___.

    // If bignum overflows the required width, just print Bignum.
    if (requiredWidth <= num->length) {
        printBignum(num);
        return;
    }

    // Calculate left and right widths.
    unsigned int leftWidth = (requiredWidth - num->length) / 2;
    unsigned int rightWidth = (requiredWidth - num->length) / 2;

    // Calculate total width to determine if we have exceeded or receded the required width.
    unsigned int totalWidth = leftWidth + rightWidth + num->length;

    // If bignum is negative, decrement left width as it will be printed later on.
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

void trimBignum(Bignum *num) {
    // Function to trim leading 0s of Bignum. This function works by counting leading 0s encapsulated by the current Bignum.length, and once a non-zero is found, will update the length of Bignum by adjusting Bignum.length.

    unsigned long long int numOfZeros = 0;
    // Flag to check if a non-zero integer was found. Cases where a Bignum was intentionally set to 0: setBignum(&x, "0", positive);. Should not be trimmed.
    int bignumIsZero = 1;

    // Start from the most significant digit, looking for 0s, and keep track of the number of 0s found.
    for (int i = num->length - 1; i >= 0; i--) {
        if (num->digits[i] == 0) {
            numOfZeros++;
        } 
        // If a non-zero integer is found, it has found the MSD(most significant digit), exit the loop and update Bignum.length.
        else {
            bignumIsZero = 0;
            break;
        }
    }

    // If Bignum is intentionally set to 0, don't trim the length and set it to 1.
    if (bignumIsZero) {
        num->length = 1;
        return;
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
    //    - A ZERO HAS THE SIGN OF POSITIVE IN Bignum STRUCT DEFINITION.
    //    - A BIGNUM THAT HASN'T BEEN PROPERLY INITIALIZED, (Ie: initBignum() was not used) WILL BE CONSIDERED AS A ZERO. DECIDE WHETHER THIS IS A WANTED BEHAVIOR

    // Go through each digit in Bignum.digits[] looking for a non-zero
    for (unsigned long long i = 0; i < DEFAULT_BIGNUM_LENGTH; i++) {
        if (num->digits[i] != 0) {
            return 0;
        }
    }

    return 1;
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

int isLessThanOrEqualToBignum(Bignum *num1, Bignum *num2) {
   // Function that will compare two Bignums, and determine if they are less than or equal to each other.

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

    // Worst-case, both Bignums have the same sign and length. Go through each digit of the two Bignums, starting from the MSD(most significant digit), comparing them. Do this until a difference is found.
    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] < num2->digits[i]) {
            return 1;
        }

        if (num1->digits[i] > num2->digits[i]) {
            return 0;
        }
    }

    // If no difference is found, it means that the two Bignums are equal. Return true(1)
    return 1;
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

int incrementBignum(Bignum *num, unsigned long long int incrementValue) {
    Bignum offset;
    initBignum(&offset);
    intToBignum(&offset, incrementValue, positive);
    addBignum(num, num, &offset);
    freeBignum(&offset);
    return 0;
}

int decrementBignum(Bignum *num, unsigned long long int decrementValue) {
    Bignum offset;
    initBignum(&offset);
    intToBignum(&offset, decrementValue, positive);
    subtractBignum(num, num, &offset);
    freeBignum(&offset);
    return 0;
}



void addBignum(Bignum *result, Bignum *addend1, Bignum *addend2) {
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

    // If you are adding a Bignum with 0, copy the other Bignum to result.
    if (isBignumZero(addend1)) {
        copyBignum(result, addend2);
        return;
    }
    if (isBignumZero(addend2)) {
        copyBignum(result, addend1);
        return;
    }

    // Use addition rule: if the two Bignums have different signs. Perform subtraction.
    if (addend1->sign != addend2->sign) {
        // Keep track of the original signs of the two Bignums. As they need to have the same sign to trigger subtraction in the subtractBignum() function. If they have contrasting signs, subtractBignum() will call addBignum() causing an infinite loop.
        BIGNUM_SIGN addend1Sign = addend1->sign;
        BIGNUM_SIGN addend2Sign = addend2->sign;

        addend1->sign = addend2->sign;

        if (isGreaterThanBignum(addend1, addend2)) {
            subtractBignum(result, addend1, addend2);
            // The resulting Bignum will have the sign of the bigger number(disregarding signs). E.g. 11 + (-5) = 6.
            result->sign = addend1Sign;
            // Bring back the original signs of the two Bignums.
            addend1->sign = addend1Sign;
            addend2->sign = addend2Sign;

            return;
        } else if (isLessThanBignum(addend1, addend2)) {
            subtractBignum(result, addend1, addend2);
            // The resulting Bignum will have the sign of the bigger number(disregarding signs). E.g. 5 + (-11) = -6.
            result->sign = addend2Sign;
            // Bring back the original signs of the two Bignums.
            addend1->sign = addend1Sign;
            addend2->sign = addend2Sign;

            return;
        } else if (isEqualToBignum(addend1, addend2)) {
            // If the 2 Bignums have different signs and are equal to each other. the result will be 0.
            result->digits[0] = 0;
            result->length = 1;
            result->sign = positive;

            return;
        }
    }

    // Use a temporary array to store results.digits[], then copy over the temporary array at the end of the function. This allows the function to accept a result Bignum that was passed as num1 or num2. This then will overwrite the passed result Bignum.
    // E.g: addbignum(&x, &x, &y)
    // The code above is equivalent to:
    // x = x + y; or x += y;
    int tempResultDigits[DEFAULT_BIGNUM_LENGTH];

    int sum;
    int carry = 0;
    unsigned long long int resultLength = 0;
    // Use usigned long long int to match the data type of the length property of Bignum struct.
    unsigned long long int maxLength;

    // Find the longest length. This is needed to determine which Bignum.length to use in the for-loop.
    if (addend1->length > addend2->length) {
        maxLength = addend1->length;
    } else if (addend2->length > addend1->length) {
        maxLength = addend2->length;
    } else {
        maxLength = addend1->length;
    }

    // Perform addition (this for-loop adds 1 digit at a time. Refer to the REFACTOR note above for more info).
    for (int i = 0; i < maxLength; i++) {
        // FEAT: IF ONE OF THE CURRENT DIGIT IS 0, YOU CAN DIRECTLY SET THE OTHER BIGNUM DIGIT TO CURRENT DIGIT IN RESULT TO SKIP OPERATIONS BEING DONE. 

        // Set sum to the previous carry
        sum = carry;
        // Add the digits of the addends together
        sum += addend1->digits[i] + addend2->digits[i];
        // If sum is more than 1 digit long, set carry to 1.
        carry = sum >= 10 ? 1 : 0;
        // Modulo sum by 10 to get only 1 digit to be pushed to Bignum result. This works whether sum is more than 1 digit long or not.
        // Eg: 19 % 10 = 9 or 9 % 10 = 9
        sum = sum % 10;
        // Store sum in result.digits[]
        tempResultDigits[i] = sum;
        // Increment resultLength to keep track of lenght of result Bignum
        resultLength++;
    }

    // If there is a remaining carry, append to result. e.g. 8 + 9 = 17 NOT 8 + 9 = 7 (the carry in the buffer was not added)
    if (carry == 1) {
        tempResultDigits[resultLength] = carry;
        resultLength++;
    }

    // Copy the temporary array to results.digits[]
    memcpy(result->digits, tempResultDigits, sizeof(int) * resultLength);

    // Store result digit length
    result->length = resultLength;
    // Copy sign of one of the Bignums. E.g. (+1) + (+2) = (+3) & (-1) + (-2) = (-3)
    result->sign = addend1->sign;
    // Trim result. Removing any possible leading 0s
    trimBignum(result);
}

void subtractBignum(Bignum *result, Bignum *minuend, Bignum *subtrahend) {
    // Function to subtract two Bignums together.
    // Uses basic subtraction which starts at the LSD (least significant digit) and subtracts the digits of the minuend and subtrahend together, iterating till it reaches the end. If a borrow is needed, it will loop through the next digits of the minuend until it finds a digit that can give a borrow.

    // NOTE: This function currently iterates and adds 1 digit at a time.

    // If you are subtracting a Bignum with 0, copy the other Bignum to result.
    if (isBignumZero(minuend)) {
        copyBignum(result, subtrahend);

        // Flip sign:
        // 0 - 1 = -1
        // 0 - (-1) = 1
        if (subtrahend->sign == positive) {
            result->sign = negative;
        } else if (subtrahend->sign == negative) {
            result->sign = positive;
        }

        return;
    }
    if (isBignumZero(subtrahend)) {
        copyBignum(result, minuend);
        return;
    }

    // NOTE: Function will check signs, length, and compare the two Bignums to determine twhat subtraction rule to follow.

    // Check signs. If signs are different, add the two numbers.

    // Transpose the signs: Make the two Bignums have the same sign, to trigger addition in the addBignum() function. Having different signs in addBignum() will call subtractBignum(), causing an infinite loop. Once addition is complete, bring back the original sign.

    // Store Bignum signs in a temporary variable as transposing of signs will happen, and the original sign is needed after addition.
    BIGNUM_SIGN minuendSign = minuend->sign;
    BIGNUM_SIGN num2Sign = subtrahend->sign;

    if (minuend->sign == positive && subtrahend->sign == negative) {
        subtrahend->sign = minuendSign;
        addBignum(result, minuend, subtrahend);
        subtrahend->sign = num2Sign;
        return;
    }
    if (minuend->sign == negative && subtrahend->sign == positive) { 
        subtrahend->sign = minuendSign;
        addBignum(result, minuend, subtrahend);
        result->sign = minuendSign;
        subtrahend->sign = num2Sign;
        return;
    }

    // Find minuend and subtrahend. Store in a temporary Bignum as minuend's Bignum.digits will be manipulated due to borrows. This is also needed as minuend can be either of the two Bignums (num1 or num2); If minuend is found. the other number will be the subtrahend.
    Bignum minuendTemp;
    Bignum subtrahendTemp;
    initBignum(&minuendTemp);
    initBignum(&subtrahendTemp);

    // Check length. Longer length will be set to the Temp and shorter will be the subtrahend.
    if (minuend->length > subtrahend->length) {
        minuendTemp.length = minuend->length;
        subtrahendTemp.length = subtrahend->length;

        memcpy(minuendTemp.digits, minuend->digits, sizeof(int) * minuend->length);
        memcpy(subtrahendTemp.digits, subtrahend->digits, sizeof(int) * subtrahend->length);

        result->sign = minuend->sign;
    } else if (minuend->length < subtrahend->length) {
        minuendTemp.length = subtrahend->length;
        subtrahendTemp.length = minuend->length;

        memcpy(minuendTemp.digits, subtrahend->digits, sizeof(int) * subtrahend->length);
        memcpy(subtrahendTemp.digits, minuend->digits, sizeof(int) * minuend->length);

        // If num1 - num2, and bot Bignums have the same sign, and num2 is longer than num1, the result's sign will be the inverse of the sign of the 2 Bignums.
        // E.g.: (+7) - (+10) = -3 or (-7) - (-10) = 3 
        if (minuend->sign == positive) {
            result->sign = negative;
        } else if (minuend->sign == negative) {
            result->sign = positive;
        }
    }

    // Compare two Bignums. If minuend and subtrahend was not found in the previous conditions, i.e. they're of the same sign and length. Compare the two Bignums.
    // Still chain to the previous condition with else-if to prevent redundant condition checks when a minuend and subtrahend was found earlier.
    else if (isGreaterThanBignum(minuend, subtrahend) && minuendTemp.length == 0) {
        minuendTemp.length = minuend->length;
        subtrahendTemp.length = subtrahend->length;

        memcpy(minuendTemp.digits, minuend->digits, sizeof(int) * minuend->length);
        memcpy(subtrahendTemp.digits, subtrahend->digits, sizeof(int) * subtrahend->length);

        result->sign = minuend->sign;
    }else if (isLessThanBignum(minuend, subtrahend) && minuendTemp.length == 0) {
        minuendTemp.length = subtrahend->length;
        subtrahendTemp.length = minuend->length;

        memcpy(minuendTemp.digits, subtrahend->digits, sizeof(int) * subtrahend->length);
        memcpy(subtrahendTemp.digits, minuend->digits, sizeof(int) * minuend->length);

        // If num1 - num2, and both Bignums have the same sign, but num1 is less than num2, the result's sign will be the inverse of the sign of the 2 Bignums.
        // E.g.: (+30) - (+70) = -40 or (-30) - (-70) = 40 
        if (minuend->sign == positive) {
            result->sign = negative;
        } else if (minuend->sign == negative) {
            result->sign = positive;
        }
        
    } else if (isEqualToBignum(minuend, subtrahend)) {
        // If the two Bignums are equal to each other. Set result to zero.
        result->digits[0] = 0;
        result->length = 1;
        result->sign = positive;

        freeBignum(&minuendTemp);
        freeBignum(&subtrahendTemp);

        return;
    }

    // Once miunend and subtrahends are found, perform subtraction.

    // FEAT: A CONDITION TO CHECK IF A MINUEND OR SUBTRAHEND IS ACTUAL FOUND CAN BE IMPLEMENTED TO CONFIRM THAT THE FUNCTION CAN CONTINUE. IF IN ANY CASE IT REACHES THIS POINT BUT SOMEHOW HAVE NOT FOUND A MINUEND AND SUBTRAHEND: THROW AN ERROR.

    // Use a temporary array to store results.digits[], then copy over the temporary array at the end of the function. This allows the function to accept a result Bignum that was passed as num1 or num2. This then will overwrite the passed result Bignum.
    // E.g: subtractbignum(&x, &x, &y)
    // The code above is equivalent to:
    // x = x - y; or x -= y;
    int tempResultDigits[DEFAULT_BIGNUM_LENGTH];
    unsigned long long int resultLength = 0;

    // Variable to store the difference of the individual digits of the minuend and subtrahend.
    int difference;

    // Start at the LSD (least significant digit). Then iterate through, using the the min length (subtrahend length).
    for (int i = 0; i < subtrahendTemp.length; i++) {
        // If current minuend digit is greater than the current subtrahend digit: No need to borrow.
        if (minuendTemp.digits[i] > subtrahendTemp.digits[i]) {
            // Get difference of the current minuend and subtrahend digits and increment the result's digit length counter.
            tempResultDigits[i] = minuendTemp.digits[i] - subtrahendTemp.digits[i];
            resultLength++;
        } else if (minuendTemp.digits[i] < subtrahendTemp.digits[i]) {
            // If borrowing is needed, start from the next digit after the current index, and traverse through till the MSD (most significant digit or the last digit) of the minuend until you find a digit that can give you a borrow (anything greater than 0 can give a borrow).
            for (int j = i + 1; j < minuendTemp.length; j++) {
                if (minuendTemp.digits[j] > 0) {
                    // If a digit that can give a borrow is found, decrement that digit, then add 10 to the current index that needs a borrow.
                    minuendTemp.digits[j] = minuendTemp.digits[j] - 1;
                    minuendTemp.digits[i] = minuendTemp.digits[i] + 10;

                    // Since a borrow was found. Exit out of loop.
                    break;
                } else if (minuendTemp.digits[j] == 0) {
                    // If the digit is a 0, replace that 0 with 9 and move to the next digit looking for a digit that can give a borrow. We can immediately give it a value of 9, since it is bound to be a 9 as it will be a 10 once a borrow is found, then it will be decremented as it needs to give a borrow to the digit to its right.
                    //
                    // E.g.:
                    // modified minuend:   1,2,9,9,11
                    // -------------------------------
                    // original minuend:   1,3,0,0, 1
                    //       subtrahend: -  , , ,2, 3
                    //                    ------------
                    //           result:    1,2,9,7,8
                    minuendTemp.digits[j] = 9;
                }
            }

            // Once a borrow is found, get difference of the current minuend and subtrahend digits, store the result, and increment the result's digit length counter.
            tempResultDigits[i] = minuendTemp.digits[i] - subtrahendTemp.digits[i];
            resultLength++;
        } else if (minuendTemp.digits[i] == subtrahendTemp.digits[i]) {
            // When minuend's and subtrahend's current digit is equal. Set result's current iteration digit to 0, as subtracting them will result in a 0.
            tempResultDigits[i] = 0;
            resultLength++;
        }
    }

    // If subtrahend is shorter than the minuend, and all neccessary subtractions are finished. Drop down/copy the remaining values of the minuend.
    if (minuendTemp.length > subtrahendTemp.length) {
        for (int i = subtrahendTemp.length; i < minuendTemp.length; i++) {
            tempResultDigits[i] = minuendTemp.digits[i];
            resultLength++;
        }
    }

    // Copy the temporary array to results.digits[]
    memcpy(result->digits, tempResultDigits, sizeof(int) * resultLength);

    // Store the final length of result.
    result->length = resultLength;
    // Trim result. Removing any possible leading 0s
    trimBignum(result);

    freeBignum(&minuendTemp);
    freeBignum(&subtrahendTemp);
}

int multiplyBignum(Bignum *result, Bignum *multiplicand, Bignum *multiplier) {
    // Function that multiplies 2 Bignums together.
    // Uses the karatsuba multiplication algorithm (https://www.youtube.com/watch?v=yWI2K4jOjFQ&t=6s) that has a time complexity of O(n^1.6). Which is faster than the traditional multiplication algorithm with a time complexity of O(n^2).

    // If you are subtracting a Bignum with 0, copy the other Bignum to result.
    if (isBignumZero(multiplicand)) {
        resetBignum(result);
        setBignum(result, "0", positive);
        return 0;
    }
    if (isBignumZero(multiplier)) {
        resetBignum(result);
        setBignum(result, "0", positive);
        return 0;
    }

    // Store multiplicand and multiplier sign in a temporary enum. Then make both Bignums positive to trigger the other function calls correctly.
    BIGNUM_SIGN tempMultiplicandSign = multiplicand->sign;
    BIGNUM_SIGN tempMultiplierSign = multiplier->sign;
    multiplicand->sign = positive;
    multiplier->sign = positive;

    // Base Case
    if (
        (multiplicand->length <= 9 || multiplier->length <= 9) &&
        (
            multiplicand->length < 9 && 
            multiplier->length < 9
        )
        ) {
        long long int multiplicandInt = bignumToInt(multiplicand);
        long long int multiplierInt = bignumToInt(multiplier);
        intToBignum(result, multiplicandInt * multiplierInt, positive);

        // Determine sign of result using multiplication rules:
        // +x * +y = +r
        // -x * -y = +r
        // +x * -y = -r
        // -x * +y = -r
        if (
        (tempMultiplicandSign == positive && tempMultiplierSign == positive) 
        || 
        (tempMultiplicandSign == negative && tempMultiplierSign == negative)) {
            result->sign = positive;
        } else {
            result->sign = negative;
        }

        // Bring back the original signs
        multiplicand->sign = tempMultiplicandSign;
        multiplier->sign = tempMultiplierSign;

        return 0;
    }


    
    // Determine the maximum length (n) and half (n/2) of multiplicand and multiplier.
    unsigned long long int n = fmax(multiplicand->length, multiplier->length);
    unsigned long long int half = floor(n / 2.0);

    // Initialize nessessary Bignums
    Bignum a;
    Bignum b;
    Bignum c;
    Bignum d;

    Bignum ac;
    Bignum bd;

    Bignum a_plus_b;
    Bignum c_plus_d;
    Bignum ac_minus_bd;
    Bignum a_plus_b_times_c_plus_d;
    Bignum a_plus_b_times_c_plus_d_minus_ac;
    Bignum ad_plus_bc;

    Bignum ac_left_shift;
    Bignum ad_plus_bc_left_shift;
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift;
    Bignum ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd;
    Bignum zero;

    initBignum(&a);
    initBignum(&b);
    initBignum(&c);
    initBignum(&d);

    initBignum(&ac);
    initBignum(&bd);

    initBignum(&a_plus_b);
    initBignum(&c_plus_d);
    initBignum(&ac_minus_bd);
    initBignum(&a_plus_b_times_c_plus_d);
    initBignum(&a_plus_b_times_c_plus_d_minus_ac);
    initBignum(&ad_plus_bc);

    initBignum(&ac_left_shift);
    initBignum(&ad_plus_bc_left_shift);
    initBignum(&ac_left_shift_plus_ad_plus_bc_left_shift);
    initBignum(&ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd);
    initBignum(&zero);

    setBignum(&zero, "0", positive);

    // Split the multiplicand and multiplier.
    // E.g: x = 1234 & y = 4567
    // a = 12 | b = 34 | c = 45 | d = 67
    // Custom utility functions that uses for-loops to split the Bignum are made to split the multiplicand and multiplier using the specified half (n/2). This is so that we don't have to use modulo and division in multiplyBignum().
    // 
    // a = x / (pow(10, half))
    multiplyBignumGetLeftHalf(&a, multiplicand, half);
    // b = x % (pow(10, half))
    multiplyBignumGetRightHalf(&b, multiplicand, half);
    // c = y / (pow(10, half))
    multiplyBignumGetLeftHalf(&c, multiplier, half);
    // d = y % (pow(10, half))
    multiplyBignumGetRightHalf(&d, multiplier, half);
    
    // Recursive call #1
    multiplyBignum(&ac, &a, &c);
    // Recursive call #2
    multiplyBignum(&bd, &b, &d);

    // Recursive call #3
    // ad_plus_bc = multiplyBignum(a+b, c+d) - ac - bd
    addBignum(&a_plus_b, &a, &b);
    addBignum(&c_plus_d, &c, &d);
    multiplyBignum(&a_plus_b_times_c_plus_d, &a_plus_b, &c_plus_d);
    subtractBignum(&a_plus_b_times_c_plus_d_minus_ac, &a_plus_b_times_c_plus_d, &ac);
    subtractBignum(&ad_plus_bc, &a_plus_b_times_c_plus_d_minus_ac, &bd);

    // Collect results
    // result = ac * (pow(10, half * 2)) + (ad_plus_bc * (pow(10, half))) + bd
    bignumShiftLeft(&ac_left_shift, &ac, half * 2);
    bignumShiftLeft(&ad_plus_bc_left_shift, &ad_plus_bc, half);
    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift, &ac_left_shift, &ad_plus_bc_left_shift);
    addBignum(&ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &ac_left_shift_plus_ad_plus_bc_left_shift, &bd);
    addBignum(result, &ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd, &zero);

    // Determine sign of result using multiplication rules:
    // +x * +y = +r
    // -x * -y = +r
    // +x * -y = -r
    // -x * +y = -r
    if (
    (tempMultiplicandSign == positive && tempMultiplierSign == positive) 
    || 
    (tempMultiplicandSign == negative && tempMultiplierSign == negative)) {
        result->sign = positive;
    } else {
        result->sign = negative;
    }

    // Bring back the original signs
    multiplicand->sign = tempMultiplicandSign;
    multiplier->sign = tempMultiplierSign;

    freeBignum(&a);
    freeBignum(&b);
    freeBignum(&c);
    freeBignum(&d);
    freeBignum(&ac);
    freeBignum(&bd);
    freeBignum(&a_plus_b);
    freeBignum(&c_plus_d);
    freeBignum(&ac_minus_bd);
    freeBignum(&a_plus_b_times_c_plus_d);
    freeBignum(&a_plus_b_times_c_plus_d_minus_ac);
    freeBignum(&ad_plus_bc);
    freeBignum(&ac_left_shift);
    freeBignum(&ad_plus_bc_left_shift);
    freeBignum(&ac_left_shift_plus_ad_plus_bc_left_shift);
    freeBignum(&ac_left_shift_plus_ad_plus_bc_left_shift_plus_bd);
    freeBignum(&zero);

    return 0;
}

int divideBignum(Bignum *result, Bignum *dividend, Bignum *divisor) {
    // Function that will find the quotient of two Bignums. Uses repeated multiplication to find the quotient of the dividend and divisor.
    // E.g: 111 / 20:
    //      20 * 1 = 20
    //      20 * 2 = 40
    //      20 * 3 = 60
    //      20 * 4 = 80
    //      20 * 5 = 100 <-- quotient
    //
    // It uses binary search to look for the quotient faster.
    // The left and right indexes will start from the estimated min and max number of digits of the quotient. The estimated number of digits of the quotient can be determined by: num of digits of divident - num of digits of divisor.
    // E.g:  123456  /   789    =    156
    //      6 Digits - 3 Digits = 3 Digits
    //                          => 1 * 10^(6 - (3 - 1)) = 10000 
    //    : The left and right indexes will be: 10 - 10000
    //
    // The left and right indexes are given extra place values as there are some cases where the quotient receeds or exceeds the estimated number of digits of the quotient.
    // E.g:    987   /    123   =    8
    //      3 Digits - 3 Digits = 0 Digit (quotient is actually 1 digits long not 0)
    //    : The left and right indexes will be: 1 - 10
    //              
    // E.g:  999999  /   100    =   9999
    //      6 Digits - 3 Digits = 3 Digit (quotient is actually 4 digits long not 3)
    //    : The left and right indexes will be: 10 - 10000 
    // Therefore, the left and right indexes should be given extra place values.

    // Declare temporary sign enums
    BIGNUM_SIGN dividendSign, divisorSign, resultSign;
    // Store the original sign of the dividend and divisor
    dividendSign = dividend->sign;
    divisorSign = divisor->sign;

    // Use division rule:
    //    +dividend / +divisor = +result
    //    -dividend / -divisor = +result
    //    -dividend / +divisor = -result
    //    +dividend / -divisor = -result
    // to determine the sign of the result
    if (dividendSign == divisorSign) {
        resultSign = positive;
    } else {
        resultSign = negative;
    }

    // Set the sign of the dividend and the divisor to be the same to correctly trigger the condition below
    dividend->sign = positive;
    divisor->sign = positive;

    // If dividend is less than the divisor. Quotient is 0.
    // 123 % 987654321 = 0
    if (isLessThanBignum(dividend, divisor)) {
        dividend->sign = dividendSign;
        divisor->sign = divisorSign;
        setBignum(result, "0", resultSign);
        return 0;
    }

    // A temporary Bignum set as 1 is used as bignumShiftLeft doesn't modify the actual Bignum that is passed. But modifies a separate resulting Bignum.
    Bignum tempOne;
    Bignum counterLeftIndex;
    Bignum counterRightIndex;
    Bignum counterMiddleIndex;

    initBignum(&tempOne);
    initBignum(&counterLeftIndex);
    initBignum(&counterRightIndex);
    initBignum(&counterMiddleIndex);

    setBignum(&tempOne, "1", positive);

    // Identify the left and right indexes
    unsigned long long int leftShiftBy = 0;
    unsigned long long int rightShiftBy = dividend->length - (divisor->length - 1);

    // If estimated left index is a valid index, use the formula. Else, use 0 to default the left index to 1.
    if ((divisor->length + 1) < dividend->length) {
        leftShiftBy = dividend->length - (divisor->length + 1);
    }

    // Get left and right Bignum indexes.
    // I.e: I.e: 1 * 10^leftShiftBy & 1 * 10^rightShiftBy
    bignumShiftLeft(&counterLeftIndex, &tempOne, leftShiftBy);
    bignumShiftLeft(&counterRightIndex, &tempOne, rightShiftBy);

    // Initialize multiplyResult which will be the Bignum that will hold the test quotient.
    // divisor * count = multiplyResult.
    Bignum multiplyResult;
    Bignum num1PlusNum2;
    Bignum dividendMinusMultiplyResult;
    initBignum(&multiplyResult);
    initBignum(&num1PlusNum2);
    initBignum(&dividendMinusMultiplyResult);

    setBignum(&multiplyResult, "0", positive);

    // Perform binary search to find the quotient.
    while(1) {
        // Reinitialize multiplyResult to reset its members.
        resetBignum(&multiplyResult);
        resetBignum(&num1PlusNum2);
        resetBignum(&dividendMinusMultiplyResult);


        // Get the middle index of the left and right index.
        // (L + R) / 2 = M
        addBignum(&num1PlusNum2, &counterLeftIndex, &counterRightIndex);
        halfBignum(&counterMiddleIndex, &num1PlusNum2);

        // Multiply the divisor with the middle index
        multiplyBignum(&multiplyResult, divisor, &counterMiddleIndex);
 
        // Determine which half, left or right, the quotient lies, and adjust the left and right indexes accordingly.
        if (isGreaterThanBignum(&multiplyResult, dividend)) {
            copyBignum(&counterRightIndex, &counterMiddleIndex);
        } else if (isLessThanBignum(&multiplyResult, dividend)) {
            copyBignum(&counterLeftIndex, &counterMiddleIndex);
        }

        // Check if the product is fit to be the quotient:
        // (Dividend - Product) < Divisor && (Dividend - Product) >= 0
        subtractBignum(&dividendMinusMultiplyResult, dividend, &multiplyResult);
        if (
            (isLessThanBignum(&dividendMinusMultiplyResult, divisor)) && 
            dividendMinusMultiplyResult.sign == positive
            ) {
            break;
        }

        // FEAT: ADD AN IF CASE WHERE THE LEFT INDEX IS GREATER THAN THE RIGHT INDEX, JUST LIKE IN OTHER BINARY SEARCH IMPLEMENTATIONS. HOWEVER, THIS CASE WHERE THIS FUNCTION DOESN'T FIND AN APPROPRIATE QUOTIENT, IS HYPOTHETICALLY IMPOSSIBLE DUE TO: (1) THE CONDITION ABOVE THAT CHECKS IF THE PRODUCT IS FIT TO BE A QUOTIENT, AND (2) ALL NUMBERS ARE PRESENT WITHIN THE RANGE. IF IN ANY CASE WHERE THE FUNCTION FAILS DUE TO THIS PROBLEM, THERE MUST BE A FLAW IN ANOTHER PART OF THIS FUNCTION. PERHAPS THE LEFT AND IRGHT INDEXES OR THE CONDITION THAT CHECKS IF THE PRODUCT IS A FIT QUOTIENT.
    }

    // Return the latest counterMiddleIndex that triggered the exit condition. i.e: the quotient
    copyBignum(result, &counterMiddleIndex);

    // Set the corresponding result sign, and bring back the original sign of the dividend and the divisor.
    result->sign = resultSign;
    dividend->sign = dividendSign;
    divisor->sign = divisorSign;

    freeBignum(&tempOne);
    freeBignum(&counterLeftIndex);
    freeBignum(&counterRightIndex);
    freeBignum(&counterMiddleIndex);
    freeBignum(&multiplyResult);
    freeBignum(&num1PlusNum2);
    freeBignum(&dividendMinusMultiplyResult);
}

int moduloBignum(Bignum *result, Bignum *dividend, Bignum *divisor) {
    // Function that will find the modulo of two Bignums. Uses repeated multiplication to find the quotient of the dividend and divisor. dividend - (quotient * divisor) will then give the remainder/modulo.
    // E.g: 111 / 20:
    //      20 * 1 = 20
    //      20 * 2 = 40
    //      20 * 3 = 60
    //      20 * 4 = 80
    //      20 * 5 = 100
    //      111 - (20 * 5) = 11 <-- remainder/modulo
    //
    // It uses binary search to look for the quotient for faster.
    // The left and right indexes will start from the estimated min and max number of digits of the quotient. The estimated number of digits of the quotient can be determined by: num of digits of divident - num of digits of divisor.
    // E.g:  123456  /   789    =    156
    //      6 Digits - 3 Digits = 3 Digits
    //                          => 1 * 10^(6 - (3 - 1)) = 10000 
    //    : The left and right indexes will be: 10 - 10000
    //
    // The left and right indexes are given extra place values as there are some cases where the quotient receeds or exceeds the estimated number of digits of the quotient.
    // E.g:    987   /    123   =    8
    //      3 Digits - 3 Digits = 0 Digit (quotient is actually 1 digits long not 0)
    //    : The left and right indexes will be: 1 - 10
    //              
    // E.g:  999999  /   100    =   9999
    //      6 Digits - 3 Digits = 3 Digit (quotient is actually 4 digits long not 3)
    //    : The left and right indexes will be: 10 - 10000 
    // Therefore, the left and right indexes should be given extra place values.

    // NOTE: This function only returns positive values and follows the Remainder Theorem

    // If dividend is a negative, perform the recursive call:
    //    -a mod b = b - (a mod b)
    // Reference: https://www.youtube.com/watch?v=AbGVbgQre7I
    if (dividend->sign == negative) {
        BIGNUM_SIGN tempDividendSign = dividend->sign;
        BIGNUM_SIGN tempDivisorSign = divisor->sign;

        dividend->sign = positive;
        divisor->sign = positive;

        Bignum tempMod;
        initBignum(&tempMod);
        // a mod b
        moduloBignum(&tempMod, dividend, divisor);
        // b - (a mod b)
        subtractBignum(result, divisor, &tempMod);

        dividend->sign = tempDividendSign;
        divisor->sign = tempDivisorSign;
        result->sign = positive;

        freeBignum(&tempMod);
        return 0;
    }

    // If dividend is less than the divisor. It is the remainder/modulo
    // 123 % 987654321 = 123
    if (isLessThanBignum(dividend, divisor)) {
        copyBignum(result, dividend);
        return 0;
    }

    BIGNUM_SIGN tempDividendSign = dividend->sign;
    BIGNUM_SIGN tempDivisorSign = divisor->sign;

    dividend->sign = positive;
    divisor->sign = positive;

    // A temporary Bignum set as 1 is used as bignumShiftLeft doesn't modify the actual Bignum that is passed. But modifies a separate resulting Bignum.
    Bignum tempOne;
    Bignum counterLeftIndex;
    Bignum counterRightIndex;
    Bignum counterMiddleIndex;

    initBignum(&tempOne);
    initBignum(&counterLeftIndex);
    initBignum(&counterRightIndex);
    initBignum(&counterMiddleIndex);

    setBignum(&tempOne, "1", positive);

    // Identify the left and right indexes.
    unsigned long long int leftShiftBy = 0;
    unsigned long long int rightShiftBy = dividend->length - (divisor->length - 1);

    // If estimated left index is a valid index, use the formula. Else, use 0 to default the left index to 1.
    if ((divisor->length + 1) < dividend->length) {
        leftShiftBy = dividend->length - (divisor->length + 1);
    }

    // Get left and right Bignum indexes
    // I.e: I.e: 1 * 10^leftShiftBy & 1 * 10^rightShiftBy
    bignumShiftLeft(&counterLeftIndex, &tempOne, leftShiftBy);
    bignumShiftLeft(&counterRightIndex, &tempOne, rightShiftBy);
    
    // Initialize multiplyResult which will be the Bignum that will hold the test quotient. I.e: divisor * count = multiplyResult.
    Bignum multiplyResult;
    Bignum num1PlusNum2;
    Bignum dividendMinusMultiplyResult;

    initBignum(&multiplyResult);
    initBignum(&num1PlusNum2);
    initBignum(&dividendMinusMultiplyResult);

    setBignum(&multiplyResult, "0", positive);



    // Perform binary search to find the quotient.
    while(1) {
        // Reinitialize multiplyResult to reset its members.
        resetBignum(&multiplyResult);

        // Get the middle index of the left and right index.
        // (L + R) / 2 = M
        resetBignum(&num1PlusNum2);

        addBignum(&num1PlusNum2, &counterLeftIndex, &counterRightIndex);
        halfBignum(&counterMiddleIndex, &num1PlusNum2);

        // Multiply the divisor with the middle index
        multiplyBignum(&multiplyResult, divisor, &counterMiddleIndex);

        // Determine which half, left or right, the quotient lies, and adjust the left and right indexes accordingly.  
        if (isGreaterThanBignum(&multiplyResult, dividend)) {
            copyBignum(&counterRightIndex, &counterMiddleIndex);
        } else if (isLessThanBignum(&multiplyResult, dividend)) {
            copyBignum(&counterLeftIndex, &counterMiddleIndex);
        }

        // Check if the product is fit to be the quotient.
        // I.e: (Dividend - Product) < Divisor && (Dividend - Product) >= 0
        resetBignum(&dividendMinusMultiplyResult);

        subtractBignum(&dividendMinusMultiplyResult, dividend, &multiplyResult);
        if (
            (isLessThanBignum(&dividendMinusMultiplyResult, divisor)) && 
            dividendMinusMultiplyResult.sign == positive
            ) {
            break;
        }

        // FEAT: ADD AN IF CASE WHERE THE LEFT INDEX IS GREATER THAN THE RIGHT INDEX, JUST LIKE IN OTHER BINARY SEARCH IMPLEMENTATIONS. HOWEVER, THIS CASE WHERE THIS FUNCTION DOESN'T FIND AN APPROPRIATE QUOTIENT, IS HYPOTHETICALLY IMPOSSIBLE DUE TO: (1) THE CONDITION ABOVE THAT CHECKS IF THE PRODUCT IS FIT TO BE A QUOTIENT, AND (2) ALL NUMBERS ARE PRESENT WITHIN THE RANGE. IF IN ANY CASE WHERE THE FUNCTION FAILS DUE TO THIS PROBLEM, THERE MUST BE A FLAW IN ANOTHER PART OF THIS FUNCTION. PERHAPS THE LEFT AND IRGHT INDEXES OR THE CONDITION THAT CHECKS IF THE PRODUCT IS A FIT QUOTIENT.
    }

    // Return the remainder/modulo:
    // dividend - (divisor * quotient) = remainder/modulo
    subtractBignum(result, dividend, &multiplyResult);

    dividend->sign = tempDividendSign;
    divisor->sign = tempDivisorSign;
    result->sign = positive;

    freeBignum(&tempOne);
    freeBignum(&counterLeftIndex);
    freeBignum(&counterRightIndex);
    freeBignum(&counterMiddleIndex);
    freeBignum(&multiplyResult);
    freeBignum(&num1PlusNum2);
    freeBignum(&dividendMinusMultiplyResult);
}

int powerBignum(Bignum *result, Bignum *base, Bignum *exponent) {
    // Function to calculate the power of a Bignum (x^n).
    // Uses binary exponentiation to calculate the power in O(log(n)) time complexity. Reference: https://www.youtube.com/watch?v=9VEqjAZxmeA&t=387s

    // The reference above worked on the exponent itself by modulo'ing the exponent by 2 to get the last binary bit of the number, then dividing the exponent by 2 to reduce the exponent. This function will instead, Convert the exponent to binary as the functions used in bignumToBinary() are faster than using moduloBignum() and divideBignum() to reduce the exponent; and iterate over the binary represented exponent, then perform the operations in the video.

    Bignum binaryExponent; 
    initBignum(&binaryExponent);

    // Convert the exponent to binary
    bignumToBinary(&binaryExponent, exponent);

    // Use temporary Bignums as multiplyBignum() can't overwrite the result Bignum.
    // E.g: multiplyBignum(base, base, base );  <- Bignum base won't be overwritten
    Bignum remainder;
    Bignum baseCopy;

    initBignum(&remainder);
    initBignum(&baseCopy);

    // Start remainder at 1
    setBignum(&remainder, "1", positive);
    copyBignum(&baseCopy, base);

    Bignum tempRemainder;
    Bignum tempBase;
    initBignum(&tempRemainder);
    initBignum(&tempBase);

    // Iterate through binaryExponent starting from the LSB (least significant bit)
    for (unsigned long long int i = 0; i < binaryExponent.length; i++) {
        resetBignum(&tempRemainder);
        resetBignum(&tempBase);

        // If current bit is 1:
        // remainder = remainder * base
        if (binaryExponent.digits[i] == 1) {
            multiplyBignum(&tempRemainder, &remainder, &baseCopy);
            copyBignum(&remainder, &tempRemainder);
        }

        // If the iteration is at the last bit of the binary, copy remainder to result. Next step is not needed:
        // Function will perform the Bignum operations below, exit the for-loop, then a copyBignum(result,&remainder) would be needed to be performed before exiting the function --Exiting early at the last bit will save 2 operations.
        if (i == binaryExponent.length - 1) {
            copyBignum(result, &remainder);

            freeBignum(&binaryExponent); 
            freeBignum(&remainder);
            freeBignum(&baseCopy);
            freeBignum(&tempRemainder);
            freeBignum(&tempBase);
            
            return 0;
        }
        
        // base = base * base
        multiplyBignum(&tempBase, &baseCopy, &baseCopy);
        copyBignum(&baseCopy, &tempBase);
    }

    freeBignum(&binaryExponent); 
    freeBignum(&remainder);
    freeBignum(&baseCopy);
    freeBignum(&tempRemainder);
    freeBignum(&tempBase);

    return 0;
}

int modularExponentiationBignum(Bignum *result, Bignum *base, Bignum *exponent, Bignum *divisor) {
    // Function to calculate ( (base^exponent) mod divisor ) of Bignums
    // Uses binary exponentiation, but with inserted modulos to calculate the modular exponentiation in O(log(n)) time complexity. Reference: https://www.youtube.com/watch?v=8r4-5k-o1QE

    // The function works a little differently from the usual binary exponentiation of using the base-10 representation of the exponent, and modulo-ing by 2 and halving it to get the binary bit to determine the steps. Instead it will first convert the exponent into its base-2 (binary) representation and start from the MSB to determine the steps needed, as shown in the reference above.

    Bignum binaryExponent; 
    initBignum(&binaryExponent);

    // COnvert the exponent to its base-2 (binary) representation
    bignumToBinary(&binaryExponent, exponent);

    Bignum remainder;
    initBignum(&remainder);
    // Copy the base to the remainder
    // The initial value of the remainder should be set to the value of the base
    copyBignum(&remainder, base);

    Bignum remainderSquared;
    Bignum remainderSquaredModDivisor;
    initBignum(&remainderSquared);
    initBignum(&remainderSquaredModDivisor);

    Bignum tempRemainder;
    initBignum(&tempRemainder);

    Bignum remainderSquaredModDivisorTimesBase;
    Bignum remainderSquaredModDivisorTimesBaseModDivisor;
    initBignum(&remainderSquaredModDivisorTimesBase);
    initBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

    // Iterate through the bits of the exponent, starting from the MSB (most significant bit)
    for (unsigned long long int i = binaryExponent.length - 1; i > 0; i--) {
        resetBignum(&remainderSquared);
        resetBignum(&remainderSquaredModDivisor);

        // If the next bit is 0:
        // remainder = (remainder ^ 2) mod divisor
        if (binaryExponent.digits[i - 1] == 0) {
            resetBignum(&tempRemainder);

            copyBignum(&tempRemainder, &remainder);

            multiplyBignum(&remainderSquared, &remainder, &remainder);
            moduloBignum(&remainderSquaredModDivisor, &remainderSquared, divisor);

            copyBignum(&remainder, &remainderSquaredModDivisor);
        }
        // If the next bit is 1:
        // remainder = (remainder ^ 2) mod divisor
        // remainder = ( remainder * base ) mod divisor
        else if (binaryExponent.digits[i - 1] == 1) {
            resetBignum(&remainderSquaredModDivisorTimesBase);
            resetBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

            multiplyBignum(&remainderSquared, &remainder, &remainder);
            moduloBignum(&remainderSquaredModDivisor, &remainderSquared, divisor);
            
            multiplyBignum(&remainderSquaredModDivisorTimesBase, &remainderSquaredModDivisor, base);
            moduloBignum(&remainderSquaredModDivisorTimesBaseModDivisor, &remainderSquaredModDivisorTimesBase,divisor);

            copyBignum(&remainder, &remainderSquaredModDivisorTimesBaseModDivisor);
        }
    }

    // The final value of the remainder after iterating through all of the bits of the exponent is the result of ( ( base^exponent ) mod divisor )
    copyBignum(result, &remainder);

    freeBignum(&binaryExponent);
    freeBignum(&remainder);
    freeBignum(&remainderSquared);
    freeBignum(&remainderSquaredModDivisor);
    freeBignum(&tempRemainder);
    freeBignum(&remainderSquaredModDivisorTimesBase);
    freeBignum(&remainderSquaredModDivisorTimesBaseModDivisor);

    return 0;
}

int modularInverseBignum(Bignum *result, Bignum *num, Bignum *divisor) {
    // Function to get the Modular Multiplicative Inverse of a Bignum.
    // Given num and divisor, find 'k'(result), such that: (num * k) mod divisor = 1
    // Uses the Extended Euclidean Algorithm to find 'k'. Reference: https://www.youtube.com/watch?v=lq285DDdmtw

    Bignum quotient, remainder;
    initBignum(&quotient);
    initBignum(&remainder);

    Bignum t, t1, t2;
    initBignum(&t);
    initBignum(&t1);
    initBignum(&t2);
    // Initial values of t1 and t2
    setBignum(&t1, "0", positive);
    setBignum(&t2, "1", positive);

    Bignum a, b;
    initBignum(&a);
    initBignum(&b);

    // Determine which is greater between num and divisor, let the greater one be 'a' and the other be 'b'
    copyBignum(&a, isGreaterThanBignum(num, divisor) ? num : divisor);
    copyBignum(&b, isLessThanBignum(num, divisor) ? num : divisor);

    Bignum t2TimesQuotient;
    initBignum(&t2TimesQuotient);

    // Iterate through till 'b' is 0
    while (!isBignumZero(&b)) {
        resetBignum(&remainder);
        resetBignum(&quotient);

        // remainder = a % b
        moduloBignum(&remainder, &a, &b);
        // quotient = a / b
        divideBignum(&quotient, &a, &b);

        // t = t1 - (t2 * quotient)
        multiplyBignum(&t2TimesQuotient, &t2, &quotient);
        subtractBignum(&t, &t1, &t2TimesQuotient);

        // Perform shifting operation
        // b -> a
        copyBignum(&a, &b);
        // remainder -> b
        copyBignum(&b, &remainder);
        // t2 -> t1
        copyBignum(&t1, &t2);
        // t2 -> t
        copyBignum(&t2, &t);
    }

    // Once 'b' is equal to 0, the value in t1 will be the modular inverse
    // If t1 is a negative Bignum, add the divisor to offset it to get the positive multiplicative inverse
    if (t1.sign == negative) {
        addBignum(result, &t1, divisor);
    } else {
        copyBignum(result, &t1);
    }

    freeBignum(&quotient);
    freeBignum(&remainder);
    freeBignum(&t);
    freeBignum(&t1);
    freeBignum(&t2);
    freeBignum(&a);
    freeBignum(&b);

    freeBignum(&t2TimesQuotient);

    return 0;
}



int halfBignum(Bignum *result, Bignum *num) {
    // Function that divides a Bignum by 2 without using divideBignum(). This is useful as this function is faster at dividing a Bignum by 2 compared to divideBignum() --especially when the Bignum being halved is a really large number.

    // NOTE: this function might be redundant, but it is needed in the algorithms used in the actual RSA Cipher Tool where it requires big Bignums to be divided by 2 effieciently.

    // NOTE: this function doesn't take into consideration the division sign rules. It will simply half the Bignum and the result will get the same sign as the Bignum being halved.

    // divideBignum() uses repeated multiplication and binary search. When a really big Bignum is being divided by 2, the left and right limits/indexes of the binary search grows, and more steps are needed. Moreover, using divideBignum() to half a really large Bignum somehow causes an overflow.
    // On the other hand, this function iterates through Bignum.digits[] and uses the native division on each digit. This has been tested to be faster than divideBignum().
    // E.g: 567 -> [  7  ,  6  ,  5  ]
    //              7/2     |     |
    //      floor->=2.5    6/2    |
    //             = 2    =3+5<- decimal from previous iteration
    //                    = 8     | 
    //                           5/2
    //                          =2.5
    //                          =2+0<- since no decimal in previous iteration, ad 0.
    //
    //            [  2  ,  8  ,  2  ]
    //  567 / 2 = 282
    //
    
    // A temporary Bignum is used so we can use this function to pass in a Bignum, and at the same time, pass that same Bignum to the function as a result, allowing for a proper overwrite.
    // E.g: halfBignum(&x, &x);
    // The function call above is equivalent to: x = x / 2; or x /= 2;
    int tempResultDigits[DEFAULT_BIGNUM_LENGTH];
    unsigned long long int resultLength = 0;

    int carry = 0;

    for (int i = num->length - 1; i >= 0; i--) {
        tempResultDigits[i] = (num->digits[i] / 2) + carry;

        if (num->digits[i] % 2 != 0) {
            carry = 5;
        } else {
            carry = 0;
        }
        
        resultLength++;
    }

    result->length = resultLength;
    memcpy(result->digits, tempResultDigits, sizeof(int) * resultLength);
    result->sign = num->sign;

    trimBignum(result);
}

int generatePrimeBignum(Bignum *result, unsigned long long int primeLength) {
    // Function to generate a prime Bignum given a desired length.
    // Function will generate random numbers, test for its primality using the Miller Rabin Primality Test (https://home.sandiego.edu/~dhoffoss/teaching/cryptography/10-Rabin-Miller.pdf), and if the number passes all the test, it will be as MOST PROBABLY a prime number.

    // NOTE: The Miller Rabin Primality Test can determine with certainty that a number is composite, but it can only provide a probabilistic assessment for its primality. In other words, there is still a small chance that a composite number might pass the test. Hence, increasing the number of iterations/tests done on a number can increase the probabilty of it being a prime number, but would cost some significant overhead.

    srand(time(NULL));

    Bignum n;
    initBignum(&n);

    // Seed the last digit of the random Bignum with 1,3,7,9 as these are the only numbers that a prime number can end with.
    int primeLastDigits[] = {1,3,7,9};
    int randPrimeLastDigitIndex = rand() % 4;
    int isPrime = 0;

    // Generate a new Bignum and test for primality until a Bignum has passed the primality test
    while(!isPrime) {
        resetBignum(&n);

        // Generate a random number per digit of the Bignum
        for (unsigned long long int i = primeLength - 1; i > 0; i--) {
            // If the current digit is the MSD (most significant digit), generate a random number from 1 - 9
            if (i == primeLength - 1) {
                n.digits[i] = (rand() % 9) + 1;
            }
            // Else generate a random number from 0 - 9
            else {
                n.digits[i] = rand() % 10;
            }
        }
        // Choose a random number from the prime last digit array to seed the last digit of the random Bignum
        n.digits[0] = primeLastDigits[randPrimeLastDigitIndex];
        n.length = primeLength;

        // Test for the Bignum's primality
        isPrime = millerRabinPrimalityTest(&n, 10);
    }

    // If the generated Bignum passed the primality test, copy that Bignum to result Bignum
    copyBignum(result, &n);

    freeBignum(&n);

    return 0;
}

