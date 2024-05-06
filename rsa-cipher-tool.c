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
    #include <termios.h>
#endif










#define ABOUT_MAX_LENGTH 500
#define ABOUT_MAX_SUBSTRINGS 100

int loadingBarLength = 0;
int terminalWidth = 0;
int terminalHeight = 0;
int aboutLineCap = 0;

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
unsigned long long int encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic);
void decryptText();
unsigned long long int decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPrivate, Bignum *nPublic);
void getInputFile(FILE **inputFilePtr, char *inputFilename, Action type);
void getKeys(Action type, Bignum *ePublicOrDPrivate, Bignum *nPublic);
void about();



int calculateLeftPadding(int strLength);
void clearLines(int startLine, int endLine);
void clearPrompts();
void clearScreen();
void clearWord(int y, int startCol, int endCol);
void getCursorPosition(int *x, int *y);
void getTerminalSize();
void hideCursor();
void loadingBar(int x, int y, int percentageDone);
void loadingStatus(int x, int y, char message[]);
void moveCursor(int x, int y);
void printProgramHeader();
void promptExitConfirm();
void showCursor();
void sleepProgram(int milliseconds);










int main(void) {
    /* When the program initially runs, clear the terminal and determine the global
    variables that the program needs: loadingBarLength, terminalWidth, terminalHeight,
    and aboutLineCap */
    clearScreen();
    getTerminalSize();

    /* Menu options */
    int userMenuState = 0;
    char *optionsArr[] = {"Generate Keys", "Encrypt Text", "Decrypt Text", "About","Exit"};
    int optionsArrSize = sizeof(optionsArr)/sizeof(optionsArr[0]);

    do {
        printProgramHeader();

        /* Loop through menu options */
        printf("\nWhat do you want to do?\n");
		for (int i = 0; i < optionsArrSize; i++) {
			printf("\n%d) - %s", i+1, optionsArr[i]);
		}

        printf("\n\nEnter number: ");
        /* Get the current position of the cursor after the prompt. This coordinate 
        will be used to bring back the cursor at this position when the user inputs 
        an invalid option and the option that they typed is cleared. */
        int tempCursorX, tempCursorY;
        getCursorPosition(&tempCursorX, &tempCursorY);
		scanf("%d", &userMenuState);
		while (getchar() != '\n');

        /* If the user input is a valid menu option, execute the corresponding function */
        if (userMenuState > 0 && userMenuState <= optionsArrSize) {

            clearPrompts();

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
                    about();
				    break;
                case 5:
                    /* If the user wants to exit. Clear the screen and show an exiting feedback */
            	    clearScreen();
                    moveCursor(0, (terminalHeight/2) - 4);
				    printf("%*s%s\n", calculateLeftPadding(strlen("Exiting RSA Cipher Tool...")), "", "Exiting RSA Cipher Tool...");
                    freeAllBignums();
				    sleepProgram(1000);
                    return 0;
                default:
                    break;
            }

            clearScreen();
        }
        /* Else if invalid option, clear the text that the user typed, and bring the 
        cursor back to the stored coordinate */
        else {         
            clearWord(tempCursorY, strlen("Enter number: "), terminalWidth);

            moveCursor(0, terminalHeight - 7);

            printf("%*sInvalid number!", 
            calculateLeftPadding(strlen("Invalid number!")), "");

            printf("\n%*sPlease choose a number between 1 - 5.", 
            calculateLeftPadding(strlen("Please choose a number between 1 - 5.")), "");

            moveCursor(0, tempCursorY);
        }

    } while (userMenuState != optionsArrSize);

    return 0;
}









