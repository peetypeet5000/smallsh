#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct command {
    char* args[20];
    int arg_count;
    char input_redirection[255];
    char output_redirection[255];
    bool built_in;
    bool background;
};

struct command getCommand();
struct command tokenize_input();
void process_input(struct command*);

#endif