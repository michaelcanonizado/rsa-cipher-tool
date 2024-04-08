#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


// For Windows in getting the terminal size
// #include <windows.h>

// void getTerminalSize(int* width, int* height) {
// 	CONSOLE_SCREEN_BUFFER_INFO csbi;

// 	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
// 	*width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
// 	*height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
// }

void getTerminalSize(int* width, int* height) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	*width = size.ws_col;
	*height = size.ws_row;
}

void clearScreen() {
	// Write the escape code to stdout
	printf("\033[2J");
	// Optionally, move the cursor to the top-left corner
	printf("\033[H");
}

void clearLines(int startLine, int endLine, int width) {
	for (int i = startLine; i <= endLine; i++) {
		moveCursor(0, i);
		for (int j = 0; j < width; j++) {
			printf(" ");
		}
	}
}

void moveCursor(int x, int y) {
	printf("\033[%d;%dH", y, x);
}

// This function waits for the user to enter DONE to avoid the program doing the next thing accidentally.
void waitForDONE(int width, int height) {
	char done[10];
	do {
		clearLines(height - 1, height + 1, width);
		moveCursor((width - 21)/ 2, height - 1);
		printf("Enter DONE to back: ");
		scanf("%s", done); // Wait for the user to enter "done"
		
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
	
	char * optionsArr[5] = {"1) Generate Keys", "2) Encrypt Text", "3) Decrypt Text", "4) About Us", "5) Exit program"};


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
					moveCursor((width - 30)/ 2, adjustedHeight);
					char filename[100];
					printf("Enter file name: ");
					scanf("%s", filename);
					
					// TO BE CHANGED
					char* publicKEY = malloc(1000000000 * sizeof(char));
					if (publicKEY == NULL) {
							fprintf(stderr, "Failed to allocate memory for publicKEY\n");
							exit(1);
					}
					moveCursor((width - 30)/ 2, adjustedHeight + 1);
					printf("Enter public key: ");
					scanf("%s", publicKEY);

					free(publicKEY);
			}
			else {
				moveCursor((width - 25)/ 2, adjustedHeight);
				printf("Message encryption failed!\n");
			}
				break;
			case 3:
				break;
			case 4:
				break;
			default:
				break;
		}
	} while (userInput != 5);

    return 0;
}