void generateKeys() {

    KeySize keySizeOptions[] = {
        {"16 bit", 16},
        {"32 bit", 32},
        {"64 bit", 64},
        {"128 bit", 128},
        {"Back", 0},
    };
    int keySizeOptionsSize = sizeof(keySizeOptions)/sizeof(keySizeOptions[0]);

    int chosenKeySize = 0;
    int pPrivateLength, qPrivateLength, ePublicLength;

    while(1) {
        printf("\nThe key size determines the security of the encrypted text!");
        printf("\nThe longer the key size, the longer it will take to encrypt and decrypt.");
        printf("\n\nPlease choose a key size:\n");
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

    clock_t startTime, endTime;
    double elapsedTime;
    startTime = clock();

    clearPrompts();
    printf("\nKey length: %d bit",  chosenKeySize);

    printf("\nGenerating: ");
    int loadingBarX, loadingBarY;
    getCursorPosition(&loadingBarX, &loadingBarY);

    printf("\nStatus: ");
    int loadingStatusX, loadingStatusY;
    getCursorPosition(&loadingStatusX, &loadingStatusY);

#ifndef _WIN32
    loadingBarX += strlen("Generating: ");
    loadingStatusX += strlen("Status: ");
#endif

    loadingBar(loadingBarX, loadingBarY, 0);
    loadingStatus(loadingStatusX, loadingStatusY, "Calculating P prime length...");

    pPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));

    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 5);
    loadingStatus(loadingStatusX, loadingStatusY, "Calculating Q prime length...");

    qPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));

    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 10);
    loadingStatus(loadingStatusX, loadingStatusY, "Calculating E public length...");

    ePublicLength = pPrivateLength > 3 ? pPrivateLength / 2 : (chosenKeySize / log2(10.0)) - 1;

    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 20);
    loadingStatus(loadingStatusX, loadingStatusY, "Initializing Bignums...");

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

    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 30);
    loadingStatus(loadingStatusX, loadingStatusY, "Generating P prime...");

    while (1) {
        // Generate p and q primes
        generatePrimeBignum(&pPrimePrivate, pPrivateLength);

        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 35);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating Q prime...");

        generatePrimeBignum(&qPrimePrivate, qPrivateLength);

        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 40);

        while (isEqualToBignum(&pPrimePrivate, &qPrimePrivate)) {
            generatePrimeBignum(&qPrimePrivate, qPrivateLength);
        }

        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 50);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating N public...");

        // Get n:
        // n = p * q
        multiplyBignum(&nPublic, &pPrimePrivate, &qPrimePrivate);

        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 60);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating phi of N...");

        // Get phi of n:
        // phi of n = (p - 1) * (q - 1)
        subtractBignum(&pPrimePrivateMinusOne, &pPrimePrivate, &one);
        subtractBignum(&qPrimePrivateMinusOne, &qPrimePrivate, &one);
        multiplyBignum(&phiOfNPrivate, &pPrimePrivateMinusOne, &qPrimePrivateMinusOne);

        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 70);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating E public...");

        // Generate e (public key):
        // 2 < e < phi of n
        generatePrimeBignum(&ePublic, ePublicLength);

        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 80);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating D private...");
    
        // Get d (private key):
        // (e * d)mod(n) = 1
        modularInverseBignum(&dPrivate, &ePublic, &phiOfNPrivate);

        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 85);
        loadingStatus(loadingStatusX, loadingStatusY, "Testing keys...");

        modularExponentiationBignum(&encryptedChar, &plainChar, &ePublic, &nPublic);
        modularExponentiationBignum(&decryptedChar, &encryptedChar, &dPrivate, &nPublic);


        if (isEqualToBignum(&plainChar, &decryptedChar)) {
            sleepProgram(300);
            loadingBar(loadingBarX, loadingBarY, 90);
            loadingStatus(loadingStatusX, loadingStatusY, "Keys passed the test...");
            break;
        }

        sleepProgram(500);
        loadingStatus(loadingStatusX, loadingStatusY, "Keys failed! Regenerating new keys...");

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

    sleepProgram(500);
    loadingBar(loadingBarX, loadingBarY, 95);
    loadingStatus(loadingStatusX, loadingStatusY, "Finishing up...");

    int keyPromptLength = 13 + dPrivate.length + nPublic.length;

    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 100);
    loadingStatus(loadingStatusX, loadingStatusY, "Complete");

    endTime = clock();
    elapsedTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;
    printf("\nGenerated keys in: %.2f seconds", elapsedTime);

    printf("\n");

    printf("\nPUBLIC KEY: ");
    printBignum(&ePublic);
    printf(".");
    printBignum(&nPublic);

    printf("\nPRIVATE KEY: ");
    printBignum(&dPrivate);
    printf(".");
    printBignum(&nPublic);

    printf("\n\nRemember: please make sure to safely secure and properly copy the keys above!");

    freeAllBignums();

    promptExitConfirm();
}

