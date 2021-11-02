#include "input.h"

struct command getCommand() {
    // Print prompt and flush
    printf("\n: ");
    fflush(stdout);

    // Get user input and tokenize it
    struct command tokenized_input = tokenize_input();
    
    // Process user input into useful stuct
    process_input(&tokenized_input);

}


struct command tokenize_input() {
    struct command tokenized_input = {{0}};
    char* input;
    size_t size;
    int args_read = 0;
    char *saveptr;

    // Get a line of user input
     getline(&input, &size, stdin);

    // Clean the newline character from the input
    char* clean_input = calloc(strlen(input) - 1, sizeof(char));
    strncpy(clean_input, input, strlen(input) - 1);
    

    // Get first section
    char *token = strtok_r(clean_input, " ", &saveptr);

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

    // Serach for input redirection
    for(int i = 0; i < input->arg_count; i++) {
        if(input->args[i][0] == '<'){
            // Make sure there is a value after the redirection
            if(i + 1 < input->arg_count) {
                strncpy(input->input_redirection, input->args[i+1], strlen(input->args[i+1]));
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
                strncpy(input->output_redirection, input->args[i+1], strlen(input->args[i+1]));
                new_arg_count = new_arg_count - 2;
            }
        }
    }

    // Search for trailing & for background
    if(input->args[input->arg_count - 1][0] == '&') {
        input->background = true;
    }


    // Save the relevant number of arguments (for exec) in the struct
    input->arg_count = new_arg_count;

}