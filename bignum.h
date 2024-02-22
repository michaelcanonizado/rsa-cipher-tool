#ifndef BIGNUM_H
#define BIGNUM_H

#define MAX_INT_LENGTH 10000

typedef enum {
    positive = 0,
    negative = 1
} BIGNUM_SIGN;

typedef struct {
    // Integer will be in reverse for easier arithmetic
    int digits[MAX_INT_LENGTH];
    // Number of digits in integer
    int length;
    // Sign will be 0 (positive) or 1 (negative), following the binary sign bit convention.
    BIGNUM_SIGN sign;
} Bignum;

Bignum initBignum();
void setBignum(Bignum *numStruct, char numStr[], BIGNUM_SIGN sign);
void intToBignum(Bignum *numStruct, unsigned long long int integer);
long long int bignumToInt(Bignum *num);
void trimBignum(Bignum *num);
int isGreaterThanBignum(Bignum *num1, Bignum *num2);
int isLessThanBignum(Bignum *num1, Bignum *num2);
int isEqualToBignum(Bignum *num1, Bignum *num2);
void addBignum(Bignum *result, Bignum *num1, Bignum *num2);
void subtractBignum(Bignum *result, Bignum *num1, Bignum *num2);

#endif /* BIGNUM_H */