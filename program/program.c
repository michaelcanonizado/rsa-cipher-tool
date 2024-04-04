#include <ncurses/ncurses.h>
#include <string.h>

// compiling format
// gcc printf.c -o printf.exe -lncurses -DNCURSES_STATIC

int main() {
    // Initialize ncurses
    initscr();
    refresh();
    raw();
    noecho(); // Disable echoing of input 

    // Get the size of the terminal
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Calculate the center position
    int x = cols / 2;
    int y = rows / 2;

    // Print "Hello World" at the center position
    mvprintw(y++, x - 6, "Hello World");

    // Box outline for the window
    box(stdscr, 0, 0);

    // In order to capture special keystrokes like Backspace, Delete and the four arrow keys by getch(), you need to call
    keypad(stdscr, TRUE);

    // Get user input
    char user_input[100];
        
    do {
        mvprintw(y, x - 22, "Enter 'exit' to end the program: ");
        echo(); // Enable echoing of input to allow user to see what they're typing
        getstr(user_input);
        noecho(); // Disable echoing of input again
    } while (strcmp(user_input, "exit") != 0 && strcmp(user_input, "EXIT") != 0);
    // End ncurses
    endwin();

    return 0;
}