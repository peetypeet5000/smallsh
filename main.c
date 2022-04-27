#include "main.h"

bool background_disabled = false;

/*
Checks the provided command line argument, starts functions
to process file & start user interactive menu
*/
int main(int argc, char *argv[])
{
    // Create initially empty enviroment struct and init signals
    struct shell enviroment = {0};
    init_signals();

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

        clean_memory(input);

    }
    
    return EXIT_SUCCESS;
}


void clean_memory(struct command input) {
    for(int i = 0; i < input.arg_count; i++) {
        free(input.args[i]);
    }
}



void init_signals() {
    // Initilize struct for SIGINT
    struct sigaction SIGINT_action = {0};

    // Set to ignore signal, mask all others
    SIGINT_action.sa_handler = SIG_IGN;
    sigfillset(&SIGINT_action.sa_mask);
    SIGINT_action.sa_flags = 0;

    // Set action for SIGINT
    sigaction(SIGINT, &SIGINT_action, NULL);

    // Initilize struct for SIGTSTP
    struct sigaction SIGTSTP_action = {0};
    memset(&SIGTSTP_action, 0, sizeof(SIGTSTP_action));

    // Set to call handler, mask all others
    SIGTSTP_action.sa_handler = SIGINT_handler;
    sigfillset(&SIGTSTP_action.sa_mask);
    SIGTSTP_action.sa_flags = SA_RESTART;

    // Set action for SIGTSTP
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}



void SIGINT_handler() {
    if(background_disabled == false) {
        // Print message with system call to avoid issues
        char* msg = "Entering foreground-only mode (& is now ignored)\n: ";
        write(STDOUT_FILENO, msg, 52);
        // Switch global var
        background_disabled = true;
    } else {
        char* msg = "Exiting foreground-only mode\n: ";
        write(STDOUT_FILENO, msg, 31);
        background_disabled = false;
    }
}
