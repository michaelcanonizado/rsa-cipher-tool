#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#define tc_clear_screen() write(1, "\033[H\033[2J\033[3J", 11)

#define tc_move_cursor(X, Y) printf("\033[%d;%dH", Y, X)

void tc_get_cols_rows(int *cols, int *rows);

#define tc_enter_alt_screen() puts("\033[?1049h\033[H")
#define tc_exit_alt_screen() puts("\033[?1049l")

void tc_echo_off();
void tc_echo_on();

void tc_get_cols_rows(int *cols, int *rows){

	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	*cols = size.ws_col;
	*rows = size.ws_row;

}//tec_get_cols_rows*/

void print_with_word_wrap(const char* str, int terminal_width, int start_column) {
    int column = start_column;
    while (*str) {
        if (*str == '\n') {
            // Reset the column count to the starting column when a newline is encountered
            column = start_column;
            putchar('\n');
            for (int i = 0; i < start_column; i++) {
                putchar(' ');
            }
        } else if (column >= terminal_width) {
            // Insert a newline and reset the column count to the starting column when the terminal width is exceeded
            putchar('\n');
            for (int i = 0; i < start_column; i++) {
                putchar(' ');
            }
            column = start_column;
        }

        putchar(*str);
        column++;
        str++;
    }
}

void win_change(int sig){
    tc_clear_screen();

    int cols = 0, rows = 0;
    tc_get_cols_rows(&cols, &rows);

    int horizontal_position = (cols - 12) / 2;
    int vertical_position = rows / 2;

    tc_move_cursor(horizontal_position, vertical_position);

    // Save the cursor position
    printf("\033[s");

    print_with_word_wrap("Hello, Worghkajshfkjsadhfjksdh fkjashfkjashfjkash fsadkfh ksafhasjk fjkas dhash sd sahgld!\n", cols, horizontal_position);

    // Restore the cursor position
    printf("\033[u");
}

int main(){

    signal(SIGWINCH, &win_change);

    tc_clear_screen();

    int cols = 0, rows = 0;
    tc_get_cols_rows(&cols, &rows);

    int horizontal_position = (cols - 12) / 2;
    int vertical_position = rows / 2;

    tc_move_cursor(horizontal_position, vertical_position);

    // Save the cursor position
    printf("\033[s");

    printf("Hello, Worghkajshfkjsadhfjksdh fkjashfkjashfjkash fsadkfh ksafhasjk fjkas dhash sd sahgld!\n");

    // Restore the cursor position
    printf("\033[u");

    char c = getchar();

    return 0;
}