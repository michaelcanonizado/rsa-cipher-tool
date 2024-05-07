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
void getInputFile(FILE **inputFilePtr, char *inputFilename, Action type);
int isValidEncryptedFile(FILE *inputFilePtr);
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
    variables that the program needs: loadingBarLength, terminalWidth, and terminalHeight*/
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

            /* Each menu option will still display the program header, thus just clear 
            the prompts */ 
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

    /* Key sizes to choose from.

    Currently, the NIST (National Institute of Standards and Technology)
    recommends a minimum of 2048 bit keys (Barker & Dang, 2015,
    https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-57Pt3r1.pdf).
    But a 2048 bit key is takes too long for bignum.h to calculate and leads to an 
    overflow. Thus, smaller key sizes where chosen for the sake of demonstration.
    Further optimization can be made to bignum.h to enable it to handle longer
    key sizes efficiently.

    An integer referred to its "bit" has a maximum value of: 2 ^ (num of bits - 1)
        E.g:
        2 ^ (128 bit - 1) = 170,141,183,460,469,231,731,687,303,715,884,105,728 (51 digits). 
    So the recommended 2048 key length would have the program deal with 308 - 617 digits
    long integers.

    The size or the length of the keys in RSA refer to the size of the modulus "n" (nPublic).
    For example, a 128 bit RSA key means that the modulus "n" (nPublic) is a product of two
    random prime numbers (pPrimePrivate and qPrimePrivate) each not exceeding 64 bit long.

    The size of the modulus "n" determines how secure the key is. A larger modulus "n"
    is exponentially hard to factorize. If an attacker manages to factorize modulus "n"
    into its prime factors p and q (which are kept secret), they can easily calculate 
    the private key using the public key and decrypt the encrypted message. */
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

    /* Prompt user for their desired key size */
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

        /* The chosen index is temporarily stored in chosenKeySize. Once a valid key
        size is chosen, dereference the index in the keySizeOptions[] and store the
        actual key size. */
        if (chosenKeySize > 0 && chosenKeySize <= keySizeOptionsSize) {
            chosenKeySize = keySizeOptions[chosenKeySize-1].size;
            break;
        }

        clearPrompts();
    }

    /* Exit case */
    if (chosenKeySize == 0) {
        return;
    }

    /* Start elapsed-time timer */
    clock_t startTime, endTime;
    double elapsedTime;
    startTime = clock();

    /* Claer previous prompt and output the state of the key generation */
    clearPrompts();

    printf("\nKey length: %d bit",  chosenKeySize);

    printf("\nGenerating: ");
    /* Get the position of the cursor after printing the loading bar title */
    int loadingBarX, loadingBarY;
    getCursorPosition(&loadingBarX, &loadingBarY);

    printf("\nStatus: ");
    /* Get the position of the cursor after printing the loading status title */
    int loadingStatusX, loadingStatusY;
    getCursorPosition(&loadingStatusX, &loadingStatusY);

#ifndef _WIN32
    /* Offset the X coordinate in UNIX systems */
    loadingBarX += strlen("Generating: ");
    loadingStatusX += strlen("Status: ");
