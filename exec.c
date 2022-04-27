#include "exec.h"


int exec_command(struct command* input, struct shell* enviroment) {
    // Fork the parent process
    pid_t pid = fork();

    // If this is the parent (pid is not 0), wait for the child
    if((int)pid > 0 && (input->background == false || background_disabled == true)) {
        int exit_status;
        waitpid(pid, &exit_status, 0);

        // If ended because of a signal, print that & get signal number
        if(WIFSIGNALED(exit_status)) {
            printf("terminated by signal %d\n", WTERMSIG(exit_status));
            fflush(stdout);
            enviroment->last_exit = WTERMSIG(exit_status);
        // Otherwise, save the exit code for status
        } else {
            enviroment->last_exit = WEXITSTATUS(exit_status);
        }


    // If it's a background process, save to array but don't wait
    } else if((int)pid > 0) {
        // Save the processes PID
        printf("bacground pid is %d\n", pid);
        fflush(stdout);
        enviroment->background_processes[enviroment->num_background_processes] = (int)pid;
        enviroment->num_background_processes++;
    // If the child, exec child program
    } else if (pid == 0){
        // First, process redirection
        bool redir_sucess = process_redirection(input);

        // If file redirection worked, continue
        if(redir_sucess == true) {
            // Set signal handlers
            set_child_signals(input);
            // Exec child process
            int valid = execvp(input->args[0], input->args);

            // If exec returns a -1, there was an error
            if(valid == -1) {
                perror("execvp");
                enviroment->last_exit = 1;
                _Exit(1);
            }
        // If redirection fails, exit too
        } else {
            _Exit(1);
        }
        
    // Else, there was an error
    } else {
        perror("fork");
        exit(-1);
    }

    return (int)pid;
}


bool process_redirection(struct command* input) {
    // If there is a value for input redirection, redirect it
    if(strlen(input->input_redirection) > 0) {
        int input_fd = open(input->input_redirection, O_RDONLY, 00740);

        // If file failed to open, print so and return false
        if (input_fd == -1) {
            printf("can not open %s for input\n", input->input_redirection);
            fflush(stdout);
            return false;
        }
        dup2(input_fd,0);
    }

    // Same for output redirection
    if(strlen(input->output_redirection) > 0) {
        int output_fd = open(input->output_redirection, O_WRONLY | O_CREAT, 00740);
        if (output_fd == -1) {
            printf("can not open %s for input\n", input->output_redirection);
            fflush(stdout);
            return false;
        }
        dup2(output_fd,1);
    }

    // If no errors, return true
    return true;
}



void check_background_processes(struct shell* enviroment) {
    for(int i = 0; i < enviroment->num_background_processes; i++) {
        // Ensure the PID hasn't already been waited and set back to 0
        if(enviroment->background_processes[i] != 0) {
            // Wait the process without hangning
            int exit_code;
            int done = waitpid(enviroment->background_processes[i], &exit_code, WNOHANG);

            // If the process is actually done, it will return the positive PID
            if(done > 0) { 
                // If done because of a signal, print the signal
                if(WIFSIGNALED(exit_code)) {
                    printf("background pid %d is done. terminated by signal %d\n", enviroment->background_processes[i], WTERMSIG(exit_code));
                    fflush(stdout);
                // Otherwise, print that it exited normally
                } else {
                    printf("background pid %d is done. exit value %d\n", enviroment->background_processes[i], WEXITSTATUS(exit_code));
                    fflush(stdout);
                }

                // either way, set to 0 so not checked again
                enviroment->background_processes[i] = 0;
            }
        }
    }
}



bool exec_built_in(struct command* input, struct shell* enviroment) {
    // Check the first argument for one of the built-in commands
    if(strncmp(input->args[0], "exit", 4) == 0) {
        exit_shell(enviroment);
    } else if (strncmp(input->args[0], "cd", 2) == 0) {
        change_directory(input);
    } else if (strncmp(input->args[0], "status", 6) == 0) {
        status(enviroment);
    } else if (input->args[0][0] == '#') { // Also check for line comments to ignore
        // ignore
    }
    else {
        return false;
    }

    // If one of the built-ins was executed, return true
    return true;
}


// Handles the "cd" built-in Command
// If no argument provided, switches to $HOME
// Otherwise, changes working dir to the argument
void change_directory(struct command* input) {
    // If additial argument to cd provided, change to that directory
    if(input->arg_count > 1) {
        chdir(input->args[1]);
    // Otherwise, get $HOME and set that is directory
    } else {
        // Get $HOME enviroment variable
        char* home_dir = getenv("HOME");
        chdir(home_dir);
    }
}


// Prints the last exit value which is stored
// in the enviroment struct
void status(struct shell* enviroment) {
    // Print the last exit value
    printf("exit value %d\n", enviroment->last_exit);
    // Always fliush the output
    fflush(stdout);
}


// Kills remaining processes before setting
// value that will end loop
void exit_shell(struct shell* enviroment) {
    int exit;

    // Kill all remiaining processes
    for(int i = 0; i < enviroment->num_background_processes; i++) {
        if(enviroment->background_processes[i] != 0) {
            // Send SIGTERM to all open background processes
            kill(enviroment->background_processes[i], SIGTERM);
            // Wait to clean mem
            waitpid(enviroment->background_processes[i], &exit, 0);
        }
    }

    enviroment->done = true;
}


// Set SIGINT back to default behavior for child
// Set SIGSTP to ignore for all children
void set_child_signals(struct command* input) {
    // Only set SIGINT to not ignore if it's foreground
    if(input->background == false || background_disabled == true) {
        // Initilize struct for SIGINT
        struct sigaction SIGINT_action = {0};

        // Set to ignore signal, mask all others
        SIGINT_action.sa_handler = SIG_DFL;
        sigfillset(&SIGINT_action.sa_mask);
        SIGINT_action.sa_flags = 0;

        // Set action for SIGINT
        sigaction(SIGINT, &SIGINT_action, NULL);
    }

    // Set SIGSTP back to ignore
    struct sigaction SIGTSTP_action = {0};

    // Set to ignore, mask all others
    SIGTSTP_action.sa_handler = SIG_IGN;
    sigfillset(&SIGTSTP_action.sa_mask);
    SIGTSTP_action.sa_flags = 0;

    // Set action for SIGTSTP
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}