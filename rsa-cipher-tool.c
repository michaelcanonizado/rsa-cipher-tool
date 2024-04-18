#include <stdio.h>
#include <string.h>
#include "src/bignum.h"

void encryptMessage(char *plaintext, Bignum *ePublic, Bignum *nPublic);
void decryptMessage(int encryptedText[], Bignum *dPublic, Bignum *nPublic);

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

    setBignum(&pPrimePrivate, "92726898528067157119", positive);
    setBignum(&qPrimePrivate, "59596883466967193887", positive);
    // setBignum(&pPrimePrivate, "11", positive);
    // setBignum(&qPrimePrivate, "13", positive);

    multiplyBignum(&nPublic, &pPrimePrivate, &qPrimePrivate);

    subtractBignum(&pPrimePrivateMinusOne, &pPrimePrivate, &one);
    subtractBignum(&qPrimePrivateMinusOne, &qPrimePrivate, &one);
    multiplyBignum(&phiOfNPrivate, &pPrimePrivateMinusOne, &qPrimePrivateMinusOne);

    setBignum(&ePublic, "96139263103317626603",positive);
    
    modularInverseBignum(&dPrivate, &ePublic, &phiOfNPrivate);

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
    int encryptedText[] = {19,62,4,4,45,98,87,45,49,4,100,110};

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

    printf("\nDecrypted text: ");
    decryptMessage(encryptedText, &dPrivate, &nPublic);

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

void decryptMessage(int encryptedText[], Bignum *dPublic, Bignum *nPublic) {
    Bignum decryptedChar, encryptedTextChar;
    initBignum(&decryptedChar);
    initBignum(&encryptedTextChar);

    for (unsigned long long int i = 0; i < 12; i++) {
        intToBignum(&encryptedTextChar, encryptedText[i], positive);

        modularExponentiationBignum(&decryptedChar, &encryptedTextChar, dPublic, nPublic);

        printBignum(&decryptedChar);
        printf(",");

        resetBignum(&decryptedChar);
        resetBignum(&encryptedTextChar);
    }

    freeBignum(&decryptedChar);
    freeBignum(&encryptedTextChar);
}