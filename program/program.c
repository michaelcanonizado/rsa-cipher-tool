#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif

// Function prototypes
void getTerminalSize(int *width, int *height);
void clearScreen();
void sleepProgram(int milliseconds);
void moveCursor(int x, int y);
void clearLines(int startLine, int endLine, int width);
void waitForInput(char *message);
char getConfirm();
int displayMessage(char *message[], int count, int width, int height);
void displayAbout(char *message[], int count, int width, int height, int offsestY);
void generateKeys(int width, int height, int i);
void encryptText(int width, int height);
void decryptText(int width, int height);
void aboutProgram(int width, int height);

char confirm;
int offsetY;
char fileName[100];
FILE *file;

int main() {
	clearScreen();

	int width, height;
	getTerminalSize(&width, &height);
	printf("Width: %d\nHeight: %d\n", width, height);

	int adjustedHeight = height / 3;
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
		while (getchar() != '\n'); // Clear input buffer

		switch (userInput) {
			case 1:
				generateKeys(width, adjustedHeight, i);
				break;
			case 2:
				encryptText(width, adjustedHeight);
				break;
			case 3:
				decryptText(width, adjustedHeight);
				break;
			case 4:
				aboutProgram(width, height);
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

void getTerminalSize(int *width, int *height) {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	*width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	*height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*width = size.ws_col;
	*height = size.ws_row;
#endif
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

void waitForDone(int width, int height) {
char done[100];
do {
	clearLines(height - 2, height - 2, width);
	moveCursor((width - 21)/ 2, height - 2);
	printf("Enter DONE to back: ");
	fgets(done, sizeof(done), stdin); // Read a line from stdin

	// Remove the newline character at the end of the input
	if (done[strlen(done) - 1] == '\n') {
		done[strlen(done) - 1] = '\0';
	}

	// If the input was just a newline character, continue with the next iteration
	if (strlen(done) == 0) {
		continue;
	}

	// Convert the user's input to lowercase
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

int displayMessage(char *message[], int count, int width, int adjustedHeight) {
	int i;
	for ( i = 0; i < count; i++) {
		moveCursor((width - strlen(message[i])) / 2, adjustedHeight + i);
		printf("%s\n", message[i]);
	}

	return i;
}

void displayAbout(char *message[], int count, int width, int height, int offsetY) {
	for (int i = 0; i < count; i++) {
		moveCursor((width - strlen(message[i])) / 2, height / 4 + i + offsetY);
		printf("%s\n", message[i]);
	}
}

void generateKeys(int width, int adjustedHeight, int i) {
	clearScreen();
	char *msgGenerate[] = {"You are about to generate an RSA key", "this option. This will display a private key", "and public key of the generated private key."};
	int countGenerate = sizeof(msgGenerate) / sizeof(msgGenerate[0]);
	offsetY = displayMessage(msgGenerate, countGenerate, width, adjustedHeight);

	do{
		clearLines(adjustedHeight + offsetY, adjustedHeight + offsetY + 1, width);
		moveCursor((width - 37)/ 2, adjustedHeight + offsetY);
		printf("Do you agree to save a copy of your\n");
		moveCursor((width - 32)/ 2, adjustedHeight + offsetY + 1);
		printf("private and public keys? [Y/N] ");
		// The user can press Y or N to confirm or deny the generation of keys
		confirm = getConfirm(width, adjustedHeight, offsetY + 1);

	} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

	clearScreen();	
	if (confirm == 'Y' || confirm == 'y') {
		// At this point, function calls can be made to generate the keys. For now, the program will display a message that the keys are generated.
		moveCursor((width - 30)/ 2, adjustedHeight);
		printf("Keys generated successfully!\n");
	} else {
		moveCursor((width - 25)/ 2, adjustedHeight);
		printf("Keys generation failed!\n");
	}

	waitForDone(width, adjustedHeight * 3);
	clearScreen();
}

void encryptText(int width, int adjustedHeight) {
	char* publicKEY = malloc(1000000000 * sizeof(char));
	
	clearScreen();
	char* msgEncrypt[] = {"Encryption includes the message to be encrypted and", "the public key of the recipient. The txt file of the", "message must be in the same folder of the C program."};
	int countEncrypt = sizeof(msgEncrypt) / sizeof(msgEncrypt[0]);
	offsetY = displayMessage(msgEncrypt, countEncrypt, width, adjustedHeight);

	do {
		clearLines(adjustedHeight + offsetY, adjustedHeight + offsetY, width);
		moveCursor((width - 60)/ 2, adjustedHeight + offsetY);
		printf("Is the txt file in the same folder of the C program? [Y/N] ");
		// The user can press Y or N to confirm or deny the encryption of the message
		confirm = getConfirm(width, adjustedHeight, offsetY + 1);

	} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

	if (confirm == 'Y' || confirm == 'y') {
		do {
			clearLines(adjustedHeight + offsetY + 1, adjustedHeight + offsetY + 2, width);
			moveCursor((width - 47)/ 2, adjustedHeight + offsetY + 1);
			printf("Do you have the recipient's public key? [Y/N] ");
			// The user can press Y or N to confirm or deny the encryption of the message
			confirm = getConfirm(width, adjustedHeight, offsetY + 2);

		} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

		if (confirm == 'Y' || confirm == 'y') {
			clearScreen();

			do {
				clearLines(adjustedHeight, adjustedHeight, width);
				moveCursor((width - 38)/ 2, adjustedHeight);
				printf("Enter file name: ");
				fgets(fileName, sizeof(fileName), stdin);

				// Remove the newline character at the end of the filename
				if (fileName[strlen(fileName) - 1] == '\n') {
					fileName[strlen(fileName) - 1] = '\0';
				}
		
				// Try to open the file for reading and writing
				file = fopen(fileName, "r+");
		
				if (file == NULL) {
					// If the file could not be opened, print an error message
					moveCursor((width - 38)/ 2, adjustedHeight + offsetY + 3);
					printf("Could not open file. Please try again.\n");
				}
			} while (file == NULL);
		
			// At this point, 'file' is a pointer to the opened file
			// You can use 'file' with functions like fprintf(), fscanf(), etc. to read from and write to the file


			// TO BE CHANGED
			if (publicKEY == NULL) {
					fprintf(stderr, "Failed to allocate memory for publicKEY\n");
					exit(1);
			}
			moveCursor((width - 38)/ 2, adjustedHeight + 1);
			printf("Enter public key: ");
			scanf("%s", publicKEY);

			clearScreen();

			clock_t start = clock();

			// At this point, function calls can be made to ENCRYPT the MESSAGE. For now, the program will display a message that the MESSAGE IS ENCRYPTED.

			fprintf(file, "Encrypted message here\n");
			fprintf(file, "%s", publicKEY);
			printf("Public key: %s\n", publicKEY);
			
			clock_t end = clock();
			double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

			moveCursor((width - 30)/ 2, adjustedHeight);
			printf("Message encryption success!\n");
			moveCursor((width - 30)/ 2, adjustedHeight + 1);
			printf("Function executed in: %f seconds\n", cpu_time_used);
			moveCursor((width - 30)/ 2, adjustedHeight + 2);
			printf("Encrypted ____ characters in __ seconds\n");

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

	waitForDone(width, adjustedHeight * 3);
	clearScreen();

	free(publicKEY);
	fclose(file);
}

void decryptText(int width, int adjustedHeight) {
	char* privateKEY = malloc(1000000000 * sizeof(char));

	clearScreen();
	char* msgDecrypt[] = {"Decryption includes the message to be decrypted and", "the private key of the recipient. The txt file of the", "message must be in the same folder of the C program."};
	int countDecrypt = sizeof(msgDecrypt) / sizeof(msgDecrypt[0]);
	offsetY = displayMessage(msgDecrypt, countDecrypt, width, adjustedHeight);

	do {
		clearLines(adjustedHeight + offsetY, adjustedHeight + offsetY, width);
		moveCursor((width - 60)/ 2, adjustedHeight + offsetY);
		printf("Is the txt file in the same folder of the C program? [Y/N] ");
		// The user can press Y or N to confirm or deny the encryption of the message
		confirm = getConfirm(width, adjustedHeight, offsetY);

	} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

	if (confirm == 'Y' || confirm == 'y') {
		do {
			clearLines(adjustedHeight + offsetY + 1, adjustedHeight + offsetY + 2, width);
			moveCursor((width - 38)/ 2, adjustedHeight + offsetY + 1);
			printf("Do you have your private key? [Y/N] ");
			// The user can press Y or N to confirm or deny the encryption of the message
			confirm = getConfirm(width, adjustedHeight, offsetY + 1);

		} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

		if (confirm == 'Y' || confirm == 'y') {
			clearScreen();

			do {
				clearLines(adjustedHeight, adjustedHeight, width);
				moveCursor((width - 38)/ 2, adjustedHeight);
				printf("Enter file name: ");
				fgets(fileName, sizeof(fileName), stdin);

				// Remove the newline character at the end of the filename
				if (fileName[strlen(fileName) - 1] == '\n') {
					fileName[strlen(fileName) - 1] = '\0';
				}
		
				// Try to open the file for reading and writing
				file = fopen(fileName, "r+");
		
				if (file == NULL) {
					// If the file could not be opened, print an error message
					moveCursor((width - 38)/ 2, adjustedHeight + offsetY + 3);
					printf("Could not open file. Please try again.\n");
				}
			} while (file == NULL);

			// At this point, 'file' is a pointer to the opened file
			// You can use 'file' with functions like fprintf(), fscanf(), etc. to read from and write to the file

			// TO BE CHANGED
			if (privateKEY == NULL) {
					fprintf(stderr, "Failed to allocate memory for privateKEY\n");
					exit(1);
			}
			moveCursor((width - 38)/ 2, adjustedHeight + 1);
			printf("Enter private key: ");
			scanf("%s", privateKEY);

			clearScreen();

			clock_t start = clock();

			// At this point, function calls can be made to ENCRYPT the MESSAGE. For now, the program will display a message that the MESSAGE IS ENCRYPTED.

			fprintf(file, "Decrypted message here\n");
			fprintf(file, "%s", privateKEY);
			printf("Private key: %s\n", privateKEY);
			
			clock_t end = clock();
			double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

			moveCursor((width - 30)/ 2, adjustedHeight);
			printf("Message encryption success!\n");
			moveCursor((width - 30)/ 2, adjustedHeight + 1);
			printf("Function executed in: %f seconds\n", cpu_time_used);
			moveCursor((width - 30)/ 2, adjustedHeight + 2);
			printf("Decrypted ____ characters in __ seconds\n");

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

	waitForDone(width, adjustedHeight * 3);
	clearScreen();

	free(privateKEY);
	fclose(file);
}

void aboutProgram(int width, int height) {
	clearScreen();
	moveCursor((width - 45) / 2, height / 4);
	printf("RSA Cipher Tool with Custom Bignum Library");

	char *about[] = {"The RSA Cipher Tool program generates, encrypts, and decrypts messages using the RSA", "algorithm. The program uses the RSA (Rivest, Shamir, Adleman) Algorithm to generate public", "and private keys needed to encrypt and decrypt messages and a custom bignum library to", "handle large numbers numbers for the RSA encryption and decryption processes."};
	int aboutCount = sizeof(about) / sizeof(about[0]);
	displayAbout(about, aboutCount, width, height, 2);

	char* about2[] = {"The user will use the program to encrypt a message in a file, where it will then return the", "encrypted/secret message. Then, they can now send it to the recipient of the message safely,", "where they can also use the same program to decrypt the message and reveal the actual message."};
	int about2Count = sizeof(about2) / sizeof(about2[0]);
	displayAbout(about2, about2Count, width, height, 7);

	char* about3[] = {"This is a programming project for Computer Programming 2 during the Academic Year", "2023-2024 at the Bicol University College of Science. The program is developed by the", "following students of Bicol University College of Science:"};
	int about3Count = sizeof(about3) / sizeof(about3[0]);
	displayAbout(about3, about3Count, width, height, 11);

	char* members[] = { "Michael Xavier Canonizado", "Deanne Clarice Bea", "Simon Narvaez", "Marc Jordan Campopos"};
	int membersCount = sizeof(members) / sizeof(members[0]);
	displayAbout(members, membersCount, width, height, 14);

	waitForDone(width, height);
	clearScreen();
}
