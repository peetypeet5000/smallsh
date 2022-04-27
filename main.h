#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

struct shell {
    bool done;
    int last_exit;
    int num_background_processes;
    int background_processes[255];   // Supports up to 255 background processes
};

struct command {
    char* args[512];
    int arg_count;
    char input_redirection[255];
    char output_redirection[255];
    bool built_in;
    bool background;
    int pid;
};

// Global variable to disable background processes;
bool background_disabled;

#include "input.h"
#include "exec.h"

void init_signals();
void clean_memory(struct command);
void SIGINT_handler();

#endif