#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "src/bignum.h"

#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif



int terminalWidth = 0;
int terminalHeight = 0;

typedef struct {
    char name[50];
    int size;
} KeySize;



#define PRINT_CENTER(width, str) ({ \
    int remainingWidth = terminalWidth - strlen(str); \
    int leftPadding = remainingWidth % 2 ? (remainingWidth + 1) / 2 : remainingWidth / 2; \
    int rightPadding = remainingWidth % 2 ? (remainingWidth - 1) / 2 : remainingWidth / 2; \
    printf("%*s%s%*s\n\n", leftPadding, "", str, rightPadding, ""); \
})

#define PRINT_FORMATS_CENTER(width, fmt, ...) ({ \
    int charCount = snprintf(NULL, 0, fmt, __VA_ARGS__); \
    int remainingWidth = width - charCount; \
    int leftPadding = remainingWidth % 2 ? (remainingWidth + 1) / 2 : remainingWidth / 2; \
    int rightPadding = remainingWidth % 2 ? (remainingWidth - 1) / 2 : remainingWidth / 2; \
    printf("%*s" fmt "%*s\n", leftPadding, "", \
           __VA_ARGS__, rightPadding, ""); \
})



void generateKeys();
void encryptText();
void decryptText();
void encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic);
void decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPrivate, Bignum *nPublic);
void getInputFile(FILE **inputFilePtr, char *inputFilename);
void getKeys(Bignum *ePublicOrDPrivate, Bignum *nPublic);

void clearScreen();
void getTerminalSize();
void moveCursor(int x, int y);


int main(void) {
    clearScreen();
    getTerminalSize();

    PRINT_FORMATS_CENTER(terminalWidth, "Width: %d", terminalWidth);
    PRINT_FORMATS_CENTER(terminalWidth, "Height: %d", terminalHeight);
    PRINT_CENTER(terminalWidth, "Height:asdasd");

    int userMenuState = 0;
    int adjustedHeight = terminalHeight / 3;
    int i;

    char *optionsArr[] = {"Generate Keys", "Encrypt Text", "Decrypt Text", "Exit"};

    do {
		for (i = 0; i < sizeof(optionsArr)/sizeof(optionsArr[0]); i++) {
            moveCursor(0, adjustedHeight + i);
			PRINT_FORMATS_CENTER(terminalWidth, "%d) - %s", i+1, optionsArr[i]);
            // moveCursor((terminalWidth - 14) / 2, adjustedHeight + i);
			// printf("%d) - %s", i+1, optionsArr[i]);
		}
		moveCursor((terminalWidth - 10)/ 2, adjustedHeight + (i + 1));
        printf("Enter number: ");
		scanf("%d", &userMenuState);

        switch (userMenuState) {
			case 1:
                generateKeys();
				break;
			case 2:
                encryptText();
				break;
			case 3:
                decryptText();
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
}

void generateKeys() {
    printf("\n.........................................");
    printf("\n");

    KeySize keySizeOptions[] = {
        {"16 bit", 16},
        {"32 bit", 32},
        {"64 bit", 64},
        {"128 bit", 128},
        {"256 bit", 256},
    };

    int chosenKeySize = 0;
    int pPrivateLength, qPrivateLength, ePublicLength;

    printf("\nPlease choose a key size: ");
    for (int i = 0; i < sizeof(keySizeOptions)/sizeof(keySizeOptions[0]); i++) {
        printf("\n%d) - %s", i+1, keySizeOptions[i].name);
    }
    printf("\nEnter number: ");
    scanf("%d", &chosenKeySize);
    chosenKeySize = keySizeOptions[chosenKeySize-1].size;
    printf("Chosen key size: %d", chosenKeySize);

    pPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));
    qPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));
    ePublicLength = pPrivateLength > 3 ? pPrivateLength / 2 : (chosenKeySize / log2(10.0)) - 1;
    printf("\np prime length: %d", pPrivateLength);
    printf("\nq prime length: %d", qPrivateLength);
    printf("\ne public length: %d\n", ePublicLength);

	Bignum nPublic, ePublic, dPrivate;
    initBignum(&nPublic);
    initBignum(&ePublic);
    initBignum(&dPrivate);

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

    Bignum plainChar, encryptedChar, decryptedChar;
    initBignum(&plainChar);
    initBignum(&encryptedChar);
    initBignum(&decryptedChar);
    setBignum(&plainChar, "2", positive);

    while (1) {
        // Generate p and q primes
        generatePrimeBignum(&pPrimePrivate, pPrivateLength);
        generatePrimeBignum(&qPrimePrivate, qPrivateLength);
        while (isEqualToBignum(&pPrimePrivate, &qPrimePrivate)) {
            generatePrimeBignum(&qPrimePrivate, qPrivateLength);
        }

        // Get n:
        // n = p * q
        multiplyBignum(&nPublic, &pPrimePrivate, &qPrimePrivate);

        // Get phi of n:
        // phi of n = (p - 1) * (q - 1)
        subtractBignum(&pPrimePrivateMinusOne, &pPrimePrivate, &one);
        subtractBignum(&qPrimePrivateMinusOne, &qPrimePrivate, &one);
        multiplyBignum(&phiOfNPrivate, &pPrimePrivateMinusOne, &qPrimePrivateMinusOne);

        // Generate e (public key):
        // 2 < e < phi of n
        generatePrimeBignum(&ePublic, ePublicLength);
    
        // Get d (private key):
        // (e * d)mod(n) = 1
        modularInverseBignum(&dPrivate, &ePublic, &phiOfNPrivate);

        printf("\n\nTesting keys: ");

        modularExponentiationBignum(&encryptedChar, &plainChar, &ePublic, &nPublic);
        modularExponentiationBignum(&decryptedChar, &encryptedChar, &dPrivate, &nPublic);

        printf("\nplain char: ");
        printBignum(&plainChar);
        printf("\nencrypted char: ");
        printBignum(&encryptedChar);
        printf("\ndecrypted char: ");
        printBignum(&decryptedChar);

        if (isEqualToBignum(&plainChar, &decryptedChar)) {
            break;
        }

        resetBignum(&nPublic);
        resetBignum(&ePublic);
        resetBignum(&dPrivate);
        resetBignum(&pPrimePrivate);
        resetBignum(&qPrimePrivate);
        resetBignum(&phiOfNPrivate);
        resetBignum(&pPrimePrivateMinusOne);
        resetBignum(&qPrimePrivateMinusOne);
        resetBignum(&encryptedChar);
        resetBignum(&decryptedChar);
    }

    printf("\n\nRESULTS: ");
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

    printf("\n\nPUBLIC KEY: ");
    printBignum(&ePublic);
    printf(".");
    printBignum(&nPublic);
    printf("\nPRIVATE KEY: ");
    printBignum(&dPrivate);
    printf(".");
    printBignum(&nPublic);
    printf("\n\n");

    freeAllBignums();
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

    printf("\n\nEncrypting %s...\n\n", inputFilename);

    encryptTextFile(inputFilePtr, outputFilePtr, &ePublic, &nPublic);

    freeAllBignums();

    fclose(inputFilePtr);
    fclose(outputFilePtr);
}

