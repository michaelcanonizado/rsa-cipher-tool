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
int currLeftPadding = 0;
int prevCursorX = 0;
int prevCursorY = 0;

typedef struct {
    char name[50];
    int size;
} KeySize;

typedef enum {
    encrypt = 0,
    decrypt = 1
} Action;



void generateKeys();
void encryptText();
void decryptText();
void encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic);
void decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPrivate, Bignum *nPublic);
void getInputFile(FILE **inputFilePtr, char *inputFilename);
void getKeys(Action type, Bignum *ePublicOrDPrivate, Bignum *nPublic);
void aboutProject();

void clearScreen();
void clearPrompts();
void getTerminalSize();
void moveCursor(int x, int y);
void getCursorPosition(int *x, int *y);
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

void printProgramHeader() {
    moveCursor(0,0);
    printf("\n");
    for (int i = 0; i < terminalWidth; i++) printf("-");

    printf("\n%*sRSA Cipher Tool", calculateLeftPadding(strlen("RSA Cipher Tool")), "");

    printf("\n");
    for (int i = 0; i < terminalWidth; i++) printf("-");
    printf("\n");
}

void printLineBreak() {
    printf("\n");
    for (int i = 0; i < terminalWidth; i++) printf("-");
}

int main(void) {
    clearScreen();
    getTerminalSize();

    int userMenuState = 0;
    int i;

    char *optionsArr[] = {"Generate Keys", "Encrypt Text", "Decrypt Text", "About","Exit"};
    int optionsArrSize = sizeof(optionsArr)/sizeof(optionsArr[0]);

    do {
        printProgramHeader();

        printf("\nWhat do you want to do?\n");
		for (i = 0; i < optionsArrSize; i++) {
			printf("\n%d) - %s", i+1, optionsArr[i]);
		}

        printf("\n\nEnter number: ");
		scanf("%d", &userMenuState);
		while (getchar() != '\n');

        if (userMenuState > 0 && userMenuState <= optionsArrSize) {
            switch (userMenuState) {
			    case 1:
                    clearPrompts();
                    generateKeys();
                    clearPrompts();
			    	break;
			    case 2:
                    clearPrompts();
                    encryptText();
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
        }

        clearPrompts();

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

void clearPrompts() {
    // moveCursor(0, 5);
    // for (int i = 5; i < terminalWidth; i++) {
    //     for (int j = 0; j < terminalHeight/2; j += 27) {
	// 	    printf("                            ");
    //     }
    // }
    // moveCursor(0, 5);
    clearScreen();
    printProgramHeader();
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

void getCursorPosition(int *x, int *y) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    *x = info.dwCursorPosition.X;
    *y = info.dwCursorPosition.Y;
#else
#endif
}

void moveCursor(int x, int y) {
#ifdef _WIN32
    getCursorPosition(&prevCursorX, &prevCursorY);
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
    moveCursor(0, 4);

    KeySize keySizeOptions[] = {
        {"16 bit", 16},
        {"32 bit", 32},
        {"64 bit", 64},
        {"128 bit", 128},
        {"256 bit", 256},
        {"Back", 0},
    };
    int keySizeOptionsSize = sizeof(keySizeOptions)/sizeof(keySizeOptions[0]);

    int chosenKeySize = 0;
    int pPrivateLength, qPrivateLength, ePublicLength;

    while(1) {
        printf("\nPlease choose a key size:\n");
        for (int i = 0; i < keySizeOptionsSize; i++) {
            printf("\n%d) - %s", i+1, keySizeOptions[i].name);
        }

        printf("\n\nEnter number: ");
        scanf("%d", &chosenKeySize);
		while (getchar() != '\n');

        if (chosenKeySize > 0 && chosenKeySize <= keySizeOptionsSize) {
            chosenKeySize = keySizeOptions[chosenKeySize-1].size;
            break;
        }

        clearPrompts();
    }

    if (chosenKeySize == 0) {
        return;
    }

    clearPrompts();
    printf("\nKey length: %d bit",  chosenKeySize);
    printf("\nGenerating: ");
    printf("[=========================================] (100%%)");

    pPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));
    qPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));
    ePublicLength = pPrivateLength > 3 ? pPrivateLength / 2 : (chosenKeySize / log2(10.0)) - 1;

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

    int keyPromptLength = 13 + dPrivate.length + nPublic.length;

    printf("\n");
    // printLineBreak();

    printf("\nPUBLIC KEY: ");
    printBignum(&ePublic);
    printf(".");
    printBignum(&nPublic);

    printf("\nPRIVATE KEY: ");
    printBignum(&dPrivate);
    printf(".");
    printBignum(&nPublic);

    // printLineBreak();

    printf("\n\nRemember: please make sure to safely secure and properly copy the keys above!");

    freeAllBignums();

    promptExitConfirm();
}

