#include <ncurses/ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// compiling format
// cd "c:\Repositories\rsa-cipher-tool\program\" ; if ($?) { gcc program.c -o program -lncurses -DNCURSES_STATIC } ; if ($?) { .\program }

int main() {
    // Initialize ncurses
    initscr();
    refresh();

    // raw();
    // noecho();

    // Get the size of the terminal
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    printw("%d\n", rows);

    // Calculate the center position
    int x = cols / 2;
    int y = rows / 10;

    // Print "Hello World" at the center position
    // mvprintw(y++, x - 6, "Hello World");

    // Box outline for the window
    // box(stdscr, 0, 0);

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

        y = 10;
        switch (userInput)
        {
        case 1:
            clear();
            refresh();
            mvprintw(y, x - choiceArr[0], userChoice[0]);
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

            char key;
            do {
                key = getch(); // Get a single character from the user
            } while (key != 'Y' && key != 'y' && key != 'N' && key != 'n');

            int enter;
            do {
                enter = getch(); // Wait for the user to press ENTER
            } while (enter != 10);

            if (key == 'Y' || key == 'y')
            {
                mvprintw(y++, x - 30, "Keys saved successfully!");
            }
            else
            {
                mvprintw(y++, x - 30, "Keys not saved!");
            }
            clear();
            refresh(); // Refresh the screen to show the changes
            break;
        case 2:
            clear();
            refresh();
            mvprintw(y, x - choiceArr[1], userChoice[1]);
            break;
        case 3:
            clear();
            refresh();
            mvprintw(y, x - choiceArr[2], userChoice[2]);
            break;
        case 4:
            clear();
            refresh();
            mvprintw(y, x - choiceArr[3], userChoice[3]);
            break;      
        default:
            break;
        }

        noecho(); // Disable echoing of input again
    } while (userInput != 5);

    return 0;
}