#endif


    /* Calculate the length of p prime using the chosen key size (size of modulus n) */
    loadingBar(loadingBarX, loadingBarY, 0);
    loadingStatus(loadingStatusX, loadingStatusY, "Calculating P prime length...");
    pPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));

    /* Calculate the length of q prime using the chosen key size (size of modulus n)

    NOTE: Since this option of the program deals with small but fast successions of
    loading bar percentages. Add a short sleep in between the loading bar updates to
    smoothen the loading bar. */
    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 5);
    loadingStatus(loadingStatusX, loadingStatusY, "Calculating Q prime length...");
    qPrivateLength = ceil((chosenKeySize / 2.0) / log2(10.0));

    /* Calculate the length of e public (public key). Cap the size of e public to not
    be too small if a 16 bit key was chosen */
    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 10);
    loadingStatus(loadingStatusX, loadingStatusY, "Calculating E public length...");
    ePublicLength = pPrivateLength > 3 ? pPrivateLength / 2 : (chosenKeySize / log2(10.0)) - 1;
    
    /* Initialize Bignums */
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

    /* There is a very small chance that the generated keys won't work due to 
    generatePrimeBignum() generating a composite number (For more info, read the
    documenation of generatePrimeBignum() and millerRabinPrimalityTest() in
    src/bignum.c). Thus, the keys need to be tested after generation, if the keys
    fail the test, regenerate new keys. */
    while (1) {
        /* Generate a random p prime */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 30);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating P prime...");
        generatePrimeBignum(&pPrimePrivate, pPrivateLength);

        /* Generate a random q prime */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 35);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating Q prime...");
        generatePrimeBignum(&qPrimePrivate, qPrivateLength);


        /* If the generated q prime is equal to the generated p prime, regenerate
        another q prime */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 40);
        while (isEqualToBignum(&pPrimePrivate, &qPrimePrivate)) {
            generatePrimeBignum(&qPrimePrivate, qPrivateLength);
        }



        /* Calculate modulus "n":
            n = p prime * q qprime */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 50);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating N public...");
        multiplyBignum(&nPublic, &pPrimePrivate, &qPrimePrivate);

        /* Calculate phi of n:
            phi of n = (p prime - 1) * (q qprime - 1) */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 60);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating phi of N...");
        subtractBignum(&pPrimePrivateMinusOne, &pPrimePrivate, &one);
        subtractBignum(&qPrimePrivateMinusOne, &qPrimePrivate, &one);
        multiplyBignum(&phiOfNPrivate, &pPrimePrivateMinusOne, &qPrimePrivateMinusOne);

        /* Calculate e (public key):
            2 < e < phi of n */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 70);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating E public...");
        generatePrimeBignum(&ePublic, ePublicLength);

        /* Calculate d (private key):
            (e * d) mod (phi of n) = 1 */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 80);
        loadingStatus(loadingStatusX, loadingStatusY, "Generating D private...");
        modularInverseBignum(&dPrivate, &ePublic, &phiOfNPrivate);

        /* Once the keys are generated, test the keys to verify that they are able
        to encrypt and decrypt */
        sleepProgram(300);
        loadingBar(loadingBarX, loadingBarY, 85);
        loadingStatus(loadingStatusX, loadingStatusY, "Testing keys...");
        modularExponentiationBignum(&encryptedChar, &plainChar, &ePublic, &nPublic);
        modularExponentiationBignum(&decryptedChar, &encryptedChar, &dPrivate, &nPublic);

        /* If the decrypted character is equal to the plain character, the keys are
        valid, thus exit the loop */
        if (isEqualToBignum(&plainChar, &decryptedChar)) {
            sleepProgram(300);
            loadingBar(loadingBarX, loadingBarY, 90);
            loadingStatus(loadingStatusX, loadingStatusY, "Keys passed the test...");
            break;
        }

        /* If the generate keys failed the test, reset the Bignums and regenerate new keys */
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

    sleepProgram(300);
    loadingBar(loadingBarX, loadingBarY, 100);
    loadingStatus(loadingStatusX, loadingStatusY, "Complete");

    /* End the elapsed-time timer and output the elapsed-time */
    endTime = clock();
    elapsedTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;
    printf("\nGenerated keys in: %.2f seconds", elapsedTime);

    /* Output the generate keys 
    The RSA encryption algorthim requires nPublic along with ePublic or dPrivate
    to encrypt and decrypt respectively. Since we have to output two keys (public
    and private) we have to store nPublic within the keys. The program does so by
    concatinating nPublic to ePublic/dPrivate (separated by a '.').
    
    Equation to encrypt a character:
        encryptedChar = ( plainChar ^ (ePublic) ) mod (npublic)
    Equation to decrypt a character:
        decryptedChar = ( encryptedChar ^ (dPrivate) ) mod (npublic)
    */
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

    /* Prompt user to go back to the main menu */
    promptExitConfirm();
}

