#include <stdio.h>
#include <string.h>
#include "src/bignum.h"

void encryptMessage(char *plaintext, Bignum *ePublic, Bignum *nPublic);

int main(void) {

    Bignum one;
    initBignum(&one);
    setBignum(&one, "1", positive);

    Bignum pPrimePrivate, qPrimePrivate;
    initBignum(&pPrimePrivate);
    initBignum(&qPrimePrivate);

    Bignum nPublic;
    initBignum(&nPublic);

    Bignum phiOfNPrivate, pPrimePrivateMinusOne, qPrimePrivateMinusOne;
    initBignum(&phiOfNPrivate);
    initBignum(&pPrimePrivateMinusOne);
    initBignum(&qPrimePrivateMinusOne);

    Bignum ePublic, dPrivate;
    initBignum(&ePublic);
    initBignum(&dPrivate);

    setBignum(&pPrimePrivate, "11", positive);
    setBignum(&qPrimePrivate, "13", positive);

    multiplyBignum(&nPublic, &pPrimePrivate, &qPrimePrivate);

    subtractBignum(&pPrimePrivateMinusOne, &pPrimePrivate, &one);
    subtractBignum(&qPrimePrivateMinusOne, &qPrimePrivate, &one);
    multiplyBignum(&phiOfNPrivate, &pPrimePrivateMinusOne, &qPrimePrivateMinusOne);

    setBignum(&ePublic, "7",positive);
    setBignum(&dPrivate, "103",positive);

    printf("\np: ");
    printBignum(&pPrimePrivate);
    printf("\nq: ");
    printBignum(&qPrimePrivate);
    printf("\nnPublic: ");
    printBignum(&nPublic);
    printf("\nphiOfN: ");
    printBignum(&phiOfNPrivate);
    printf("\ne: ");
    printBignum(&ePublic);
    printf("\nd: ");
    printBignum(&dPrivate);

    char plaintext[] = "Hello World!";

    printf("\n\nPlaintext: ");
    for (unsigned long long int i = 0; i < strlen(plaintext); i++) {
        printf("%c,", plaintext[i]);
    }
    printf("\nPlaintext: ");
    for (unsigned long long int i = 0; i < strlen(plaintext); i++) {
        printf("%d,", plaintext[i]);
    }
    printf("\nEncrypted text: ");
    encryptMessage(plaintext, &ePublic, &nPublic);

    freeAllBignums();

    return 0;
}


void encryptMessage(char *plaintext, Bignum *ePublic, Bignum *nPublic) {
    Bignum encryptedChar, plaintextChar;
    initBignum(&encryptedChar);
    initBignum(&plaintextChar);

    for (unsigned long long int i = 0; i < strlen(plaintext); i++) {
        intToBignum(&plaintextChar, plaintext[i], positive);

        modularExponentiationBignum(&encryptedChar, &plaintextChar, ePublic, nPublic);

        printBignum(&encryptedChar);
        printf(",");

        resetBignum(&encryptedChar);
        resetBignum(&plaintextChar);
    }

    freeBignum(&encryptedChar);
    freeBignum(&plaintextChar);
}