void encryptText() {

    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    char outputFilename[] = "en.txt";

    printf("\nAction: Encryption");
    getInputFile(&inputFilePtr, inputFilename, encrypt);

    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        printf("Could not open output file \"%s\". Please try again...", inputFilename);
        exit(1);
    }

    Bignum nPublic, ePublic;
    initBignum(&ePublic);
    initBignum(&nPublic);

    getKeys(encrypt, &ePublic, &nPublic);

    clock_t startTime, endTime;
    double elapsedTime;
    startTime = clock();

    unsigned long long int charactersEncrypted = encryptTextFile(inputFilePtr, outputFilePtr, &ePublic, &nPublic);

    endTime = clock();
    elapsedTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;
    printf("\nEncrypted file in: %.2f seconds", elapsedTime);

    printf("\nCharacters encrypted: %llu", charactersEncrypted);
    printf("\nView the encrypted file at: %s", outputFilename);

    printf("\n\nNote: Make sure you encrypted the file with the recipient's public key, or else they won't be able to decrypt it!");

    freeAllBignums();

    fclose(inputFilePtr);
    fclose(outputFilePtr);

    promptExitConfirm();
}

unsigned long long int encryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *ePublic, Bignum *nPublic) {
    unsigned long long int characterCount = 0;
    unsigned long long int totalCharactersEncrypted = 0;
    int percentageEncrypted = 0;
    char character;

    Bignum encryptedChar, plainChar;
    initBignum(&encryptedChar);
    initBignum(&plainChar);

    fseek(inputFilePtr, 0, SEEK_END);
    characterCount = ftell(inputFilePtr);
    fseek(inputFilePtr, 0, SEEK_SET);

    printf("\nEncryption progress: ");
    int loadingBarX, loadingBarY;
    getCursorPosition(&loadingBarX, &loadingBarY);

    printf("\nStatus: ");
    int loadingStatusX, loadingStatusY;
    getCursorPosition(&loadingStatusX, &loadingStatusY);

#ifndef _WIN32
    loadingBarX += strlen("Encryption progress: ");
    loadingStatusX += strlen("Status: ");
#endif

    loadingBar(loadingBarX, loadingBarY, 0);
    loadingStatus(loadingStatusX, loadingStatusY, "Encrypting file...");

    while ((character = fgetc(inputFilePtr)) != EOF) {
        intToBignum(&plainChar, character, positive);

        modularExponentiationBignum(&encryptedChar, &plainChar, ePublic, nPublic);

        for (unsigned long long int i = encryptedChar.length - 1; i > 0; i--) {
            fprintf(outputFilePtr, "%d", encryptedChar.digits[i]);
        }
        fprintf(outputFilePtr, "%d/", encryptedChar.digits[0]);

        resetBignum(&encryptedChar);
        resetBignum(&plainChar);

        totalCharactersEncrypted++;

        percentageEncrypted = (totalCharactersEncrypted / (float)characterCount) * 100;
        loadingBar(loadingBarX, loadingBarY, percentageEncrypted);
    };

    freeBignum(&encryptedChar);
    freeBignum(&plainChar);

    loadingBar(loadingBarX, loadingBarY, 100);
    loadingStatus(loadingStatusX, loadingStatusY, "Complete");

    return totalCharactersEncrypted;
}

void decryptText() {

    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    char outputFilename[] = "dc.txt";

    printf("\nAction: Decryption");
    getInputFile(&inputFilePtr, inputFilename, decrypt);

    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        printf("Could not open output file \"%s\". Please try again...", inputFilename);
        exit(1);
    }

    Bignum nPublic, dPrivate;
    initBignum(&dPrivate);
    initBignum(&nPublic);

    getKeys(decrypt, &dPrivate, &nPublic);

    clock_t startTime, endTime;
    double elapsedTime;
    startTime = clock();

    unsigned long long int charactersEncrypted = decryptTextFile(inputFilePtr, outputFilePtr, &dPrivate, &nPublic);

    endTime = clock();
    elapsedTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;
    printf("\nDecrypted file in: %.2f seconds", elapsedTime);

    printf("\nCharacters decrypted: %llu", charactersEncrypted);
    printf("\nView the decrypted file at: %s", outputFilename);

    printf("\n\nNote: If the decrypted message doesn't look right, it can either be:");
    printf("\n- The message was not encrypted with your public key");
    printf("\n- You used the wrong key to decrypt the file");

    freeAllBignums();

    fclose(inputFilePtr);
    fclose(outputFilePtr);

    promptExitConfirm();
}

