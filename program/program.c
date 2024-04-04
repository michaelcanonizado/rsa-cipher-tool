#include <ncurses/ncurses.h>
#include <string.h>
#include <stdlib.h>

// compiling format
// gcc printf.c -o printf.exe -lncurses -DNCURSES_STATIC

int main() {
    // Initialize ncurses
    initscr();
    refresh();

    // Get the size of the terminal
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Calculate the center position
    int x = cols / 2;
    int y = rows / 2;

    // Box outline for the window
    box(stdscr, 0, 0);

    // In order to capture special keystrokes like Backspace, Delete and the four arrow keys by getch(), you need to call
    keypad(stdscr, TRUE);

    // Get user input
    char user_input[100];
    
    char * optionsArr[5] = {"1) Generate Keys", "2) Encrypt Text", "3) Decrypt Text", "4) About", "5) Exit program"};

    char * userChoice[5] = {"GENERATING KEYS", "ENCRYPTING TEXT", "DECRYPTING TEXT", "ABOUT US", "EXIT PROGRAM"};

    int i, userInput;
    int lengthArr[5], choiceArr[5];
        
    do {
        for ( i = 0; i < 5; i++)
        {
            lengthArr[i] = strlen(optionsArr[i]);
            mvprintw(y++, x - 15, optionsArr[i]);
        }
        
        // Enable echoing of input to allow user to see what they're typing
        echo(); 
        mvprintw(y++, x - 10, "Enter number: ");
        
        getstr(user_input);
        userInput = atoi(user_input);

        int choiceArr[5];
        for ( i = 0; i < 5; i++)
            choiceArr[i] = strlen(userChoice[i]);

        switch (userInput)
        {
        case 1:
            mvprintw(y++, x - choiceArr[0], userChoice[0]);
            refresh();
            mvprintw(y++, x - 20, "You are about to generate an RSA private key");
            refresh();
            mvprintw(y++, x - 20, "with this option. Additionally, it will display");
            refresh();
            mvprintw(y++, x - 20, "the public key of a generated private key.");
            refresh();
            mvprintw(y++, x - 20, "Do you agree to save a copy of your private");
            refresh();
            mvprintw(y++, x - 20, "and public keys? [Y/N]");

            break;
        case 2:
            mvprintw(y++, x - choiceArr[1], userChoice[1]);
            break;
        case 3:
            mvprintw(y++, x - choiceArr[2], userChoice[2]);
            break;
        case 4:
            mvprintw(y++, x - choiceArr[3], userChoice[3]);
            break;      
        default:
            break;
        }
        
        noecho(); // Disable echoing of input again
    } while (userInput != 5);

    // End ncurses
    endwin();

    return 0;
}