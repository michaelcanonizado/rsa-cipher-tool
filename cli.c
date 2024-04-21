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
int startingHeight = 0;

typedef struct {
    char name[50];
    int size;
} KeySize;



void generateKeys();
void encryptText();
void decryptText();
void encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic);
void decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPrivate, Bignum *nPublic);
void getInputFile(int promptLeftPadding, FILE **inputFilePtr, char *inputFilename);
void getKeys(int promptLeftPadding, Bignum *ePublicOrDPrivate, Bignum *nPublic);
void aboutProject();

void clearScreen();
void getTerminalSize();
void moveCursor(int x, int y);
void sleepProgram(int milliseconds);
void clearLines(int startLine, int endLine);
void clearWord(int y, int startCol, int endCol);
void promptExitConfirm();

int getMaxStringLengthInArray(char *stringsArr[], int stringsCount) {
    int maxLength = 0;

    for (int i = 0; i < stringsCount; i++) {
        int length = strlen(stringsArr[i]);
        if (length > maxLength) {
            maxLength = length;
        }
    }
    
    return maxLength;
}

int calculateLeftPadding(int strLength) {
    int remainingWidth = terminalWidth - strLength;
    int leftPadding = remainingWidth % 2 ? (remainingWidth + 1) / 2 : remainingWidth / 2;
    return leftPadding;
}


int main(void) {
    clearScreen();
    getTerminalSize();

    printf("\n%*sWidth: %d", calculateLeftPadding(strlen("Width")), "", terminalWidth);
	printf("\n%*sHeight: %d", calculateLeftPadding(strlen("Height")), "", terminalHeight);

    int userMenuState = 0;
    int i;

    char *optionsArr[] = {"Generate Keys", "Encrypt Text", "Decrypt Text", "About","Exit"};
    char promptStr[] = "Enter number: ";
    int optionsArrSize = sizeof(optionsArr)/sizeof(optionsArr[0]);
    int optionsLeftPadding = calculateLeftPadding(strlen(promptStr));

    do {
		for (i = 0; i < optionsArrSize; i++) {
            moveCursor(0, startingHeight + i);
			printf("\n%*s%d) - %s", optionsLeftPadding, "", i+1, optionsArr[i]);
		}

		moveCursor(0, startingHeight + (i + 2));
        printf("%*s%s", calculateLeftPadding(strlen(promptStr)), "", promptStr);
		scanf("%d", &userMenuState);
		while (getchar() != '\n');

        switch (userMenuState) {
			case 1:
                generateKeys();
                promptExitConfirm();
                clearScreen();
				break;
			case 2:
                encryptText();
                promptExitConfirm();
                clearScreen();
				break;
			case 3:
                clearScreen();
				break;
            case 4:
                clearScreen();
				break;
            case 5:
            	clearScreen();
				printf("Exiting program...\n");
                freeAllBignums();
				sleepProgram(500);
                return 0;
            default:
                clearScreen();
				printf("Please enter a number between 1 and 5.");
				sleepProgram(1000);
				clearScreen();
                break;
        }

    } while (userMenuState != optionsArrSize);

    return 0;
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
    startingHeight = terminalHeight / 3;
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

void sleepProgram(int milliseconds) {
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}

void clearLines(int startLine, int endLine) {
	for (int i = startLine; i <= endLine; i++) {
		moveCursor(0, i);
		for (int j = 0; j < terminalWidth; j++) {
			printf(" ");
		}
	}
}

void clearWord(int y, int startCol, int endCol) {
    moveCursor(startCol, y);
    for (int i = 0; i < endCol - startCol; i++) {
			printf(" ");
    }
}

void promptExitConfirm() {
	char userInput[100];

	do {
		clearLines(terminalHeight - 5, terminalHeight - 5);
		moveCursor((terminalWidth - 21)/ 2, terminalHeight - 5);
        // Ask user for confirmation
		printf("Enter DONE to go back: ");
		fgets(userInput, sizeof(userInput), stdin);

		// Replace the newline character at the end of the input to NULL
		if (userInput[strlen(userInput) - 1] == '\n') {
			userInput[strlen(userInput) - 1] = '\0';
		}

		// Convert the user's input to lowercase
		for(int i = 0; userInput[i]; i++){
			userInput[i] = tolower(userInput[i]);
		}

	} while (strcmp(userInput, "done") != 0);
}







void generateKeys() {
    clearScreen();

    KeySize keySizeOptions[] = {
        {"16 bit", 16},
        {"32 bit", 32},
        {"64 bit", 64},
        {"128 bit", 128},
        {"256 bit", 256},
    };
    char promptStr[] = "Enter number: ";

    int chosenKeySize = 0;
    int pPrivateLength, qPrivateLength, ePublicLength;

    printf("\nPlease choose a key size: ");
    for (int i = 0; i < sizeof(keySizeOptions)/sizeof(keySizeOptions[0]); i++) {
        moveCursor(0, startingHeight + i);
        printf("\n%*s%d) - %s", calculateLeftPadding(strlen(promptStr)), "", i+1, keySizeOptions[i].name);
    }

    printf("\n\n%*s%s", calculateLeftPadding(strlen(promptStr)), "", promptStr);
    scanf("%d", &chosenKeySize);
    chosenKeySize = keySizeOptions[chosenKeySize-1].size;

    clearScreen();
    moveCursor(0, startingHeight);
    printf("%*sGenerating %d bit keys...", calculateLeftPadding(strlen("Generating 256 bit keys...")), "", chosenKeySize);

    pPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));
    qPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));
    ePublicLength = pPrivateLength > 3 ? pPrivateLength / 2 : (chosenKeySize / log2(10.0)) - 1;
    // printf("\np prime length: %d", pPrivateLength);
    // printf("\nq prime length: %d", qPrivateLength);
    // printf("\ne public length: %d\n", ePublicLength);

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

        modularExponentiationBignum(&encryptedChar, &plainChar, &ePublic, &nPublic);
        modularExponentiationBignum(&decryptedChar, &encryptedChar, &dPrivate, &nPublic);

        // printf("\nplain char: ");
        // printBignum(&plainChar);
        // printf("\nencrypted char: ");
        // printBignum(&encryptedChar);
        // printf("\ndecrypted char: ");
        // printBignum(&decryptedChar);

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

    // printf("\n\nRESULTS: ");
    // printf("\np: ");
    // printBignum(&pPrimePrivate);
    // printf("\nq: ");
    // printBignum(&qPrimePrivate);
    // printf("\nnPublic: ");
    // printBignum(&nPublic);
    // printf("\nphiOfN: ");
    // printBignum(&phiOfNPrivate);
    // printf("\ne: ");
    // printBignum(&ePublic);
    // printf("\nd: ");
    // printBignum(&dPrivate);

    // printf("\n\nPUBLIC KEY: ");
    // printBignum(&ePublic);
    // printf(".");
    // printBignum(&nPublic);
    // printf("\nPRIVATE KEY: ");
    // printBignum(&dPrivate);
    // printf(".");
    // printBignum(&nPublic);
    // printf("\n\n");

    int keyPromptLength = 13 + dPrivate.length + nPublic.length;

    printf("\n\n%*s%s", calculateLeftPadding(keyPromptLength), "", "PUBLIC KEY: ");
    printBignum(&ePublic);
    printf(".");
    printBignum(&nPublic);

    printf("\n%*s%s", calculateLeftPadding(keyPromptLength), "", "PRIVATE KEY: ");
    printBignum(&dPrivate);
    printf(".");
    printBignum(&nPublic);

    freeAllBignums();
}