void decryptText() {
    printf("\n.........................................\n");
    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    char outputFilename[] = "dc.txt";

    getInputFile(&inputFilePtr, inputFilename);

    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        printf("Error opening output %s...\n", outputFilename);
        exit(1);
    }

    Bignum nPublic, dPrivate;
    initBignum(&nPublic);
    initBignum(&dPrivate);

    getKeys(&dPrivate, &nPublic);

    printf("\nBignum key d: ");
    printBignum(&dPrivate);
    printf("\nBignum key n: ");
    printBignum(&nPublic);

    printf("\n\ndecrypting %s...\n\n", inputFilename);

    decryptTextFile(inputFilePtr, outputFilePtr, &dPrivate, &nPublic);

    freeAllBignums();

    fclose(inputFilePtr);
    fclose(outputFilePtr);
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

void decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPrivate, Bignum *nPublic) {
    char encryptedCharacter[100];
    char decryptedCharacter;

    Bignum decryptedChar, encryptedChar;
    initBignum(&decryptedChar);
    initBignum(&encryptedChar);

    while (fscanf(inputFilePtr, "%[^/]/", encryptedCharacter) == 1) {
        setBignum(&encryptedChar, encryptedCharacter, positive);

        modularExponentiationBignum(&decryptedChar, &encryptedChar, dPrivate, nPublic);

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

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	printf("\033[2J");
	printf("\033[H");
#endif
}

void getTerminalSize() {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	terminalWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	terminalHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	terminalWidth = size.ws_col;
	terminalHeight = size.ws_row;
#endif
}

void moveCursor(int x, int y) {
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {x, y};
	SetConsoleCursorPosition(hConsole, pos);
#else
	printf("\033[%d;%dH", y, x);
#endif
}