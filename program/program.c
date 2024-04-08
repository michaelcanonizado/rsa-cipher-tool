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


int main (){
    
  clearScreen();

  int width, height;
	getTerminalSize(&width, &height);
	printf("Width: %d\nHeight: %d\n", width, height);

	// Since the horizontal positioning of the outputs can be adjusted  by incrementing the y-axis, this integer variable stores value of the height divided by 3 as all the outputs starts to be displayed at this value.
	int adjustedHeight = height / 3;
	// This can't be applied to the x-axis as the outputs are displayed at the center of the screen. The x-axis is adjusted by subtracting the length of the string from the width of the screen and dividing the result by 2. It is depedent on the length of the string to be displayed
	



    return 0;
}