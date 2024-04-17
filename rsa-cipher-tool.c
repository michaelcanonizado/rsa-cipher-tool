#include <stdio.h>
#include <string.h>
#include "src/bignum.h"

void generateKeys(Bignum *ePublic, Bignum *dPrivate, Bignum *nPublic);
void encryptMessage(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic);
void decryptMessage(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPublic, Bignum *nPublic);

int main(void) {
    int userMenuState = 0;

    char *optionsArr[] = {"Generate Keys", "Encrypt Text", "Decrypt Text", "Exit"};

    do {
		for (int i = 0; i < sizeof(optionsArr)/sizeof(optionsArr[0]); i++) {
			printf("\n%d) - %s", i+1, optionsArr[i]);
		}
        printf("Enter number: ");
		scanf("%d", &userMenuState);

        switch (userMenuState) {
			case 1:
                printf("\n.........................................");
                printf("\n");
				Bignum nPublic, ePublic, dPrivate;
                initBignum(&nPublic);
                initBignum(&ePublic);
                initBignum(&dPrivate);

                generateKeys(&ePublic, &dPrivate, &nPublic);

                freeAllBignums();
				break;
			case 2:
                printf("\n.........................................");
                printf("\n");
				printf("\nCase 2");
				break;
			case 3:
                printf("\n.........................................");
                printf("\n");
				printf("\nCase 3");
				break;
            case 4:
                freeAllBignums();
                return 0;
            default:
                break;
        }

        printf("\n\n.........................................");

    } while (userMenuState != sizeof(optionsArr)/sizeof(optionsArr[0]));

    FILE *inputFilePtr, *outputFilePtr;
    char inputFilename[] = "encrypted.txt";
    char outputFilename[] = "decrypted.txt";
    // char inputFilename[] = "plaintext.txt";
    // char outputFilename[] = "encrypted.txt";
    
    inputFilePtr = fopen(inputFilename, "r");
    outputFilePtr = fopen(outputFilename, "w");

    if (inputFilePtr == NULL) {
        printf("Error opening input %s...\n", inputFilename);
        return 1;
    }
    if (outputFilePtr == NULL) {
        printf("Error opening output %s...\n", outputFilename);
        return 2;
    }

    // printf("\n\nEncrypting...\n\n");
    // encryptMessage(inputFilePtr, outputFilePtr, &ePublic, &nPublic);
    printf("\n\nDecrypting...\n\n");
    // decryptMessage(inputFilePtr, outputFilePtr, &dPrivate, &nPublic);
    
    fclose(inputFilePtr);
    fclose(outputFilePtr);

    freeAllBignums();

    return 0;
}

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

void encryptMessage(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic) {
    char character;

    Bignum encryptedChar, plainChar;
    initBignum(&encryptedChar);
    initBignum(&plainChar);

    while ((character = fgetc(inputFilePtr)) != EOF) {
        intToBignum(&plainChar, character, positive);

        modularExponentiationBignum(&encryptedChar, &plainChar, ePublic, nPublic);

        printBignum(&encryptedChar);
        printf(",");

        for (unsigned long long int i = encryptedChar.length - 1; i > 0; i--) {
            fprintf(outputFilePtr, "%d", encryptedChar.digits[i]);
        }
        fprintf(outputFilePtr, "%d/", encryptedChar.digits[0]);

        resetBignum(&encryptedChar);
        resetBignum(&plainChar);
    };

    freeBignum(&encryptedChar);
    freeBignum(&plainChar);
}

void decryptMessage(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPublic, Bignum *nPublic) {
    char encryptedCharacter[100];
    char decryptedCharacter;

    Bignum decryptedChar, encryptedChar;
    initBignum(&decryptedChar);
    initBignum(&encryptedChar);

    while (fscanf(inputFilePtr, "%[^/]/", encryptedCharacter) == 1) {
        setBignum(&encryptedChar, encryptedCharacter, positive);

        modularExponentiationBignum(&decryptedChar, &encryptedChar, dPublic, nPublic);

        decryptedCharacter = bignumToInt(&decryptedChar);

        printf("%c", decryptedCharacter);

        fprintf(outputFilePtr, "%c", decryptedCharacter);

        resetBignum(&encryptedChar);
        resetBignum(&decryptedChar);
        encryptedCharacter[0] = '\0';
        decryptedCharacter = '\0';
    };

    freeBignum(&decryptedChar);
    freeBignum(&encryptedChar);
}