void encryptText() {
    clearPrompts();

    currLeftPadding = calculateLeftPadding(strlen("Enter the name of the input file: "));

    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    char outputFilename[] = "en.txt";

    getInputFile(&inputFilePtr, inputFilename);

    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        printf("Could not open output file \"%s\". Please try again...", inputFilename);
        exit(1);
    }

    Bignum nPublic, ePublic;
    initBignum(&ePublic);
    initBignum(&nPublic);

    getKeys(encrypt, &ePublic, &nPublic);
    printf("\nEncryption progress: [=======================================] (100%%)");

    encryptTextFile(inputFilePtr, outputFilePtr, &ePublic, &nPublic);

    printf("\nEncryption complete!");

    freeAllBignums();

    fclose(inputFilePtr);
    fclose(outputFilePtr);

    promptExitConfirm();
}

void encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic) {
    char character;

    Bignum encryptedChar, plainChar;
    initBignum(&encryptedChar);
    initBignum(&plainChar);

    while ((character = fgetc(inputFilePtr)) != EOF) {
        intToBignum(&plainChar, character, positive);

        modularExponentiationBignum(&encryptedChar, &plainChar, ePublic, nPublic);

        // printBignum(&encryptedChar);
        // printf(",");

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

void getInputFile(FILE **inputFilePtr, char *inputFilename) {
    while (1) {
        int tempCursorX, tempCursorY;
        getCursorPosition(&tempCursorX, &tempCursorY);

        printf("\nFile name: ");
        scanf("%s", inputFilename);

        *inputFilePtr = fopen(inputFilename, "r");

        if (*inputFilePtr != NULL) {
            clearWord(terminalHeight - 7, 0, terminalWidth);
            moveCursor(prevCursorX, prevCursorY);
            break;
        } else {
            clearWord(tempCursorY+1, strlen("Enter the name of the input file: "), terminalWidth);

            moveCursor(0, terminalHeight - 7);
            printf("%*sCould not open \"%s\". Please try again...", currLeftPadding, "", inputFilename);
            moveCursor(tempCursorX, tempCursorY);
        }
    }
}

void getKeys(Action type, Bignum *ePublicOrDPrivate, Bignum *nPublic) {
    char key[5000];
    char firstKey[2500];
    char secondKey[2500];
    char flag = '.';
    int flagIndex;

    while(1) {
        int flagCount = 0, i;

        int tempCursorX, tempCursorY;
        getCursorPosition(&tempCursorX, &tempCursorY);

        if (type == encrypt) {
            printf("Public key: ");
        } else if (type == decrypt) {
            printf("Private key: ");
        }
        scanf("%s", key);

        for (i = 0; i < strlen(key); i++) {
            if (key[i] == flag) {
                flagIndex = i;
                flagCount++;
            }
        }

        if (flagCount == 1) {
            clearWord(terminalHeight - 7, 0, terminalWidth);
            clearWord(terminalHeight - 6, 0, terminalWidth);
            moveCursor(tempCursorX, tempCursorY);
            break;
        }

        clearWord(tempCursorY, strlen("Please enter the public key: "), terminalWidth);

        moveCursor(0, terminalHeight - 7);

        printf("%*sInvalid key!", 
        calculateLeftPadding(strlen("Invalid key!")), "");

        printf("\n%*sPlease make sure you properly pasted the key generated by the program.", 
        calculateLeftPadding(strlen("Please make sure you properly copied the key generated by the program.")), "");

        moveCursor(0, tempCursorY);
    }

    strncpy(firstKey, key, flagIndex);
    firstKey[flagIndex] = '\0';
    strcpy(secondKey, key + flagIndex + 1);

    setBignum(ePublicOrDPrivate, firstKey, positive);
    setBignum(nPublic, secondKey, positive);
}



