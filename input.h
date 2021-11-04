#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

struct command {
    char* args[512];
    int arg_count;
    char input_redirection[255];
    char output_redirection[255];
    bool built_in;
    bool background;
    int pid;
};

struct command getCommand();
struct command tokenize_input();
void process_input(struct command*);

#endif