#include <ncurses/ncurses.h>
#include <string.h>

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

    int i;
    int lengthArr[5];
        
    do {
        for ( i = 0; i < 5; i++)
        {
            lengthArr[i] = strlen(optionsArr[i]);
            mvprintw(y++, x - 15, optionsArr[i]);
        }

        noecho(); // Disable echoing of input again
    } while (strcmp(user_input, "exit") != 0 && strcmp(user_input, "EXIT") != 0);
    // End ncurses
    endwin();

    return 0;
}