void encryptText() {

    /* Initialize file pointers */
    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    /* Default output file name */
    char outputFilename[] = "encrypted.txt";

    /* Open plaintext input file */
    printf("\nAction: Encryption");
    getInputFile(&inputFilePtr, inputFilename, encrypt);

    /* Open output file */
    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        printf("Could not open output file \"%s\". Please try again...", inputFilename);
        exit(1);
    }

    /* Initialize Bignums */
    Bignum nPublic, ePublic, encryptedChar, plainChar;
    initBignum(&ePublic);
    initBignum(&nPublic);
    initBignum(&encryptedChar);
    initBignum(&plainChar);

    /* Get the public key and store it in Bignums */
    getKeys(encrypt, &ePublic, &nPublic);

    /* Start elepased-time timer */
    clock_t startTime, endTime;
    double elapsedTime;
    startTime = clock();

    unsigned long long int characterCount = 0;
    unsigned long long int totalCharactersEncrypted = 0;
    int percentageEncrypted = 0;
    char character;

    /* Get the number of characters in the plaintext file. This number be used as a basis
    to track the percentage of characters encrypted, which will be used in the loading
    bar.  */
    fseek(inputFilePtr, 0, SEEK_END);
    characterCount = ftell(inputFilePtr);
    fseek(inputFilePtr, 0, SEEK_SET);

    /* Get the position of the cursor after printing the loading bar title */
    printf("\nEncryption progress: ");
    int loadingBarX, loadingBarY;
    getCursorPosition(&loadingBarX, &loadingBarY);

    /* Get the position of the cursor after printing the loading status title */
    printf("\nStatus: ");
    int loadingStatusX, loadingStatusY;
    getCursorPosition(&loadingStatusX, &loadingStatusY);

#ifndef _WIN32
    /* Offset the X coordinate in UNIX systems */
    loadingBarX += strlen("Encryption progress: ");
    loadingStatusX += strlen("Status: ");
#endif

    /* Initialize the loading bar and status */
    loadingBar(loadingBarX, loadingBarY, 0);
    loadingStatus(loadingStatusX, loadingStatusY, "Encrypting file...");

    /* Loop through and encrypt each character in the plaintext file */
    while ((character = fgetc(inputFilePtr)) != EOF) {
        /* Convert the character's ASCII value to a Bignum */
        intToBignum(&plainChar, character, positive);

        /* Encrypt the character */
        modularExponentiationBignum(&encryptedChar, &plainChar, &ePublic, &nPublic);

        /* Print the encrypted character and separate each encrypted character
        with a flag */
        for (unsigned long long int i = encryptedChar.length - 1; i > 0; i--) {
            fprintf(outputFilePtr, "%d", encryptedChar.digits[i]);
        }
        fprintf(outputFilePtr, "%d/", encryptedChar.digits[0]);

        /* Reset the following Bignums for the next iteration*/
        resetBignum(&encryptedChar);
        resetBignum(&plainChar);


        /* Update the progress of the loading bar */
        totalCharactersEncrypted++;
        percentageEncrypted = (totalCharactersEncrypted / (float)characterCount) * 100;
        loadingBar(loadingBarX, loadingBarY, percentageEncrypted);
    };

    loadingBar(loadingBarX, loadingBarY, 100);
    loadingStatus(loadingStatusX, loadingStatusY, "Complete");

    /* End elapsed-time timer */
    endTime = clock();
    elapsedTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;

    /* Output details */
    printf("\nEncrypted file in: %.2f seconds", elapsedTime);
    printf("\nCharacters encrypted: %llu", totalCharactersEncrypted);
    printf("\nView the encrypted file at: %s", outputFilename);
    printf("\n\nNote: Make sure you encrypted the file with the recipient's public key, or else they won't be able to decrypt it!");

    /* Free the Bignums and file pointers */
    freeAllBignums();
    fclose(inputFilePtr);
    fclose(outputFilePtr);

    /* Prompt user to go back to the main menu */
    promptExitConfirm();
}