unsigned long long int decryptTextFile(FILE *inputFilePtr, FILE *outputFilePtr, Bignum *dPrivate, Bignum *nPublic) {
    unsigned long long int characterCount = 0;
    unsigned long long int totalCharactersEncrypted = 0;
    int percentageEncrypted = 0;
    char tempCharacter;
    char encryptedCharacter[100];
    char decryptedCharacter;

    Bignum decryptedChar, encryptedChar;
    initBignum(&decryptedChar);
    initBignum(&encryptedChar);

    while((tempCharacter = fgetc(inputFilePtr)) != EOF) {
        if (tempCharacter == '/') {
            characterCount++;
        }
    }
    rewind(inputFilePtr);

    printf("\nDecryption progress: ");
    int loadingBarX, loadingBarY;
    getCursorPosition(&loadingBarX, &loadingBarY);

    printf("\nStatus: ");
    int loadingStatusX, loadingStatusY;
    getCursorPosition(&loadingStatusX, &loadingStatusY);

#ifndef _WIN32
    loadingBarX += strlen("Decryption progress: ");
    loadingStatusX += strlen("Status: ");
#endif

    loadingBar(loadingBarX, loadingBarY, 0);
    loadingStatus(loadingStatusX, loadingStatusY, "Decrypting file...");

    while (fscanf(inputFilePtr, "%[^/]/", encryptedCharacter) == 1) {
        setBignum(&encryptedChar, encryptedCharacter, positive);

        modularExponentiationBignum(&decryptedChar, &encryptedChar, dPrivate, nPublic);

        decryptedCharacter = bignumToInt(&decryptedChar);

        fprintf(outputFilePtr, "%c", decryptedCharacter);

        resetBignum(&encryptedChar);
        resetBignum(&decryptedChar);
        encryptedCharacter[0] = '\0';
        decryptedCharacter = '\0';

        totalCharactersEncrypted++;

        percentageEncrypted = (totalCharactersEncrypted / (float)characterCount) * 100;
        loadingBar(loadingBarX, loadingBarY, percentageEncrypted);
    };

    loadingBar(loadingBarX, loadingBarY, 100);
    loadingStatus(loadingStatusX, loadingStatusY, "Complete");

    freeBignum(&decryptedChar);
    freeBignum(&encryptedChar);

    return totalCharactersEncrypted;
}

int isValidEncryptedFile(FILE *inputFilePtr) {
    char character;
    while((character = fgetc(inputFilePtr)) != EOF) {
        if (!isdigit(character) && character != '/') {
            rewind(inputFilePtr);
            return 0;
        }
    }
    rewind(inputFilePtr);
    return 1;
}

void getInputFile(FILE **inputFilePtr, char *inputFilename, Action type) {
    unsigned long long int characterCount = 0;
    char character;
    int isValidFile = 0;
    char errorMessage[100];

    printf("\n");
    int tempCursorX, tempCursorY;
    getCursorPosition(&tempCursorX, &tempCursorY);

    while (1) {
        char *promptMessage = "File name: ";
        printf("%s", promptMessage);
        scanf("%s", inputFilename);

        sprintf(errorMessage, "Could not open \"%s\". Please try again...", inputFilename);

        *inputFilePtr = fopen(inputFilename, "r");

        if (*inputFilePtr != NULL) {
            isValidFile = type == decrypt ? isValidEncryptedFile(*inputFilePtr) : 1;

            if (type == decrypt) {
                sprintf(errorMessage, "\"%s\" is not a valid file encrypted by the program...", inputFilename);
            }
        }

        if (isValidFile) {
            clearWord(terminalHeight - 7, 0, terminalWidth);
            moveCursor(tempCursorX, tempCursorY+1);
            break;
        }

        clearWord(tempCursorY, strlen(promptMessage), terminalWidth);
        clearWord(terminalHeight - 7, 0, terminalWidth);
        moveCursor(0, terminalHeight - 7);
        printf("%*s%s", calculateLeftPadding(strlen(errorMessage)), "", errorMessage);
        moveCursor(tempCursorX, tempCursorY);
        
    }
}

