#include "exec.h"

bool exec_built_in(struct command input) {
    // Check the first argument for one of the built-in commands
    if(strncmp(input.args[0], "exit", 4) == 0) {
        //exit
    } else if (strncmp(input.args[0], "cd", 2) == 0) {
        //cd
    } else if (strncmp(input.args[0], "status", 6) == 0) {
        //status
    } 
    // Also check for line comments to ignore
    else if (strncmp(input.args[0], "#", 1)) {
        // ignore
    }
    else {
        return false;
    }

    // If one of the built-ins was executed, return true
    return true;
}