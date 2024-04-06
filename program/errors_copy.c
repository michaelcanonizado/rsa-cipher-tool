#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>

#define OPTIONS 5

#define clear_screen() write(1, "\033[H\033[2J\033[3J", 11)

#define move_cursor(X, Y) printf("\033[%d;%dH", Y, X)

void get_cols_rows(int *cols, int *rows){

    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);
    *cols = size.ws_col;
    *rows = size.ws_row;

}//tec_get_cols_rows*/

char *global_text;
void win_change(int sig){
    printf("%s\n", global_text);
    printf("win_change happening\n");

}//win_change*/


void print_options(char *global_text, int row_offset, int col_offset){
    int cols = 0, rows = 0;
    get_cols_rows(&cols, &rows);
    row_offset = row_offset + 1;
    move_cursor((cols - col_offset) / 2, rows / 20 + row_offset);
    win_change(0);
}//print_options*/

int main () {
    
    clear_screen();
    signal(SIGWINCH, win_change);

    print_options("enterrr", 1, 10);

    int i, userInput;
    int lengthArr[5], choiceArr[5];

    char * optionsArr[OPTIONS] = {"1) Generate Keys", "2) Encrypt Text", "3) Decrypt Text", "4) About", "5) Exit program"};

    for (i = 0; i < OPTIONS; i++){
        print_options(optionsArr[i], i+2, 10);
    }

    // global_text = "Enter number: ";
    // offset = 0;
    // win_change(0); 

    // c = getchar();
    // int choice = c - '0';

    // char choiceStr[2];  // Buffer to hold the string representation of choice
    // sprintf(choiceStr, "%d", choice);  // Convert choice to a string

    // global_text = "You entered: ";
    // win_change(0);  

    // global_text = choiceStr;
    // win_change(0); 
    

    // // This array will be used to display the temporary outputs of the user's choice. This will be removed in the final version.
    // char * userChoice[5] = {"GENERATING KEYS", "ENCRYPTING TEXT", "DECRYPTING TEXT", "ABOUT US", "EXIT PROGRAM"};
    
    getchar();

    clear_screen();


    return 0;
}