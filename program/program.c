#include <stdio.h>
#include <stdlib.h>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// This function waits for the user to press ENTER to avoid the program from closing immediately after the user's input
void waitForEnter() {
    int enter;
    do {
        enter = getchar(); // Wait for the user to press ENTER
    } while (enter != 10);
}

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