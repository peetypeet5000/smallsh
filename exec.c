#include "exec.h"


int exec_command(struct command* input, struct shell* enviroment) {
    // Fork the parent process
    pid_t pid = fork();

    // If this is the parent (pid is not 0), wait for the child
    if((int)pid > 0 && input->background == false) {
        waitpid(pid, &enviroment->last_exit, 0);
    // If it's a background process, save to array but don't wait
    } else if((int)pid > 0) {
        // Save the processes PID
        enviroment->background_processes[enviroment->num_background_processes] = (int)pid;
    // If the child, exec child program
    } else {
        // First, process redirection
        bool redir_sucess = process_redirection(input);

        // If file redirection worked, continue
        if(redir_sucess == true) {
            // Exec child process
            int valid = execvp(input->args[0], input->args);

            // If exec returns a -1, there was an error
            if(valid == -1) {
                perror("execvp");
                enviroment->last_exit = 1;
            }
        }
        
    }

    return (int)pid;
}


bool process_redirection(struct command* input) {
    // If there is a value for input redirection, redirect it
    if(strlen(input->input_redirection) > 0) {
        int input_fd = open(input->input_redirection, O_RDONLY);

        // If file failed to open, print so and return false
        if (input_fd == -1) {
            printf("could not open %s for input", input->input_redirection);
            return false;
        }
        dup2(input_fd,0);
    }

    // Same for output redirection
    if(strlen(input->output_redirection) > 0) {
        int output_fd = open(input->output_redirection, O_WRONLY | O_CREAT);
        if (output_fd == -1) {
            printf("could not open %s for input", input->output_redirection);
            return false;
        }
        dup2(output_fd,1);
    }

    // If no errors, return true
    return true;
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
    printf("exit value %d", enviroment->last_exit);
    // Always fliush the output
    fflush(stdout);
}


void exit_shell(struct shell* enviroment) {
    printf("exiting...");
    fflush(stdout);
    exit(0);
}