void decryptText() {

    /* Initialize file pointers */
    FILE *inputFilePtr = NULL, *outputFilePtr = NULL;

    char inputFilename[100];
    /* Default output file name */
    char outputFilename[] = "decrypted.txt";

    /* Open encrypted input file */
    printf("\nAction: Decryption");
    getInputFile(&inputFilePtr, inputFilename, decrypt);

    /* Open output file */
    outputFilePtr = fopen(outputFilename, "w");
    if (outputFilePtr == NULL) {
        printf("Could not open output file \"%s\". Please try again...", inputFilename);
        exit(1);
    }

    /* Initialize Bignums */
    Bignum nPublic, dPrivate, decryptedChar, encryptedChar;
    initBignum(&dPrivate);
    initBignum(&nPublic);
    initBignum(&decryptedChar);
    initBignum(&encryptedChar);

    /* Get the private key and store it in Bignums */
    getKeys(decrypt, &dPrivate, &nPublic);

    /* Start elepased-time timer */
    clock_t startTime, endTime;
    double elapsedTime;
    startTime = clock();

    unsigned long long int characterCount = 0;
    unsigned long long int totalCharactersEncrypted = 0;
    int percentageEncrypted = 0;
    char tempCharacter;
    char encryptedCharacter[100];
    char decryptedCharacter;

    /* Get the number of encrypted characters in the input file. This number be
    used as a basis to track the percentage of characters encrypted, which will 
    be used in the loading bar.  */
    while((tempCharacter = fgetc(inputFilePtr)) != EOF) {
        if (tempCharacter == '/') {
            characterCount++;
        }
    }
    rewind(inputFilePtr);

    /* Get the position of the cursor after printing the loading bar title */
    printf("\nDecryption progress: ");
    int loadingBarX, loadingBarY;
    getCursorPosition(&loadingBarX, &loadingBarY);

    /* Get the position of the cursor after printing the loading status title */
    printf("\nStatus: ");
    int loadingStatusX, loadingStatusY;
    getCursorPosition(&loadingStatusX, &loadingStatusY);

#ifndef _WIN32
    /* Offset the X coordinate in UNIX systems */
    loadingBarX += strlen("Decryption progress: ");
    loadingStatusX += strlen("Status: ");
#endif

    /* Initialize the loading bar and status */
    loadingBar(loadingBarX, loadingBarY, 0);
    loadingStatus(loadingStatusX, loadingStatusY, "Decrypting file...");

    /* Loop through and decrypt each character in the encrypted file */
    while (fscanf(inputFilePtr, "%[^/]/", encryptedCharacter) == 1) {
        /* Convert the encrypted character's value to a Bignum */
        setBignum(&encryptedChar, encryptedCharacter, positive);

        /* Decrypt the character */
        modularExponentiationBignum(&decryptedChar, &encryptedChar, &dPrivate, &nPublic);

        /* Convert the decrypted character Bignum to an integer, and store it in a char. 
        NOTE: The decryted character will be an ASCII value */
        decryptedCharacter = bignumToInt(&decryptedChar);

        /* Print the decrypted character */
        fprintf(outputFilePtr, "%c", decryptedCharacter);

        /* Reset the following Bignums for the next iteration*/
        resetBignum(&encryptedChar);
        resetBignum(&decryptedChar);
        encryptedCharacter[0] = '\0';
        decryptedCharacter = '\0';

        /* Update the progress of the loading bar */
        totalCharactersEncrypted++;
        percentageEncrypted = (totalCharactersEncrypted / (float)characterCount) * 100;
        loadingBar(loadingBarX, loadingBarY, percentageEncrypted);
    };

    loadingBar(loadingBarX, loadingBarY, 100);
    loadingStatus(loadingStatusX, loadingStatusY, "Complete");

    /* End elapsed-time timer */
    endTime = clock();
    elapsedTime = (double) (endTime - startTime) / CLOCKS_PER_SEC;

    /* Output details */
    printf("\nDecrypted file in: %.2f seconds", elapsedTime);
    printf("\nCharacters decrypted: %llu", totalCharactersEncrypted);
    printf("\nView the decrypted file at: %s", outputFilename);
    printf("\n\nNote: If the decrypted message doesn't look right, it can either be:");
    printf("\n- The message was not encrypted with your public key");
    printf("\n- You used the wrong key to decrypt the file");

    /* Free the Bignums and file pointers */
    freeAllBignums();
    fclose(inputFilePtr);
    fclose(outputFilePtr);

    /* Prompt user to go back to the main menu */
    promptExitConfirm();
}

