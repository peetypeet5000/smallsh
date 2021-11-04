#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "input.h"
#include "exec.h"

struct shell {
    char cwd[2048];
    int last_exit;
    bool background_disabled;
    int num_background_processes;
    int background_processes[255];   // Supports up to 255 background processes
};

struct shell init();

#endif