void getKeys(Action type, Bignum *ePublicOrDPrivate, Bignum *nPublic) {
    char key[5000];
    char firstKey[2500];
    char secondKey[2500];
    char flag = '.';
    int flagIndex;

    int tempCursorX, tempCursorY;
    getCursorPosition(&tempCursorX, &tempCursorY);

    while(1) {
        int flagCount = 0, i;

        moveCursor(tempCursorX, tempCursorY);

        char promptMsg[20];
        if (type == encrypt) {
            strcpy(promptMsg, "Public key: ");
        } else if (type == decrypt) {
            strcpy(promptMsg, "Private key: ");
        }
        printf("%s", promptMsg);
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

        clearWord(tempCursorY, strlen(promptMsg), terminalWidth);
        clearLines(tempCursorY+1, terminalHeight - 8);

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

void splitString(const char *inputStr, char outputArr[][ABOUT_MAX_LENGTH], int *outputArrCount, int lineCap) {
    int indexOffset = 0;
    int substringIndex = 0;

    int chunkToCopy = lineCap;

    int numOfCharactersBeforeASpace = 0;
    int j = 0;
    int k = 0;

    // Iterate through the input string
    for (int i = 0; i < strlen(inputStr); i += lineCap) {
        while (indexOffset + chunkToCopy + j < strlen(inputStr) && inputStr[indexOffset + chunkToCopy + j] != ' ') {
            numOfCharactersBeforeASpace++;
            j++;
        }

        chunkToCopy += numOfCharactersBeforeASpace;

        if (inputStr[indexOffset + k] == '\t' && indexOffset != 0) {
            chunkToCopy -= 8;
        }
        if (inputStr[indexOffset] == ' ' && indexOffset != 0) {
            k++;
        }

        strncpy(outputArr[*outputArrCount], inputStr + indexOffset + k, chunkToCopy);

        // Null terminate the substring
        outputArr[*outputArrCount][chunkToCopy + numOfCharactersBeforeASpace] = '\0';
        // outputArr[*outputArrCount][lineCap] = '\0';

        // Update indices
        indexOffset += chunkToCopy;

        // Increment count of substrings
        (*outputArrCount)++;

        chunkToCopy = lineCap;
        j = 0;
        k = 0;
        numOfCharactersBeforeASpace = 0;
    }
}

void about() {
    char paragraphs[][ABOUT_MAX_LENGTH] = {
        "This tool is a smart way to keep secrets safe online! It's like a lock and key system, where only the right key can unlock the secret message. It uses the RSA encryption magic to keep your messages secure. You can encrypt messages that you want to keep tucked away, or encrypt a message that you want to send to your friend using their public key.\n",
        "Using our tool is easy-peasy!",
        "(1) Generate your very own private and public keys, but remember to keep these keys safe and hidden.",
        "(2) With the keys generate, you can now lock and unlock messages. The public key will encrypt messages, and the private key will decrypt them back to the original text. Note: Only the corresponding private key can be used to decrypt the text encrypted with its corresponding public key.\n",
        "This is a freshman Computer Science final project for Computer Programming 2 (CS103), Academic Year 2023-2024, at Bicol University College of Science. This project would not be possible without our amazing contributors:\n",
        "Michael Xavier Canonizado",
        "Deanne Clarice Bea",
        "Simon Narvaez",
        "Marc Jordan Campopos"
    };
    int paragraphsSize = sizeof(paragraphs) / sizeof(paragraphs[0]);
    char paragraphsSubstrings[ABOUT_MAX_SUBSTRINGS][ABOUT_MAX_LENGTH];
    int paragraphSubstringCount = 0;

    for (int i = 0; i < paragraphsSize; i++) {
        splitString(paragraphs[i], paragraphsSubstrings, &paragraphSubstringCount, aboutLineCap);
    }

    int topPadding = (terminalHeight - paragraphSubstringCount) / 4;

    for (int i = 0; i < topPadding; i++) {
        printf("\n");
    }

    for (int i = 0; i < paragraphSubstringCount; i++) {
        printf("\n%*s%s", calculateLeftPadding(strlen(paragraphsSubstrings[i])), "", paragraphsSubstrings[i]);
    }

    char userInput[100];
    int bottomPadding = topPadding;
    int bottomLeftPadding = calculateLeftPadding(strlen("Enter DONE to go back: "));
    
    int tempCursorX, tempCursorY;
    int cursorXToDelete, cursorYToDelete;

    for (int i = 0; i < bottomPadding + 3; i++) {
        printf("\n");
    }

    getCursorPosition(&tempCursorX, &tempCursorY);

    do {
        moveCursor(tempCursorX, tempCursorY-1);
        for (int i = 0; i < bottomLeftPadding; i++) {
            printf(" ");
        }

        printf("Enter DONE to go back: ");
        getCursorPosition(&cursorXToDelete, &cursorYToDelete);
        fgets(userInput, sizeof(userInput), stdin);

		// Replace the newline character at the end of the input to NULL
		if (userInput[strlen(userInput) - 1] == '\n') {
			userInput[strlen(userInput) - 1] = '\0';
		}

        // Convert the user's input to lowercase
        for(int i = 0; userInput[i]; i++){
            userInput[i] = tolower(userInput[i]);
        }

        clearWord(cursorYToDelete - 1, 0, terminalWidth);

    } while (strcmp(userInput, "done") != 0);
}




int calculateLeftPadding(int strLength) {
    int remainingWidth = terminalWidth - strLength;
    int leftPadding = remainingWidth % 2 ? (remainingWidth + 1) / 2 : remainingWidth / 2;
    return leftPadding;
}

void clearLines(int startLine, int endLine) {
    hideCursor();
	for (int i = startLine; i <= endLine; i++) {
		moveCursor(0, i);
		for (int j = 0; j < terminalWidth; j++) {
			printf(" ");
		}
	}
    showCursor();
}

void clearPrompts() {
    clearScreen();
    printProgramHeader();
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	printf("\033[2J");
	printf("\033[H");
#endif
}

void clearWord(int y, int startCol, int endCol) {
    moveCursor(startCol, y);
    for (int i = 0; i < endCol - startCol; i++) {
		printf(" ");
    }
}

void getCursorPosition(int *x, int *y) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    *x = info.dwCursorPosition.X;
    *y = info.dwCursorPosition.Y;
#else
    // https://stackoverflow.com/questions/50884685/how-to-get-cursor-position-in-c-using-ansi-code/50888457#50888457
    char buf[30]={0};
    int ret, i, pow;
    char ch;

    *y = 0; *x = 0;

    struct termios term, restore;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for( i = 0, ch = 0; ch != 'R'; i++ )
    {
        ret = read(0, &ch, 1);
        if ( !ret ) {
            tcsetattr(0, TCSANOW, &restore);
            fprintf(stderr, "getpos: error reading response!\n");
            return;
        }
        buf[i] = ch;
    }

    if (i < 2) {
        tcsetattr(0, TCSANOW, &restore);
        printf("i < 2\n");
        return;
    }

    for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
        *x = *x + ( buf[i] - '0' ) * pow;

    for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + ( buf[i] - '0' ) * pow;

    tcsetattr(0, TCSANOW, &restore);
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

   loadingBarLength = terminalWidth - strlen("Encryption progress:   ( 100%% )");
    aboutLineCap = terminalWidth * 0.7;
    if (aboutLineCap > 100) {
        aboutLineCap = 100;
    }
}

void hideCursor() {
#ifdef _WIN32
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
#else
    printf("\e[?25l");
#endif
}

void loadingBar(int x, int y, int percentDone) {
    if (percentDone > 100) {
        percentDone = 100;
    }

	int fill = percentDone * loadingBarLength / 100;
    int track = loadingBarLength - fill;

    moveCursor(x, y);

    if (percentDone < 100) {
        hideCursor();
    }

    printf("[");

	char a = '=', b = ' ';
    
	for (int i = 0; i < fill; i++) {
		printf("%c", a);
	}
	for (int i = 0; i < track; i++) {
		printf("%c", b);
	}

    printf("]");
    
	printf(" ( %d%% )", percentDone);
	fflush(stdout);

    if (percentDone >= 100) {
        showCursor();
    }
}

void loadingStatus(int x, int y, char message[]) {
    clearWord(y, x, terminalWidth);
    moveCursor(x, y);
    printf("%s", message);
	fflush(stdout);
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

void printProgramHeader() {
    moveCursor(0,0);
    printf("\n");
    for (int i = 0; i < terminalWidth; i++) printf("-");

    printf("\n%*sRSA Cipher Tool", calculateLeftPadding(strlen("RSA Cipher Tool")), "");

    printf("\n");
    for (int i = 0; i < terminalWidth; i++) printf("-");
    printf("\n");

#ifdef _WIN32
    moveCursor(0,4);
#else
    moveCursor(0,5);
#endif
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

void showCursor() {
#ifdef _WIN32
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = TRUE;
   SetConsoleCursorInfo(consoleHandle, &info);
#else
    printf("\e[?25h");
#endif
}

void sleepProgram(int milliseconds) {
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}



