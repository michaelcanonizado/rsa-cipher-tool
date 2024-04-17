#include <stdio.h>
#include <string.h>
#include "src/bignum.h"

void generateKeys(Bignum *ePublic, Bignum *dPrivate, Bignum *nPublic);
void encryptMessage(char *plaintext, Bignum *ePublic, Bignum *nPublic);
void decryptMessage(int encryptedText[], Bignum *dPublic, Bignum *nPublic);

void generateKeys(Bignum *ePublic, Bignum *dPrivate, Bignum *nPublic) {
    Bignum one;
    initBignum(&one);
    setBignum(&one, "1", positive);

    Bignum pPrimePrivate, qPrimePrivate;
    initBignum(&pPrimePrivate);
    initBignum(&qPrimePrivate);

    Bignum phiOfNPrivate, pPrimePrivateMinusOne, qPrimePrivateMinusOne;
    initBignum(&phiOfNPrivate);
    initBignum(&pPrimePrivateMinusOne);
    initBignum(&qPrimePrivateMinusOne);

    // Generate p and q primes
    setBignum(&pPrimePrivate, "11", positive);
    setBignum(&qPrimePrivate, "13", positive);

    // Get n:
    // n = p * q
    multiplyBignum(nPublic, &pPrimePrivate, &qPrimePrivate);

    // Get phi of n:
    // phi of n = (p - 1) * (q - 1)
    subtractBignum(&pPrimePrivateMinusOne, &pPrimePrivate, &one);
    subtractBignum(&qPrimePrivateMinusOne, &qPrimePrivate, &one);
    multiplyBignum(&phiOfNPrivate, &pPrimePrivateMinusOne, &qPrimePrivateMinusOne);

    // Generate e (public key):
    // 2 < e < phi of n
    setBignum(ePublic, "7",positive);
    
    // Get d (private key):
    // (e * d)mod(n) = 1
    modularInverseBignum(dPrivate, ePublic, &phiOfNPrivate);

    printf("\np: ");
    printBignum(&pPrimePrivate);
    printf("\nq: ");
    printBignum(&qPrimePrivate);
    printf("\nnPublic: ");
    printBignum(nPublic);
    printf("\nphiOfN: ");
    printBignum(&phiOfNPrivate);
    printf("\ne: ");
    printBignum(ePublic);
    printf("\nd: ");
    printBignum(dPrivate);

    freeBignum(&one);
    freeBignum(&pPrimePrivate);
    freeBignum(&qPrimePrivate);
    freeBignum(&phiOfNPrivate);
    freeBignum(&pPrimePrivateMinusOne);
    freeBignum(&qPrimePrivateMinusOne);
}

int main(void) {
    Bignum nPublic, ePublic, dPrivate;
    initBignum(&nPublic);
    initBignum(&ePublic);
    initBignum(&dPrivate);

    generateKeys(&ePublic, &dPrivate, &nPublic);

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