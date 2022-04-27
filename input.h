#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "main.h"

struct command getCommand();
struct command tokenize_input();
void process_input(struct command*);

#endif