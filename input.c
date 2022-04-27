#include "input.h"

struct command getCommand() {

    // Get user input and tokenize it
    struct command tokenized_input = tokenize_input();
    
    // Process user input into useful stuct
    process_input(&tokenized_input);

    return tokenized_input;

}


struct command tokenize_input() {
    struct command tokenized_input = {{0}};
    char* input = {0};
    size_t size = 0;
    int args_read = 0;
    char *saveptr;

    // Get a line of user input
    do {
        printf(": ");
        fflush(stdout);
        getline(&input, &size, stdin);
    } while(strlen(input) < 2);

    // Clean the newline character from the input
    input[strlen(input) - 1] = 0;

    // Expand $$
    for(int i = 0; i < strlen(input); i++) {
        if(input[i] == '$' && input[i+1] == '$') {
            // split the string at this point
            char* first_half = calloc(strlen(input) + 16, sizeof(char));
            strncpy(first_half, input, i);

            // get second half of the string
            char* second_half = calloc(strlen(input), sizeof(char));
            for(int j = 0; j < strlen(input) - i - 1; j++) {
                second_half[j] = input[i + j + 2];
            }

            // get the pid
            char pid[16];
            sprintf(pid, "%d", getpid());

            strcat(first_half, pid);
            strcat(first_half, second_half);

            free(input);
            input = calloc(strlen(first_half), sizeof(char));
            strncpy(input, first_half, strlen(first_half));
            break; //only need one deferernace
        }
    }
    

    // Get first section
    char *token = strtok_r(input, " ", &saveptr);

    // Keep tokenizing until there are no more arguments
    while(token != NULL) {
        // Allocated space, copy in, get next token
        tokenized_input.args[args_read] = calloc(strlen(token) + 1, sizeof(char));
        // Post increments args_read
        strcpy(tokenized_input.args[args_read++], token);
        token = strtok_r(NULL, " ", &saveptr);
    }
    // Save the number of arguments in the struct
    tokenized_input.arg_count = args_read;

    free(input);

    return tokenized_input;

}

void process_input(struct command* input) {
    // Keep track of how many arguemnts are not from the command itself
    int new_arg_count = input->arg_count;

    // Search for trailing & for background
    if(strcmp(input->args[input->arg_count - 1], "&") == 0) {
        input->background = true;
        // Set redirection to /dev/null by default
        strncpy(input->input_redirection, "/dev/null", strlen("/dev/null"));
        strncpy(input->output_redirection, "/dev/null", strlen("/dev/null"));
        new_arg_count--;
    }

    // Serach for input redirection
    for(int i = 0; i < input->arg_count; i++) {
        if(input->args[i][0] == '<'){
            // Make sure there is a value after the redirection
            if(i + 1 < input->arg_count) {
                strncpy(input->input_redirection, input->args[i+1], 255);
                // Subtract off the two arguments that are related to redirection
                new_arg_count = new_arg_count - 2;
            }
        }
    }

    // Search for output redirection
    for(int i = 0; i < input->arg_count; i++) {
        if(input->args[i][0] == '>'){
            // Make sure there is a value after the redirection
            if(i + 1 < input->arg_count) {
                strncpy(input->output_redirection, input->args[i+1], 255);
                new_arg_count = new_arg_count - 2;
            }
        }
    }

    // Remove the arguments that were for redirection or bg
    for(int i = new_arg_count; i < input->arg_count; i++) {
        free(input->args[i]);
        input->args[i] = NULL;
    }
    input->arg_count = new_arg_count;

}
