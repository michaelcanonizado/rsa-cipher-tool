#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>

#define clear_screen() write(1, "\033[H\033[2J\033[3J", 11)
#define move_cursor(X, Y) printf("\033[%d;%dH", Y, X)

void get_cols_rows(int *cols, int *rows);

void get_cols_rows(int *cols, int *rows){
    struct winsize size;
    ioctl(1, TIOCGWINSZ, &size);
    *cols = size.ws_col;
    *rows = size.ws_row;
}

void waitForEnter() {
    int enter;
    do {
        enter = getchar(); // Wait for the user to press ENTER
    } while (enter != 10);
}

char* options[] = {"1) Generate Keys", "2) Encrypt Text", "3) Decrypt Text", "4) About", "5) Exit program"};
int optionCount = sizeof(options) / sizeof(options[0]);
void print_option() {
    int i;
    int cols = 0, rows = 0;
    get_cols_rows(&cols, &rows);

    int horizontal_position = (cols - 10) / 2;

    for ( i = 0; i < optionCount; i++) {
        move_cursor(horizontal_position, rows / 5 + i);
        printf("%s\n", options[i]);
    }
    move_cursor(horizontal_position, rows / 5 + i);
    printf("Enter number: ");
}
void win_options(int sig){
    clear_screen();
    print_option();
}


char* confirm[] = {"You are about to generate an RSA key with this option.", "This will display a private key and public key of the", "generated private key. Do you agree to save a copy of", "your private and public keys? [Y/N]"};
int confirmCount = sizeof(confirm) / sizeof(confirm[0]);
void print_confirm() {
    int i;
    int cols = 0, rows = 0;
    get_cols_rows(&cols, &rows);

    int horizontal_position = (cols - 1)/ 2;

    for ( i = 0; i < confirmCount; i++) {
        move_cursor(horizontal_position , rows / 5 + i);
        printf("%s\n", confirm[i]);
    }
}
void win_confirm(int sig){
    clear_screen();
    print_confirm();
}

int main () {
    clear_screen();

    int cols = 0, rows = 0;
    get_cols_rows(&cols, &rows);

    signal(SIGWINCH, win_options);
    print_option();
    
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        clear_screen();
        signal(SIGWINCH, win_confirm); // Reset the SIGWINCH signal handler to win_confirm
        print_confirm();
        win_confirm(SIGWINCH); // Manually call win_confirm

        break;
    
    default:
        break;
    }

    getchar();

    return 0;
}
