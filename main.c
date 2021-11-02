#include "main.h"

/*
Checks the provided command line argument, starts functions
to process file & start user interactive menu
*/
int main(int argc, char *argv[])
{
    // Start base-level menu
    bool done = false;
    int selection = 0;

    // Keep looping until user is done
    while(done != true) {
        printf("\n1. Select file to process\n2. Exit the program\nEnter a choice 1 or 2: ");
        scanf ("%d", &selection);

        switch(selection) {
            // Enter movie interface
            case 1:
                file_menu();
                break;
            // Quit the program
            case 2:
                done = true;
                break;
            // Invalid Choice
            default:
                printf("\nYou entered an incorrect choice. Try again.\n");
                break;
        }
    }
    
    return EXIT_SUCCESS;
}