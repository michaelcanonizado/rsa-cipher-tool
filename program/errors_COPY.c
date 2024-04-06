#include <signal.h>
#include <stdio.h>
#include <unistd.h>  // for sleep function

volatile sig_atomic_t flag = 0;

void handle_signal(int sig) {
    switch (sig) {
        case SIGINT:
            printf("Caught signal SIGINT\n");
            flag = 1;
            break;
        case SIGTERM:
            printf("Caught signal SIGTERM\n");
            flag = 1;
            break;
    }
}

int main() {

    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Keep running until we receive a signal
    while (flag == 0) {
        printf("Running...\n");
        sleep(1);  // Sleep for 1 second
    }

    printf("Program terminated.\n");

    return 0;
}