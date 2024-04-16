#include <stdio.h>
#include <unistd.h>

// Function to print the indeterminate progress bar
void printIndeterminateProgressBar() {
    char progressChars[] = {'|', '/', '-', '\\'};
    int i = 0;
    
    while (1) {
        printf("\r%c", progressChars[i]); // Print the progress character
        fflush(stdout); // Flush the output buffer
        
        i = (i + 1) % 4; // Rotate through the progress characters
        
        usleep(200000); // Sleep for 200 milliseconds (adjust as needed)
    }
}

// Your other function that runs concurrently with the progress bar
void otherFunction() {
    // Example of another function
    for (int i = 0; i < 10; ++i) {
        printf("Doing some work... %d\n", i);
        sleep(1); // Simulate some work being done
    }
}

int main() {
    pid_t pid = fork(); // Create a new process
    
    if (pid == -1) {
        // Fork failed
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        printIndeterminateProgressBar();
    } else {
        // Parent process
        otherFunction();
    }
    
    return 0;
}
