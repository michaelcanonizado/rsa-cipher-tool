#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "../bignum.h"

// Includes the appropriate header file to use operating system-specific functions. This is useful for functions like clearing the screen, moving the cursor, and getting the terminal size.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif

// Function prototypes
	// Function to get the terminal size. This function will set the width and height variables to the width and height of the terminal, respectively. The function will use operating system-specific functions to get the terminal size.
	void getTerminalSize();
	// Function to clear the screen. This function will clear the screen using operating system-specific functions.
	void clearScreen();
	// Function to sleep the program. This function will pause the program for a specified number of milliseconds using operating system-specific functions.
	void sleepProgram(int milliseconds);
	// Function to move the cursor. This function will move the cursor to the specified x and y coordinates using operating system-specific functions.
	void moveCursor(int x, int y);
	// Function to clear lines. This function will clear the specified lines starting from the startLine to the endLine using the specified width.
	void clearLines(int startLine, int endLine, int width);
	// Function to display a loading bar.
	void loadingBar(int progress);
	// Function to wait for the user to input "DONE" to continue.
	void waitForDone();
	// Function to get the user's confirmation. This function will get the user's confirmation by asking the user to input 'Y' or 'N' and return the input.
	char getConfirm();
	// Function to display a message with the specified width and height and return the number of lines displayed.
	int displayMessage(char *message[], int count);
	// Function to display the About message. Separate function was made because it starts at a different y position.
	void displayAbout(char *message[], int count, int offsestY);
	// Function to get the input file. This function will get the input file from the user and return the file pointer.
	void getInputFile(FILE **inputFilePtr, char *inputFilename);
	// Function to get the public or private key.
	void getKeys(Bignum *ePublicOrDPrivate, Bignum *nPublic, char *whatKey);
	// Function to get the file size in bytes.
	long int getFileSize(FILE *file);
	// Function to generate the public and private keys for the RSA algorithm.
	void generateKeys(int i);
	// Functions to encrypt the text file using the RSA algorithm.
	void encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic);
	void encryptText();
	// Function to decrypt the text using the RSA algorithm.
	void decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPrivate, Bignum *nPublic);
	void decryptText();
	// Function to display information about the program.
	void aboutProgram();

// Global variables
char confirm;
int offsetY;
const int PROGRESS_BAR_LENGTH = 30;
							FILE *decryptionFile;
							char fileName[100];
int width, height, adjustedHeight;

typedef struct keySize {
    char name[50];
    int size;
} KeySize;

void generate(int chosenKeySize) {
	// printf("\n.........................................");
	// printf("\n");

	// KeySize keySizeOptions[] = {
	//     {"16 bit", 16},
	//     {"32 bit", 32},
	//     {"64 bit", 64},
	//     {"128 bit", 128},
	//     {"256 bit", 256},
	// };

	int pPrivateLength, qPrivateLength, ePublicLength;

	// printf("\nPlease choose a key size: ");
	// for (int i = 0; i < sizeof(keySizeOptions)/sizeof(keySizeOptions[0]); i++) {
	//     printf("\n%d) - %s", i+1, keySizeOptions[i].name);
	// }
	// printf("\nEnter number: ");
	// scanf("%d", &chosenKeySize);
	// chosenKeySize = keySizeOptions[chosenKeySize-1].size;
	// printf("Chosen key size: %d", chosenKeySize);

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

		// printf("\n\nTesting keys: ");

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

	clearLines((height / 2) - 1, (height / 2) + 1, width);

	moveCursor((width - 12) / 2, adjustedHeight);
	printf("PUBLIC KEY:");
	moveCursor((width - 30) / 2, adjustedHeight + 1);
	printBignum(&ePublic);
	printf(".");
	printBignum(&nPublic);
	moveCursor((width - 12) / 2, adjustedHeight + 2);
	printf("PRIVATE KEY:");
	moveCursor((width - 30) / 2, adjustedHeight + 3);
	printBignum(&dPrivate);
	printf(".");
	printBignum(&nPublic);

	// print("\n\n");

	freeAllBignums();
}

