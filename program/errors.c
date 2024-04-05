#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>

void tc_get_cols_rows(int *cols, int *rows){

	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	*cols = size.ws_col;
	*rows = size.ws_row;

}//tec_get_cols_rows*/




int main () {

    int cols = 0, rows = 0;
    get_cols_rows(&cols, &rows);

    move_cursor ((cols - 12) / 2, rows / 2);
    printf("Hello World!\n");


    return 0;
}