void encryptText() {
    clearScreen();

    int promptLeftPadding = calculateLeftPadding(strlen("Enter the name of the input file: "));

    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    char outputFilename[] = "en.txt";

    getInputFile(promptLeftPadding, &inputFilePtr, inputFilename);

    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        moveCursor(0, terminalHeight - 7);
        printf("%*sCould not open output file \"%s\". Please try again...", promptLeftPadding, "", inputFilename);

        exit(1);
    }

    Bignum nPublic, ePublic;
    initBignum(&nPublic);
    initBignum(&ePublic);

    getKeys(promptLeftPadding, &ePublic, &nPublic);

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

void getInputFile(int promptLeftPadding, FILE **inputFilePtr, char *inputFilename) {
    while (1) {
        moveCursor(0, startingHeight);
        printf("%*sEnter the name of the input file: ", promptLeftPadding, "");
        scanf("%s", inputFilename);

        *inputFilePtr = fopen(inputFilename, "r");

        if (*inputFilePtr != NULL) {
            break;
        } else {
            clearWord(startingHeight, promptLeftPadding + strlen("Enter the name of the input file: "), terminalWidth);

            moveCursor(0, terminalHeight - 7);
            printf("%*sCould not open \"%s\". Please try again...", promptLeftPadding, "", inputFilename);
        }
    }

    printf("%*sFile opened successfully...", promptLeftPadding, "");
}

void getKeys(int promptLeftPadding, Bignum *ePublicOrDPrivate, Bignum *nPublic) {
    char key[5000];
    char firstKey[2500];
    char secondKey[2500];
    char flag = '.';
    int flagIndex;

    printf("\n");
    printf("%*sPlease enter the private key: ", promptLeftPadding, "");
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



