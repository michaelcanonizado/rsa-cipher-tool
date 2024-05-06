#ifndef BIGNUM_H
#define BIGNUM_H

#include <limits.h>

#define DEFAULT_BIGNUM_LENGTH 1000

typedef enum {
    positive = 0,
    negative = 1
} BIGNUM_SIGN;

typedef struct {
    // Integer will be in reverse for easier arithmetic
    int *digits;
    // Number of digits in integer
    // Bignum.length will cap out at around 18,446,744,073,709,551,615 (ref: C docs). Therefore the Bignum can only have a maximum of 18,446,744,073,709,551,615 digits.
    unsigned long long int length;
    // Sign will be 0 (positive) or 1 (negative), following the binary sign bit convention.
    BIGNUM_SIGN sign;
} Bignum;

int getLengthOfInteger(long long int num);

void initBignum(Bignum *num);
void freeAllBignums();
void freeBignum(Bignum *num);
void printBignumNodeList();
void setBignum(Bignum *numStruct, char numStr[], BIGNUM_SIGN sign);
void intToBignum(Bignum *numStruct, unsigned long long int integer, BIGNUM_SIGN sign);
long long int bignumToInt(Bignum *num);
int resetBignum(Bignum *num);
void copyBignum(Bignum *result, Bignum *num);
void printBignum(Bignum *num);
void printBignumExtended(Bignum *num, unsigned long long int specifiedLength);
void printBignumCenter(Bignum *num, unsigned int requiredWidth);
void trimBignum(Bignum *num);

int isGreaterThanBignum(Bignum *num1, Bignum *num2);
int isLessThanBignum(Bignum *num1, Bignum *num2);
int isLessThanOrEqualToBignum(Bignum *num1, Bignum *num2);
int isEqualToBignum(Bignum *num1, Bignum *num2);
int isBignumZero(Bignum *num);
int incrementBignum(Bignum *num, unsigned long long int incrementValue);
int decrementBignum(Bignum *num, unsigned long long int decrementValue);

void addBignum(Bignum *result, Bignum *addend1, Bignum *addend2);
void subtractBignum(Bignum *result, Bignum *minuend, Bignum *subtrahend);
int multiplyBignum(Bignum *result, Bignum *multiplicand, Bignum *multiplier);
int divideBignum(Bignum *result, Bignum *dividend, Bignum *divisor);
int moduloBignum(Bignum *result, Bignum *dividend, Bignum *divisor);
int powerBignum(Bignum *result, Bignum *base, Bignum *exponent);
int modularExponentiationBignum(Bignum *result, Bignum *base, Bignum *exponent, Bignum *divisor);
int modularInverseBignum(Bignum *result, Bignum *num, Bignum *divisor);

int halfBignum(Bignum *result, Bignum *num);
int generatePrimeBignum(Bignum *result, unsigned long long int primeLength);

#endif /* BIGNUM_H */