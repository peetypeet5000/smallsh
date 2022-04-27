#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "input.h"
#include "main.h"

int exec_command(struct command*, struct shell*);
bool process_redirection(struct command*);
void check_background_processes(struct shell*);
bool exec_built_in(struct command*, struct shell*);
void change_directory(struct command*);
void status(struct shell*);
void exit_shell(struct shell*);
void set_child_signals();


#endif