void getInputFile(FILE **inputFilePtr, char *inputFilename, Action type) {

    unsigned long long int characterCount = 0;
    char character;
    int isValidFile = 0;
    char errorMessage[100];

    printf("\n");
    int tempCursorX, tempCursorY;
    getCursorPosition(&tempCursorX, &tempCursorY);

    /* Continuously ask user for input file until a valid input file is passed */
    while (1) {
        char *promptMessage = "File name: ";
        printf("%s", promptMessage);
        scanf("%s", inputFilename);

        *inputFilePtr = fopen(inputFilename, "r");

        /* if the file exists, check if it is a valid file */
        if (*inputFilePtr != NULL) {
            /* A valid input file to decrypt is a file that uses the encrypted 
            file format that encryptText() returns. I.e: a file that contains
            numbers separated by a flag. This prevents the user to not decrypt
            encrypted files that was not encrypted by the program. */
            isValidFile = type == decrypt ? isValidEncryptedFile(*inputFilePtr) : 1;

            if (type == decrypt) {
                sprintf(errorMessage, "\"%s\" is not a valid file encrypted by the program...", inputFilename);
            }
        }

        /* If a valid file was inputted, clear any error messages and move the
        cursor to the original position. */
        if (isValidFile) {
            clearWord(terminalHeight - 7, 0, terminalWidth);
            moveCursor(tempCursorX, tempCursorY+1);
            break;
        }

        /* Load the default error message in a buffer. This is done as a format
        specifier is used in the error message. The error message will also change
        depending on the error case that was triggered */
        sprintf(errorMessage, "Could not open \"%s\". Please try again...", inputFilename);

        /* Print error message */
        clearWord(tempCursorY, strlen(promptMessage), terminalWidth);
        clearWord(terminalHeight - 7, 0, terminalWidth);
        moveCursor(0, terminalHeight - 7);
        printf("%*s%s", calculateLeftPadding(strlen(errorMessage)), "", errorMessage);
        moveCursor(tempCursorX, tempCursorY);
        
    }
}

int isValidEncryptedFile(FILE *inputFilePtr) {
    char character;
    /* Look for any invalid characters in the input file */
    while((character = fgetc(inputFilePtr)) != EOF) {
        /* If an invalid character is found, bring the file pointer back
        to the start, and return 0. */
        if (!isdigit(character) && character != '/') {
            rewind(inputFilePtr);
            return 0;
        }
    }
    /* If the encrypted file is a valid file, return 1; */
    rewind(inputFilePtr);
    return 1;
}

void getKeys(Action type, Bignum *ePublicOrDPrivate, Bignum *nPublic) {
    char key[200];
    char firstKey[100];
    char secondKey[100];
    int flagIndex;

    /* Get current cursor position to bring back the cursor back to this 
    point after printing the error messages */
    int tempCursorX, tempCursorY;
    getCursorPosition(&tempCursorX, &tempCursorY);

    /* Continuously ask until a valid key is passed */
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

        /* Verify the characters of the key */
        for (i = 0; i < strlen(key); i++) {
            /* Look for the flag */
            if (key[i] == '.') {
                flagIndex = i;
                flagCount++;
            }

            /* Look for non-digits */
            if (!isdigit(key[i]) && key[i] != '.') {
                flagCount = 0;
                break;
            }
        }

        /* If a valid key was passed, exit the loop */
        if (flagCount == 1) {
            clearWord(terminalHeight - 7, 0, terminalWidth);
            clearWord(terminalHeight - 6, 0, terminalWidth);
            moveCursor(tempCursorX, tempCursorY);
            break;
        }

        /* If an invalid key was passed, print error messages */
        clearWord(tempCursorY, strlen(promptMsg), terminalWidth);
        clearLines(tempCursorY+1, terminalHeight - 8);

        moveCursor(0, terminalHeight - 7);

        printf("%*sInvalid key!", 
        calculateLeftPadding(strlen("Invalid key!")), "");

        printf("\n%*sPlease make sure you properly pasted the key generated by the program.", 
        calculateLeftPadding(strlen("Please make sure you properly copied the key generated by the program.")), "");

        /* Move cursor back to the stored position */
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

    /* Iterate through the string, copying chunks accoring to the line cap,
    and store it in the array of strings. */
    for (int i = 0; i < strlen(inputStr); i += lineCap) {
        /* If the chunk stops in between a word, count the number of characters
        till the end of the word and include it in the string.
        
        E.g:
        
        --------- DONT DO THIS ---------
            the quick brown fox ju
               mps over the laz
                   y dog

        ----------- DO THIS -----------
            the quick brown fox jumps
                 over the lazy 
                       dog
        
        The line cap will only serve as an initially basis of the rough number
        of characters per line. If a word will be chopped in half, where the 
        2nd half will be printed in the next line, just include the 2nd half
        in the 1st half to not break the word. */
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


        /* Copy the evaluated chunk to the array of substrings
        
        How it works:
        
        lineCap = 25;
        chunk = lineCap;
        indexOffset = 0;
        
        NOTE: inputStr will point to the FIRST character of the string.
        After each chunk copy, the pointer will be moved to the starting
        character of the next chunk.

        ---------------------------------------------------------------

        Iteration #1:

                "The quick brown fox jumps over the lazy dog"
                 |                       |
        inputStr + indexOffset           |
                 |                       |
                 THIS CHUNK WILL BE COPIED

        *move the pointer to the start of the next chunk*
        indexOffset += chunk;

        ---------------------------------------------------------------

        Iteration #2:
        
                "The quick brown fox jumps over the lazy dog"
                                        |                  |
                               inputStr + indexOffset
                                        |                  |
                                    THIS CHUNK WILL BE COPIED

        ---------------------------------------------------------------
         */
        strncpy(outputArr[*outputArrCount], inputStr + indexOffset + k, chunkToCopy);

        /* Null terminate the substring */
        outputArr[*outputArrCount][chunkToCopy + numOfCharactersBeforeASpace] = '\0';

        /* Update indices */ 
        indexOffset += chunkToCopy;

        /* Increment number of substrings */
        (*outputArrCount)++;

        /* Reset variables for next iteration */
        chunkToCopy = lineCap;
        j = 0;
        k = 0;
        numOfCharactersBeforeASpace = 0;
    }
}

