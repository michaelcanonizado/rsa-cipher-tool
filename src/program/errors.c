#include <stdio.h>

void print_progress(size_t count, size_t max) {
    const int bar_width = 50;

    float progress = (float) count / max;
    int bar_length = progress * bar_width;

    printf("\rProgress: [");
    for (int i = 0; i < bar_length; ++i) {
        printf("#");
    }
    for (int i = bar_length; i < bar_width; ++i) {
        printf(" ");
    }
    printf("] %.2f%%", progress * 100);

    fflush(stdout);
}

int main() {
    for (size_t i = 0; i <= 100; ++i) {
        print_progress(i, 100);
    }
    printf("\n");

    return 0;
}

#include <stdio.h> 
#include <windows.h> 

// Function to creating loading bar 
void loadingBar(void (*task)(int*), int* taskProgress) 
{ 
    // 0 - black background, 
    // A - Green Foreground 
    system("color 0A"); 

    // Initialize char for printing 
    // loading bar 
    char a = 177, b = 219; 

    printf("\n\n\n\n\t\t\t\t\tLoading...\n\n"); 
    printf("\t\t\t\t\t"); 

    // Print initial loading bar 
    for (int i = 0; i < 26; i++) 
        printf("%c", a); 

    // Set the cursor again starting 
    // point of loading bar 
    printf("\r"); 
    printf("\t\t\t\t\t"); 

    // Print loading bar progress 
    for (int i = 0; i < 26; i++) { 
        printf("%c", b); 

        // Run the task and update the progress
        task(taskProgress);

        // Sleep for 1 second 
        Sleep(1000); 
    } 
} 

// Example task function
void exampleTask(int* progress) {
    // Do some work here...

    // Update the progress
    (*progress)++;
}

// Driver Code 
int main() 
{ 
    int progress = 0;

    // Function Call 
    loadingBar(exampleTask, &progress); 

    // Reset the color of the terminal text to the default
    system("color 07");

    return 0; 
}