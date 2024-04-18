#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/bignum.h"

void generateKeys(Bignum *ePublic, Bignum *dPrivate, Bignum *nPublic);
void encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic);
void decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPublic, Bignum *nPublic);
void getInputFile(FILE **inputFilePtr, char *inputFilename);
void encryptText();
void getKeys(Bignum *ePublicOrDPrivate, Bignum *nPublic);

void getKeys(Bignum *ePublicOrDPrivate, Bignum *nPublic) {
    char key[5000];
    char firstKey[2500];
    char secondKey[2500];
    char flag = '.';
    int flagIndex;

    printf("\n");
    printf("\nPlease enter the private key: ");
    scanf("%s", key);

    while(1) {
        int flagCount = 0, i;

        for (i = 0; i < strlen(key); i++) {
            if (key[i] == flag) {
                flagIndex = i;
                flagCount++;
            }
        }

        if (flagCount == 1) {
            break;
        }

        printf("Invalid key! Please make sure you properly copied the key generated...");
        printf("\nPlease enter the private key: ");
        scanf("%s", key);
    }

    strncpy(firstKey, key, flagIndex);
    firstKey[flagIndex] = '\0';
    strcpy(secondKey, key + flagIndex + 1);
    // printf("String before '.': %s\n", firstKey);
    // printf("String after '.': %s\n", secondKey);

    setBignum(ePublicOrDPrivate, firstKey, positive);
    setBignum(nPublic, secondKey, positive);
}

void encryptText() {
    printf("\n.........................................\n");
    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    char outputFilename[] = "en.txt";

    getInputFile(&inputFilePtr, inputFilename);

    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        printf("Error opening output %s...\n", outputFilename);
        exit(1);
    }

    Bignum nPublic, ePublic;
    initBignum(&nPublic);
    initBignum(&ePublic);

    getKeys(&ePublic, &nPublic);

    printf("\nBignum key e: ");
    printBignum(&ePublic);
    printf("\nBignum key n: ");
    printBignum(&nPublic);

    printf("\n\nEncrypting %s...", inputFilename);

    encryptTextFile(inputFilePtr, outputFilePtr, &ePublic, &nPublic);

    freeAllBignums();

    fclose(inputFilePtr);
    fclose(outputFilePtr);
}

int main(void) {
    int userMenuState = 0;

    char *optionsArr[] = {"Generate Keys", "Encrypt Text", "Decrypt Text", "Exit"};

    do {
		for (int i = 0; i < sizeof(optionsArr)/sizeof(optionsArr[0]); i++) {
			printf("\n%d) - %s", i+1, optionsArr[i]);
		}
        printf("\nEnter number: ");
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
                encryptText();
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

    return 0;

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
    // encryptTextFile(inputFilePtr, outputFilePtr, &ePublic, &nPublic);
    printf("\n\nDecrypting...\n\n");
    // decryptTextFile(inputFilePtr, outputFilePtr, &dPrivate, &nPublic);
    
    fclose(inputFilePtr);
    fclose(outputFilePtr);

    freeAllBignums();

    return 0;
}

void getInputFile(FILE **inputFilePtr, char *inputFilename) {
    while (1) {
        printf("\nEnter the name of the input file: ");
        scanf("%s", inputFilename);

        *inputFilePtr = fopen(inputFilename, "r");

        if (*inputFilePtr != NULL) {
            break;
        } else {
            printf("Could not open \"%s\". Please try again...", inputFilename);
        }
    }

    printf("File opened successfully...");
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

void encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic) {
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

void decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPublic, Bignum *nPublic) {
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