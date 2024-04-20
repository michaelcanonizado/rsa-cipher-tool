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
    printf("\n%*s%s%*s", leftPadding, "", str, rightPadding, ""); \
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
void aboutProject();

void clearScreen();
void getTerminalSize();
void moveCursor(int x, int y);
void waitForDone();
void sleepProgram(int milliseconds);
void clearLines(int startLine, int endLine, int width);
void confirmToExit();

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


int main(void) {
    clearScreen();
    getTerminalSize();

    int userMenuState = 0;
    int adjustedHeight = terminalHeight / 3;
    int i;

    PRINT_FORMATS_CENTER(terminalWidth, "Width: %d", terminalWidth);
    PRINT_FORMATS_CENTER(terminalWidth, "Height: %d", terminalHeight);

    char *optionsArr[] = {"Generate Keys", "Encrypt Text", "Decrypt Text", "About","Exit"};
    int optionsArrSize = sizeof(optionsArr)/sizeof(optionsArr[0]);

    do {
		for (i = 0; i < optionsArrSize; i++) {
            moveCursor((terminalWidth - getMaxStringLengthInArray(optionsArr, optionsArrSize)) / 2, adjustedHeight + i);
			printf("%d) - %s", i+1, optionsArr[i]);
		}

        char str[] = "Enter number: ";
		moveCursor((terminalWidth - strlen(str))/ 2, adjustedHeight + (i + 1));
        printf("%s", str);
		scanf("%d", &userMenuState);
		while (getchar() != '\n');

        switch (userMenuState) {
			case 1:
                generateKeys();
                promptExitConfirm();
                clearScreen();
				break;
			case 2:
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

void waitForDone() {
	char done[100];
	do {
		clearLines(terminalHeight - 2, terminalHeight - 2, terminalWidth);
		moveCursor((terminalWidth - 21)/ 2, terminalHeight - 2);
		printf("Enter DONE to back: ");
		// Get the user's input from the standard input stream	
		fgets(done, sizeof(done), stdin); 

		// Remove the newline character at the end of the input
		if (done[strlen(done) - 1] == '\n') {
			done[strlen(done) - 1] = '\0';
		}

		// If the input was just a newline character, continue with the next iteration
		if (strlen(done) == 0) {
			continue;
		}

		// Convert the user's input to lowercase. This allows the user to enter "done" or "DONE" to exit the loop in any case.
		for(int i = 0; done[i]; i++){
			done[i] = tolower(done[i]);
		}

		moveCursor((terminalWidth - 21)/ 2, terminalHeight - 2);

	} while (strcmp(done, "done") != 0);
}

void sleepProgram(int milliseconds) {
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}

void clearLines(int startLine, int endLine, int width) {
	for (int i = startLine; i <= endLine; i++) {
		moveCursor(0, i);
		for (int j = 0; j < width; j++) {
			printf(" ");
		}
	}
}

void promptExitConfirm() {
	char userInput[100];

	do {
		clearLines(terminalHeight - 2, terminalHeight - 2, terminalWidth);
		moveCursor((terminalWidth - 21)/ 2, terminalHeight - 2);
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