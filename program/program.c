#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>



void clearScreen() {
	// Write the escape code to stdout
	printf("\033[2J");
	// Optionally, move the cursor to the top-left corner
	printf("\033[H");
}

void moveCursor(int x, int y) {
	printf("\033[%d;%dH", y, x);
}

void getTerminalSize(int* width, int* height) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	*width = size.ws_col;
	*height = size.ws_row;
}


int main (){
    
    clearScreen();

    




    return 0;
}