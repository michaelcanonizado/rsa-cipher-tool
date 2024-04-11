#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


// ioctl function uses the ioctl header file which is a part of the Unix/Linux standard library and is not available on Windows, which means this program can't run on Windows operating system.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif


void getTerminalSize(int* width, int* height) {
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
	// Preprocessor directive checks if the code is being compiled on a Windows system
	#ifdef _WIN32
		system("cls");
	#else
		// If the code is being compiled on Unix-like systems, the function uses ANSI escape codes to clear the screen
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

// This function waits for the user to enter DONE to avoid the program doing the next thing accidentally.
void waitForDONE(int width, int height) {
	char done[100];
	do {
		clearLines(height - 2, height, width);
		moveCursor((width - 21)/ 2, height - 1);
		printf("Enter DONE to back: ");
		fgets(done, sizeof(done), stdin); // Read a line from stdin

		// Remove the newline character at the end of the input
		done[strcspn(done, "\n")] = 0;

		// If the input was just a newline character, continue with the next iteration
		if (strlen(done) == 0) {
			continue;
		}

		// Convert the user's input to lowercase
		for(int i = 0; done[i]; i++){
			done[i] = tolower(done[i]);
		}

		moveCursor((width - 21)/ 2, height - 1);

	} while (strcmp(done, "done") != 0);
}

char getConfirm(char confirm, int width, int adjustedHeight, int i) {
	confirm = getchar();

	// Check if the input was a newline character
	if (confirm == '\n') {
		confirm = getchar(); // If it was, try to read again
	}

	// Check if the input is valid
	if (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n') {
		moveCursor((width - 37)/ 2, adjustedHeight + i + 1);
		printf("Invalid input. Please enter Y or N.");
		moveCursor((width - 37)/ 2, adjustedHeight + i + 1);
	}

	return confirm;
}

int main (){
  clearScreen();

  int width, height;
	getTerminalSize(&width, &height);
	printf("Width: %d\nHeight: %d\n", width, height);

	// Since the horizontal positioning of the outputs can be adjusted  by incrementing the y-axis, this integer variable stores value of the height divided by 3 as all the outputs starts to be displayed at this value.
	int adjustedHeight = height / 3;
	// This can't be applied to the x-axis as the outputs are displayed at the center of the screen. The x-axis is adjusted by subtracting the length of the string from the width of the screen and dividing the result by 2. It is depedent on the length of the string to be displayed
	
	char * optionsArr[5] = {"1) Generate Keys", "2) Encrypt Text", "3) Decrypt Text", "4) About", "5) Exit program"};

	int i, userInput;
	char confirm;

	do {
		for ( i = 0; i < 5; i++){   
			moveCursor((width - 14)/ 2, adjustedHeight + i);
			printf("%s\n", optionsArr[i]);
		}

		moveCursor((width - 10)/ 2, adjustedHeight + i);
		printf("Enter number: ");
		scanf("%d", &userInput);

		switch (userInput) {
			case 1:
				clearScreen();
				char* confirmGENERATE[] = {"You are about to generate an RSA key", "this option. This will display a private key", "and public key of the generated private key."};
				int GENERATEcount = sizeof(confirmGENERATE) / sizeof(confirmGENERATE[0]);

				for ( i = 0; i < GENERATEcount; i++) {
					moveCursor((width - strlen(confirmGENERATE[i]))/ 2, adjustedHeight + i);
					printf("%s\n", confirmGENERATE[i]);
				}

				do {
					clearLines(height/3 + i, adjustedHeight + i + 1, width);
					moveCursor((width - 37)/ 2, adjustedHeight + i);
					printf("Do you agree to save a copy of your\n");
					moveCursor((width - 32)/ 2, adjustedHeight + i + 1);
					printf("private and public keys? [Y/N] ");
					// The user can press Y or N to confirm or deny the generation of keys

					confirm = getConfirm(confirm, width, adjustedHeight + 1, i);

						// Consume any extra characters in the input buffer
					while (getchar() != '\n');

				} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

				clearScreen();
				if (confirm == 'Y' || confirm == 'y') {
					// At this point, function calls can be made to generate the keys. For now, the program will display a message that the keys are generated.
					moveCursor((width - 30)/ 2, adjustedHeight);
					printf("Keys generated successfully!\n");
				}
				else {
					moveCursor((width - 25)/ 2, adjustedHeight);
					printf("Keys generation failed!\n");
				}

				waitForDONE(width, height);
				clearScreen();
				break;
			case 2:
				clearScreen();
				char* confirmENCRYPT[] = {"Encryption includes the message to be encrypted and", "the public key of the recipient. The txt file of the", "message must be in the same folder of the C program."};
				int ENCRYPTcount = sizeof(confirmENCRYPT) / sizeof(confirmENCRYPT[0]);

				for ( i = 0; i < ENCRYPTcount; i++) {
					moveCursor((width - strlen(confirmENCRYPT[i]))/ 2, adjustedHeight + i);
					printf("%s\n", confirmENCRYPT[i]);
				}

				do {
					clearLines(adjustedHeight + i, adjustedHeight + i, width);
					moveCursor((width - 60)/ 2, adjustedHeight + i);
					printf("Is the txt file in the same folder of the C program? [Y/N] ");
					// The user can press Y or N to confirm or deny the generation of keys

					confirm = getConfirm(confirm, width, adjustedHeight, i);

					// Consume any extra characters in the input buffer
					while (getchar() != '\n');

				} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

				if (confirm == 'Y' || confirm == 'y') {
					do {
						clearLines(adjustedHeight + i + 1, adjustedHeight + i + 2, width);
						moveCursor((width - 47)/ 2, adjustedHeight + i + 1);
						printf("Do you have the recipient's public key? [Y/N] ");
						// The user can press Y or N to confirm or deny the generation of keys

						confirm = getConfirm(confirm, width, adjustedHeight + 1, i);

						// Consume any extra characters in the input buffer
						while (getchar() != '\n');

					} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');	

					clearScreen();  
					moveCursor((width - 20)/ 2, adjustedHeight);
					char filename[100];
					printf("Enter file name: ");
					scanf("%s", filename);
					
					// TO BE CHANGED
					char* publicKEY = malloc(1000000000 * sizeof(char));
					if (publicKEY == NULL) {
							fprintf(stderr, "Failed to allocate memory for publicKEY\n");
							exit(1);
					}
					moveCursor((width - 20)/ 2, adjustedHeight + 1);
					printf("Enter public key: ");
					scanf("%s", publicKEY);

					free(publicKEY);
					
					clearScreen();

					clock_t start = clock();

					// At this point, function calls can be made to ENCRYPT the MESSAGE. For now, the program will display a message that the MESSAGE IS ENCRYPTED.
					moveCursor((width - 30)/ 2, adjustedHeight);
					printf("Message encryption success!\n");
					
					clock_t end = clock();
					double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

					moveCursor((width - 30)/ 2, adjustedHeight + 1);
					printf("Function executed in: %f seconds\n", cpu_time_used);
					moveCursor((width - 30)/ 2, adjustedHeight + 2);
					printf("Encrypted ____ chracters in __ seconds\n");
					// Add time and number of characters	
			}
			else {
				clearScreen();
				moveCursor((width - 25)/ 2, adjustedHeight);
				printf("Message encryption failed!\n");
			}

				waitForDONE(width, height);
				clearScreen();
				break;
			case 3:
				clearScreen();
				char* confirmDECRYPT[] = {"Decryption includes the message to be decrypted and", "the private key of the recipient. The txt file of the", "message must be in the same folder of the C program."};
				int DECRYPTcount = sizeof(confirmDECRYPT) / sizeof(confirmDECRYPT[0]);

				for ( i = 0; i < DECRYPTcount; i++) {
					moveCursor((width - strlen(confirmDECRYPT[i]))/ 2, adjustedHeight + i);
					printf("%s\n", confirmDECRYPT[i]);
				}

				do {
					clearLines(adjustedHeight + i, adjustedHeight + i, width);
					moveCursor((width - 60)/ 2, adjustedHeight + i);
					printf("Is the txt file in the same folder of the C program? [Y/N] ");
					// The user can press Y or N to confirm or deny the generation of keys

					confirm = getConfirm(confirm, width, adjustedHeight, i);

					// Consume any extra characters in the input buffer
					while (getchar() != '\n');

				} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

				// clearScreen();
				if (confirm == 'Y' || confirm == 'y') {
					do {
						clearLines(adjustedHeight + i + 1, adjustedHeight + i + 2, width);
						moveCursor((width - 38)/ 2, adjustedHeight + i + 1);
						printf("Do you have your private key? [Y/N] ");
						// The user can press Y or N to confirm or deny the generation of keys

						confirm = getConfirm(confirm, width, adjustedHeight + 1, i);

						// Consume any extra characters in the input buffer
						while (getchar() != '\n');

					} while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

					if (confirm == 'Y' || confirm == 'y') {

						clearScreen();  
						moveCursor((width - 20)/ 2, adjustedHeight);
						char filename[100];
						printf("Enter file name: ");
						scanf("%s", filename);
						
						// TO BE CHANGED
						char* privateKEY = malloc(1000000000 * sizeof(char));
						if (privateKEY == NULL) {
							printf("Failed to allocate memory for privateKEY\n");
							exit(1);
						}
						moveCursor((width - 20)/ 2, adjustedHeight + 1);
						printf("Enter private key: ");
						scanf("%s", privateKEY);

						free(privateKEY);

						clearScreen();

						clock_t start = clock();

						// At this point, function calls can be made to DECRYPT the MESSAGE. For now, the program will display a message that the MESSAGE IS DECRYPTED.
						moveCursor((width - 30)/ 2, adjustedHeight);
						printf("Message decryption success!\n");
						
						clock_t end = clock();
						double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

						moveCursor((width - 30)/ 2, adjustedHeight + 1);
						printf("Function executed in: %f seconds\n", cpu_time_used);
						moveCursor((width - 30)/ 2, adjustedHeight + 2);
						printf("Decrypted ____ characters in __ seconds\n");
						// Add time and number of characters
					
					}
				}
				else {
					clearScreen();
					moveCursor((width - 25)/ 2, adjustedHeight);
					printf("Message decryption failed!\n");
				}

				waitForDONE(width, height);
				clearScreen();
				break;
			case 4:
				clearScreen();
				moveCursor((width - 45)/ 2, height / 4);
				printf("RSA Cipher Tool with Custom Bignum Library");

				char* about[] = {
					"The RSA Cipher Tool program generates, encrypts, and decrypts messages using the RSA", 
					"algorithm. The program uses the RSA (Rivest, Shamir, Adleman) Algorithm to generate public", 
					"and private keys needed to encrypt and decrypt messages and a custom bignum library to", 
					"handle large numbers numbers for the RSA encryption and decryption processes."};
				int aboutCount = sizeof(about) / sizeof(about[0]);

				for ( i = 0; i < aboutCount; i++) {
					moveCursor((width - strlen(about[i]))/ 2, height / 4 + i + 2);
					printf("%s\n", about[i]);
				}

				
				char* about2[] = {
					"The user will use the program to encrypt a message in a file, where it will then return the", 
					"encrypted/secret message. Then, they can now send it to the recipient of the message safely,",
					"where they can also use the same program to decrypt the message and reveal the actual message."};
				int about2Count = sizeof(about2) / sizeof(about2[0]);
				
				for ( i = 0; i < about2Count; i++) {
					moveCursor((width - strlen(about2[i]))/ 2, height / 4 + i + 7);
					printf("%s\n", about2[i]);
				}

				char* about3[] = { "", "This is a programming project for Computer Programming 2 during the Academic Year", "2023-2024 at the Bicol University College of Science. The program is developed by the", "following students of Bicol University College of Science:", 
					""};
				int about3Count = sizeof(about3) / sizeof(about3[0]);

				for ( i = 0; i < about3Count; i++) {
					moveCursor((width - strlen(about3[i]))/ 2, height / 4 + i + 10);
					printf("%s\n", about3[i]);
				}

				char* members[] = { "Michael Xavier Canonizado", "Deanne Clarice Bea", "Simon Narvaez", "Marc Jordan Campopos"};

				int membersCount = sizeof(members) / sizeof(members[0]);

				for ( i = 0; i < membersCount; i++) {
					moveCursor((width - strlen(members[i]))/ 2, height / 4 + i + 14);
					printf("%s\n", members[i]);
				}

				waitForDONE(width, height);
				clearScreen();
				break;
			default:
				clearScreen();
				moveCursor((width - 20)/ 2, adjustedHeight + i);
				printf("Exiting program...\n");
				sleepProgram(1000);
				break;
		}
	} while (userInput != 5);

    return 0;
}