#include <stdio.h>
#include <stdlib.h>

void clearScreen() {
	// Write the escape code to stdout
	printf("\033[2J");
	// Optionally, move the cursor to the top-left corner
	printf("\033[H");
}

void moveCursor(int x, int y) {
	printf("\033[%d;%dH", y, x);
}


int main (){
    
    clearScreen();

    




    return 0;
}