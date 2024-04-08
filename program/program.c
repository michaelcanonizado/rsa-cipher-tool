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

int main (){
    
    clearScreen();

    




    return 0;
}