void about() {
    /* Array of strings that contain the original unformatted about paragraphs
    
    If any modifications are to be made on the paragraphs in the about option
    of the program, do it here. This array of strings will be formatted to
    line wrap the text. */
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

    /* Go through each paragraph and format it according to the line cap.
    I.e: line wrap it to prevent overflow and proper format. Then store it
    in another array of strings.*/
    int lineCap = terminalWidth * 0.7;
    if (lineCap > 100) {
        lineCap = 100;
    }
    for (int i = 0; i < paragraphsSize; i++) {
        splitString(paragraphs[i], paragraphsSubstrings, &paragraphSubstringCount, lineCap);
    }

    /* Calculate the top padding and manually print new lines instead of
    using moveCursor(). This allows the outputs to be scrollable */
    int topPadding = (terminalHeight - paragraphSubstringCount) / 4;
    for (int i = 0; i < topPadding; i++) {
        printf("\n");
    }

    /* Print the format strings */
    for (int i = 0; i < paragraphSubstringCount; i++) {
        printf("\n%*s%s", calculateLeftPadding(strlen(paragraphsSubstrings[i])), "", paragraphsSubstrings[i]);
    }

    char userInput[100];
    int bottomPadding = topPadding;
    int bottomLeftPadding = calculateLeftPadding(strlen("Enter DONE to go back: "));
    
    int tempCursorX, tempCursorY;
    int cursorXToDelete, cursorYToDelete;

    /* Manually print the prompt exit confirm, as promptExitConfirm()
    uses moveCursor() to move the cursor to the bottom of the terminal.
    Using promptExitConfirm() will overwrite any text that reaches the bottom */
    for (int i = 0; i < bottomPadding + 3; i++) {
        printf("\n");
    }
    getCursorPosition(&tempCursorX, &tempCursorY);
    do {
        moveCursor(tempCursorX, tempCursorY-2);
        for (int i = 0; i < bottomLeftPadding; i++) {
            printf(" ");
        }

        printf("Enter DONE to go back: ");
        getCursorPosition(&cursorXToDelete, &cursorYToDelete);
        fgets(userInput, sizeof(userInput), stdin);

        if (userInput[strlen(userInput) - 1] == '\n') {
            userInput[strlen(userInput) - 1] = '\0';
        }

        for(int i = 0; userInput[i]; i++){
            userInput[i] = tolower(userInput[i]);
        }

        clearLines(tempCursorY-2, terminalHeight-1);

    } while (strcmp(userInput, "done") != 0);
}




