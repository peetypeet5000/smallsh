#include "main.h"

/*
Checks the provided command line argument, starts functions
to process file & start user interactive menu
*/
int main(int argc, char *argv[])
{
    // Start infinite shell argument
    bool done = false;

    // Keep looping until user is done
    while(done != true) {
        // Check for background processes ending

        // Print colon & get input
        struct command input = getCommand();

        // Check for built-in & comments/blanks
        bool built_in = exec_built_in(input)

        // If not, fork and exec
        //if(built_in = false)

        // If not forked, waitPID
    }
    
    return EXIT_SUCCESS;
}