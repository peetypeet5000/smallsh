#include "main.h"

/*
Checks the provided command line argument, starts functions
to process file & start user interactive menu
*/
int main(int argc, char *argv[])
{
    // Start infinite shell argument
    bool done = false;

    struct shell enviroment = init();

    // Keep looping until user is done
    while(enviroment.done != true) {
        // Check for background processes ending
        check_background_processes(&enviroment);

        // Print colon & get input
        struct command input = getCommand();

        // Check for built-in & comments/blanks
        bool built_in = exec_built_in(&input, &enviroment);

        // If not built-in, fork and execute
        if(built_in == false) {
            int running_pid = exec_command(&input, &enviroment);
        }

    }
    
    return EXIT_SUCCESS;
}


struct shell init() {
    struct shell enviroment = {0};

    getcwd(enviroment.cwd, (size_t)2048);

    return enviroment;
}