int main() {
	clearScreen();

	getTerminalSize();
	printf("Width: %d\nHeight: %d\n", width, height);

	int i, userInput;

	do {
		char *optionsArr[5] = {"1) Generate Keys", "2) Encrypt Text", "3) Decrypt Text", "4) About", "5) Exit program"};
		for ( i = 0; i < 5; i++) {
			moveCursor((width - 14) / 2, adjustedHeight + i);
			printf("%s\n", optionsArr[i]);
		}

		moveCursor((width - 10)/ 2, adjustedHeight + i);
		printf("Enter number: ");
		scanf("%d", &userInput);
		// Consume any extra characters in the input buffer to prevent an infinite loop
		while (getchar() != '\n');

		switch (userInput) {
			case 1:
				generateKeys(i);
				break;
			case 2:
				encryptText();
				break;
			case 3:
				decryptText();
				break;
			case 4:
				aboutProgram();
				break;
			case 5:
				clearScreen();
				moveCursor((width - 20) / 2, adjustedHeight + 5);
				printf("Exiting program...\n");
				sleepProgram(500);
				break;
			default:
				clearScreen();
				moveCursor((width - 15) / 2, adjustedHeight + 1);
				printf("Invalid input.");
				moveCursor((width - 39) / 2, adjustedHeight + 2);
				printf("Please enter a number between 1 and 5.");
				sleepProgram(1000);
				clearScreen();
				break;
		}
	} while (userInput != 5);

	return 0;
}

void getTerminalSize() {
	#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	#else
		struct winsize size;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		width = size.ws_col;
		height = size.ws_row;
	#endif
		// Since the horizontal positioning of the outputs can be adjusted by incrementing the y-axis, this integer variable stores value of the height divided by 3 as all the outputs starts to be displayed at this value. This can't be applied to the x-axis as the outputs are displayed at the center of the screen. The vertical positioning is depedent on the length of the string to be displayed.
	  adjustedHeight = height / 3;
}

void clearScreen() {
	#ifdef _WIN32
		system("cls");
	#else
		printf("\033[2J");
		printf("\033[H");
	#endif
}

