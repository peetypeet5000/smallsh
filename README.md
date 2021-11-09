# smallsh
smallsh is a simple unix shell created as part of CS340 - Operating System. It uses POSIX system calls to create a shell that features a subset of standard bash functionality.

## Usage
smallsh is designed for any POSIX compatible OS. To compile the program with gcc, use `make`. Once compiled, the shell can be started with `./smallsh`.

## Featues
- Executing any program via full file path or PATH search
- Create background processes by appending an `&` to the end of the command
- Input/Output redirection with the `>` and `<` operators
- Built-in `status`, `cd`, and `exit` commands
- Expansion of `$$` into program PID
- Disabling background processes with a SIGSTP signal (Cntl-Z), SIGINT signal handling
