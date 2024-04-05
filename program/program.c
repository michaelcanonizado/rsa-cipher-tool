#include <ncurses/ncurses.h>
#include <string.h>
#include <stdlib.h>

// compiling format
// cd "c:\Repositories\rsa-cipher-tool\program\" ; if ($?) { gcc program.c -o program -lncurses -DNCURSES_STATIC } ; if ($?) { .\program }

// This function waits for the user to press ENTER to avoid the program from closing immediately after the user's input
void waitForEnter() {
    int enter;
    do {
        enter = getch(); // Wait for the user to press ENTER
    } while (enter != 10);
}

int main() {
    // Initialize ncurses
    initscr();
    refresh();

    // Get the size of the terminal
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Calculate the center position
    int x = cols / 2;
    int y = 10;

    // Box outline for the window
    box(stdscr, 0, 0);

    // In order to capture special keystrokes like Backspace, Delete and the four arrow keys by getch(), you need to call
    keypad(stdscr, TRUE);

    // Get user input
    char user_input[100];
    
    char * optionsArr[5] = {"1) Generate Keys", "2) Encrypt Text", "3) Decrypt Text", "4) About", "5) Exit program"};

    // This array will be used to display the temporary outputs of the user's choice. This will be removed in the final version.
    char * userChoice[5] = {"GENERATING KEYS", "ENCRYPTING TEXT", "DECRYPTING TEXT", "ABOUT US", "EXIT PROGRAM"};

    int i, userInput;
    int lengthArr[5], choiceArr[5];
        
    do {
        y = 10;
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

        // This resets the value of y so that the text is displayed at the same position of the previous outputs, otherwise it will be displayed at the bottom of the screen
        y = 10;
        switch (userInput)
        {
        case 1:
            // Clear function clears the screen so when the user presses certain inputs, the screen will be cleared and the text can be displayed at the same position of the screen
            clear();
            refresh();
            mvprintw(y, x - choiceArr[0], userChoice[0]);
            // The consecutive print statements asks for confirmation from the user to generate the keys. The value of y is incremented by 1 so that the consecutive print statements won't overlap each other.
            mvprintw(y++, x - 20, "You are about to generate an RSA private key");
            mvprintw(y++, x - 20, "with this option. Additionally, it will display");
            mvprintw(y++, x - 20, "the public key of a generated private key.");
            mvprintw(y++, x - 20, "Do you agree to save a copy of your private");
            mvprintw(y++, x - 20, "and public keys? [Y/N]");
            refresh();

            char confirm;
            // The user can press Y or N to confirm or deny the generation of keys
            do {
                confirm = getch(); // Get a single character from the user
            } while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n');

            waitForEnter();
            clear();
            refresh();

            if (confirm == 'Y' || confirm == 'y') {
                // At this point, function calls can be made to generate the keys. For now, the program will display a message that the keys are generated.
                mvprintw(y, x - 8, "Keys generated!");
                refresh();
            }
            else {
                mvprintw(y, x - 12, "Keys generation failed!");
                refresh();
            }

            waitForEnter();
            clear();
            refresh();
            break;
        case 2:
            clear();
            refresh();
            mvprintw(y, x - choiceArr[1], userChoice[1]);
            clear();
            refresh();
            break;
        case 3:
            clear();
            refresh();
            mvprintw(y, x - choiceArr[2], userChoice[2]);
            clear();
            refresh();
            break;
        case 4:
            clear();
            refresh();
            mvprintw(y, x - choiceArr[3], userChoice[3]);
            clear();
            refresh();
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