void sleepProgram(int milliseconds) {
	#ifdef _WIN32
		Sleep(milliseconds);
	#else
		usleep(milliseconds * 1000);
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

void clearLines(int startLine, int endLine, int width) {
	for (int i = startLine; i <= endLine; i++) {
		moveCursor(0, i);
		for (int j = 0; j < width; j++) {
			printf(" ");
		}
	}
}

void loadingBar(int percentDone) { 
	int numChar = percentDone * PROGRESS_BAR_LENGTH / 100;
	int numSpace = PROGRESS_BAR_LENGTH - numChar;
  int start = (width - (PROGRESS_BAR_LENGTH + 11)) / 2;

  moveCursor(start, height / 2);
	printf(" ");

#ifdef _WIN32
	char a = 219, b = 177;
	for (int i = 0; i < numChar; i++) {
		printf("%c", a);
	}
	for (int i = 0; i < numSpace; i++) {
		printf("%c", b);
	}
#else
	char *a = "█", *b = "░";
	for (int i = 0; i < numChar; i++) {
		printf("%s", a);
	}
	for (int i = 0; i < numSpace; i++) {
		printf("%s", b);
	}
#endif
	printf(" %d%% Done", percentDone);
	fflush(stdout);
}

void waitForDone() {
	char done[100];
	do {
		clearLines(height - 2, height - 2, width);
		moveCursor((width - 21)/ 2, height - 2);
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

		moveCursor((width - 21)/ 2, height - 2);

	} while (strcmp(done, "done") != 0);
}

char getConfirm(int width, int adjustedHeight, int offsetY) {
	char confirm;
	confirm = getchar();
	if (confirm == '\n') {
		confirm = getchar();
	}
	if (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n') {
		moveCursor((width - 37) / 2, adjustedHeight + offsetY);
		printf("Invalid input. Please enter Y or N.");
		moveCursor((width - 37) / 2, adjustedHeight + offsetY);
	}

	// Consume any extra characters in the input buffer
	while (getchar() != '\n');
	
	return confirm;
}

int displayMessage(char *message[], int count) {
	int i;
	for ( i = 0; i < count; i++) {
		moveCursor((width - strlen(message[i])) / 2, adjustedHeight + i);
		printf("%s\n", message[i]);
	}

	return i;
}

void displayAbout(char *message[], int count, int offsetY) {
	for (int i = 0; i < count; i++) {
		moveCursor((width - strlen(message[i])) / 2, height / 4 + i + offsetY);
		printf("%s\n", message[i]);
	}
}

void getInputFile(FILE **inputFilePtr, char *inputFilename) {
	do
	{
		clearLines(adjustedHeight, adjustedHeight, width);
		moveCursor((width - 35)/ 2, adjustedHeight);
		printf("Enter input file name: ");
		scanf("%s", inputFilename);

		*inputFilePtr = fopen(inputFilename, "r");

		if (*inputFilePtr != NULL) {
			break;
		} else {
			moveCursor((width - 41)/ 2, adjustedHeight + 1);
			printf("Could not open file. Please try again...");
		}
	} while (1);
	
	clearLines(adjustedHeight + 1, adjustedHeight + 3, width);
	moveCursor((width - 28)/ 2, adjustedHeight + 1);
	printf("File opened successfully...");

	// Get the number of characters

}

void getKeys(Bignum *ePublicOrDPrivate, Bignum *nPublic, char *whatKey) {
	char key[5000];
	char firstKey[2500];
	char secondKey[2500];
	char flag = '.';
	int flagIndex;

	while(1) {
		clearLines(adjustedHeight + 2, adjustedHeight + 2, width);
		moveCursor((width - (strlen(whatKey) + 35) )/ 2, adjustedHeight + 2);
		printf("Please enter the %s key: ", whatKey);
		scanf("%s", key);

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
		moveCursor((width - 34)/ 2, adjustedHeight + 3);
		printf("Invalid key! Please make sure you");
		moveCursor((width - 37)/ 2, adjustedHeight + 4);
		printf("properly copied the key generated...");
	}

	strncpy(firstKey, key, flagIndex);
	firstKey[flagIndex] = '\0';
	strcpy(secondKey, key + flagIndex + 1);
	// printf("String before '.': %s\n", firstKey);
	// printf("String after '.': %s\n", secondKey);

	setBignum(ePublicOrDPrivate, firstKey, positive);
	setBignum(nPublic, secondKey, positive);
}

long int getFileSize(FILE *file) {
    long int size;

    // Move the file pointer to the end of the file
    fseek(file, 0, SEEK_END);

    // Get the current file pointer position which represents the file size
    size = ftell(file);

    // Move the file pointer back to the start of the file
    rewind(file);

    return size;
}

void generateKeys(int i) {
	clearScreen();
	char *msgGenerate[] = {"You are about to generate an RSA key", "this option. This will display a private key", "and public key of the generated private key."};
	int countGenerate = sizeof(msgGenerate) / sizeof(msgGenerate[0]);
	offsetY = displayMessage(msgGenerate, countGenerate);

	do{
		clearLines(adjustedHeight + offsetY, adjustedHeight + offsetY + 1, width);
		moveCursor((width - 37)/ 2, adjustedHeight + offsetY);
		printf("Do you agree to save a copy of your\n");
		moveCursor((width - 32)/ 2, adjustedHeight + offsetY + 1);
		printf("private and public keys? [Y/N] ");
		confirm = getConfirm(width, adjustedHeight, offsetY + 1);

	} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

	clearScreen();	
	if (confirm == 'Y' || confirm == 'y') {

    KeySize keySizeOptions[] = {
        {"16 bit", 16},
        {"32 bit", 32},
        {"64 bit", 64},
        {"128 bit", 128},
        {"256 bit", 256},
    };

    int chosenKeySize = 0;

		moveCursor((width - 28)/ 2, adjustedHeight);
    printf("Please choose a key size: ");
    for (int i = 0; i < sizeof(keySizeOptions)/sizeof(keySizeOptions[0]); i++) {
			moveCursor((width - 9)/ 2, adjustedHeight + i + 1);
			printf("%d) %s", i+1, keySizeOptions[i].name);
    }
		do {
			clearLines(adjustedHeight + 6, adjustedHeight + 6, width);
			moveCursor((width - 22)/ 2, adjustedHeight + 6);
			printf("Enter number: ");
			scanf("%d", &chosenKeySize);

			if (chosenKeySize < 1 || chosenKeySize > 5) {
				moveCursor((width - 15) / 2, adjustedHeight + 7);
				printf("Invalid input.");
				moveCursor((width - 39) / 2, adjustedHeight + 8);
				printf("Please enter a number between 1 and 5.");			}

		} while (chosenKeySize < 1 || chosenKeySize > 5);

		clearScreen();

		chosenKeySize = keySizeOptions[chosenKeySize-1].size;
    moveCursor((width - 25)/ 2, adjustedHeight);
		printf("Chosen key size: %d bit", chosenKeySize);

		clearScreen();
		clock_t start = clock();
		
		// At this point, function calls can be made to generate the keys. For now, the program will display a message that the keys are generated.

		loadingBar(0);
		int progress = 0;
		while (1) {
			for (int i = 0; i < 100; i++) {
				loadingBar(progress);
				sleepProgram(10);
				progress = (progress + 1) % 101;
			}
			generate(chosenKeySize);
			break;
		}

		// MAAYOS KANG SCREEEN TA NAWAWALAT SU LOADING BAR KAPAG MAY KEYS NA

		clock_t end = clock();
		double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

		moveCursor((width - 33)/ 2, adjustedHeight + 7);
		printf("Keys generated in: %.2f seconds", cpu_time_used);

	} else {
		moveCursor((width - 25)/ 2, adjustedHeight);
		printf("Keys generation failed!\n");

	}

	waitForDone();
	freeAllBignums();
	clearScreen();
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

void encryptText() {
	FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

	char inputFilename[100];
	char outputFilename[] = "en.txt";

	clearScreen();
	char* msgEncrypt[] = {"Encryption includes the message to be encrypted and", "the public key of the recipient. The txt file of the", "message must be in the same folder of the C program."};
	int countEncrypt = sizeof(msgEncrypt) / sizeof(msgEncrypt[0]);
	offsetY = displayMessage(msgEncrypt, countEncrypt);

	do {
		clearLines(adjustedHeight + offsetY, adjustedHeight + offsetY, width);
		moveCursor((width - 60)/ 2, adjustedHeight + offsetY);
		printf("Is the txt file in the same folder of the C program? [Y/N] ");
		confirm = getConfirm(width, adjustedHeight, offsetY + 1);

	} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

	if (confirm == 'Y' || confirm == 'y') {
		do {
			clearLines(adjustedHeight + offsetY + 1, adjustedHeight + offsetY + 2, width);
			moveCursor((width - 47)/ 2, adjustedHeight + offsetY + 1);
			printf("Do you have the recipient's public key? [Y/N] ");
			confirm = getConfirm(width, adjustedHeight, offsetY + 2);

		} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

		if (confirm == 'Y' || confirm == 'y') {
			clearScreen();

			getInputFile(&inputFilePtr, inputFilename);

			outputFilePtr = fopen(outputFilename, "w");
			if (outputFilePtr == NULL) {
				printf("Error opening output %s...\n", outputFilename);
				exit(1);
			}

			Bignum nPublic, ePublic;
			initBignum(&nPublic);
			initBignum(&ePublic);

			getKeys(&ePublic, &nPublic, "public");

			clearScreen();

			printf("\nBignum key e: ");
			printBignum(&ePublic);
			printf("\nBignum key n: ");
			printBignum(&nPublic);

			clearScreen();

			clock_t start = clock();

			unsigned long long int fileSize = getFileSize(inputFilePtr);

			loadingBar(0);
			for (int i = 0; i < fileSize; i++) {
				encryptTextFile(inputFilePtr, outputFilePtr, &ePublic, &nPublic);
				loadingBar((i+1)*100/fileSize);
			}
			
			clock_t end = clock();
			double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

			moveCursor((width - 30)/ 2, adjustedHeight);
			printf("Message encryption success!");
			moveCursor((width - (28 + fileSize + 7))/ 2, adjustedHeight + 2);
			printf("Encrypted %llu bytes in %.3f seconds", fileSize, cpu_time_used);

		} else {
		clearScreen();
		moveCursor((width - 25)/ 2, adjustedHeight);
		printf("Message encryption failed!\n");
		}
	} 
	else {
		clearScreen();
		moveCursor((width - 25)/ 2, adjustedHeight);
		printf("Message encryption failed!\n");
	}
	
	freeAllBignums();

	if (inputFilePtr != NULL) {
		fclose(inputFilePtr);
	}
	if (outputFilePtr != NULL) {
		fclose(outputFilePtr);
	}

	waitForDone();
	clearScreen();
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

        fprintf(outputFilePtr, "%c", decryptedCharacter);

        resetBignum(&encryptedChar);
        resetBignum(&decryptedChar);
        encryptedCharacter[0] = '\0';
        decryptedCharacter = '\0';
    };

    freeBignum(&decryptedChar);
    freeBignum(&encryptedChar);
}

void decryptText() {
	FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

	char inputFilename[100];
	char outputFilename[] = "dc.txt";
	
	clearScreen();
	char* msgDecrypt[] = {"Decryption includes the message to be decrypted and", "the private key of the recipient. The txt file of the", "message must be in the same folder of the C program."};
	int countDecrypt = sizeof(msgDecrypt) / sizeof(msgDecrypt[0]);
	offsetY = displayMessage(msgDecrypt, countDecrypt);

	do {
		clearLines(adjustedHeight + offsetY, adjustedHeight + offsetY, width);
		moveCursor((width - 60)/ 2, adjustedHeight + offsetY);
		printf("Is the txt file in the same folder of the C program? [Y/N] ");
		confirm = getConfirm(width, adjustedHeight, offsetY);

	} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

	if (confirm == 'Y' || confirm == 'y') {
		do {
			clearLines(adjustedHeight + offsetY + 1, adjustedHeight + offsetY + 2, width);
			moveCursor((width - 38)/ 2, adjustedHeight + offsetY + 1);
			printf("Do you have your private key? [Y/N] ");
			confirm = getConfirm(width, adjustedHeight, offsetY + 1);

		} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

		if (confirm == 'Y' || confirm == 'y') {
			clearScreen();

			getInputFile(&inputFilePtr, inputFilename);

			outputFilePtr = fopen(outputFilename, "w");
			if (outputFilePtr == NULL) {
				printf("Error opening output %s...\n", outputFilename);
				exit(1);
			}

    	Bignum nPublic, dPrivate;
    	initBignum(&nPublic);
    	initBignum(&dPrivate);

			getKeys(&dPrivate, &nPublic, "private");

			clearScreen();

			printf("\nBignum key d: ");
			printBignum(&dPrivate);
			printf("\nBignum key n: ");
			printBignum(&nPublic);

			clearScreen();

			clock_t start = clock();

			unsigned long long int fileSize = getFileSize(inputFilePtr);

			loadingBar(0);
			for ( int i = 0; i < fileSize; i++) {
				decryptTextFile(inputFilePtr, outputFilePtr, &dPrivate, &nPublic);
				loadingBar((i+1)*100/fileSize);
			}
			
			clock_t end = clock();
			double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

			moveCursor((width - 30)/ 2, adjustedHeight);
			printf("Message decryption success!");
			moveCursor((width - (28 + fileSize + 7))/ 2, adjustedHeight + 2);
			printf("Decrypted %llu bytes in %.3f seconds", fileSize, cpu_time_used);

		} else {
		clearScreen();
		moveCursor((width - 25)/ 2, adjustedHeight);
		printf("Message encryption failed!\n");
		}
	}
	else {
		clearScreen();
		moveCursor((width - 25)/ 2, adjustedHeight);
		printf("Message decryption failed!\n");
	}
	
	freeAllBignums();

	if (inputFilePtr != NULL) {
		fclose(inputFilePtr);
	}
	if (outputFilePtr != NULL) {
		fclose(outputFilePtr);
	}

	waitForDone();
	clearScreen();
}

void aboutProgram() {
	clearScreen();
	moveCursor((width - 45) / 2, height / 4);
	printf("RSA Cipher Tool with Custom Bignum Library");

	char *about[] = {"The RSA Cipher Tool program generates, encrypts, and decrypts messages using the RSA", "algorithm. The program uses the RSA (Rivest, Shamir, Adleman) Algorithm to generate public", "and private keys needed to encrypt and decrypt messages and a custom bignum library to", "handle large numbers numbers for the RSA encryption and decryption processes."};
	int aboutCount = sizeof(about) / sizeof(about[0]);
	displayAbout(about, aboutCount, 2);

	char* about2[] = {"The user will use the program to encrypt a message in a file, where it will then return the", "encrypted/secret message. Then, they can now send it to the recipient of the message safely,", "where they can also use the same program to decrypt the message and reveal the actual message."};
	int about2Count = sizeof(about2) / sizeof(about2[0]);
	displayAbout(about2, about2Count, 7);

	char* about3[] = {"This is a programming project for Computer Programming 2 during the Academic Year", "2023-2024 at the Bicol University College of Science. The program is developed by the", "following students of Bicol University College of Science:"};
	int about3Count = sizeof(about3) / sizeof(about3[0]);
	displayAbout(about3, about3Count, 11);

	char* members[] = { "Michael Xavier Canonizado", "Deanne Clarice Bea", "Simon Narvaez", "Marc Jordan Campopos"};
	int membersCount = sizeof(members) / sizeof(members[0]);
	displayAbout(members, membersCount, 14);

	waitForDone(width, height);
	clearScreen();
}