int calculateLeftPadding(int strLength) {
    /* Function to calculate the left padding needed to center align a print */
    int remainingWidth = terminalWidth - strLength;
    int leftPadding = remainingWidth % 2 ? (remainingWidth + 1) / 2 : remainingWidth / 2;
    return leftPadding;
}

void clearLines(int startLine, int endLine) {
    /* Function to clear lines in the terminal given a starting and ending y-coordinate */
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
    /* Function to clear the prompts of the program. It will essentially clear everything below the program header. */
    clearScreen();
    printProgramHeader();
}

void clearScreen() {
    /* Function to clear the in-view area of the terminal */
#ifdef _WIN32
	system("cls");
#else
	printf("\033[2J");
	printf("\033[H");
#endif
}

void clearWord(int y, int startCol, int endCol) {
    /* Function to clear a specific portion of a line in the terminal */
    moveCursor(startCol, y);
    for (int i = 0; i < endCol - startCol; i++) {
		printf(" ");
    }
}

void getCursorPosition(int *x, int *y) {
    /* Function to get the current position of the cursor */
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
    /* Function to get the terminal size. It also initialize and calculates the length of the loading bar */
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

    /* Loading bar will span the whole terminal width */
    loadingBarLength = terminalWidth - strlen("Encryption progress:   ( 100%% )");
}

void hideCursor() {
    /* Function to hide the cursor */
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
    /* Function to update and display the loading bar
    
    This function takes in the coordinate of where to start printing the loading bar as the cursor will be moving to other position, particularly when the program updates and prints the loading status, and the percentage of the fill. */

    /* Cap the loading bar to prevent the fill from overflowing incase the user passes a percentage greater than 100 */
    if (percentDone > 100) {
        percentDone = 100;
    }

    /* Claculate how much characters of fill and track to print */
	int fill = percentDone * loadingBarLength / 100;
    int track = loadingBarLength - fill;

    /* Move cursor to the specified coordinate */
    moveCursor(x, y);

    /* Hide cursor when loading bar is less than 100. This is done to avoid the flashing effect of the cursor when it continuously moves around the terminal to smoothen the loading bar */
    if (percentDone < 100) {
        hideCursor();
    }

    /* Print the loading bar */ 
    printf("[");
	char a = '=', b = ' ';
	for (int i = 0; i < fill; i++) {
		printf("%c", a);
	}
	for (int i = 0; i < track; i++) {
		printf("%c", b);
	}
    printf("]");
    
    /* Print the percentage done */
	printf(" ( %d%% )", percentDone);
	fflush(stdout);

    /* If loading has complete, bring back the cursor */
    if (percentDone >= 100) {
        showCursor();
    }
}

void loadingStatus(int x, int y, char message[]) {
    /* Function to update the loading status which gives more information about the current action being done */

    /* Clear previous status */
    clearWord(y, x, terminalWidth);
    /* Move cursor to the specified position */
    moveCursor(x, y);
    /* Print the status */
    printf("%s", message);
	fflush(stdout);
}

void moveCursor(int x, int y) {
    /* Function to move the cursor at a specified coordinate in the terminal */
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {x, y};
	SetConsoleCursorPosition(hConsole, pos);
#else
	printf("\033[%d;%dH", y, x);
#endif
}

void printProgramHeader() {
    /* Function to print the program header */

    /* Move cursor at the top of the file */
    moveCursor(0,0);

    /* Print header */
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
    /* Function to show the promp exit comfirm. It will continuously ask the sure to type the confirmation message until it has been typed correctly */

	char userInput[100];

	do {
        /* Clear the previous exit prompt */
		clearLines(terminalHeight - 5, terminalHeight - 5);
		moveCursor((terminalWidth - 21)/ 2, terminalHeight - 5);

        /* Ask user for confirmation */
		printf("Enter DONE to go back: ");
		fgets(userInput, sizeof(userInput), stdin);

		/* Replace the newline character at the end of the input to NULL */ 
		if (userInput[strlen(userInput) - 1] == '\n') {
			userInput[strlen(userInput) - 1] = '\0';
		}

		/* Convert the user's input to lowercase */
		for(int i = 0; userInput[i]; i++){
			userInput[i] = tolower(userInput[i]);
		}

	} while (strcmp(userInput, "done") != 0);
}

void showCursor() {
    /* Function to show the cursor */
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
    /* Function to sleep or add delay to the program */
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}



