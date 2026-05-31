/* Name-Tushar Lalage

Description-

The MiniShell project is a custom command-line shell 
implemented in C that allows users to execute both built-in 
commands(like cd, pwd, echo, jobs, fg, bg) and external programs. 
It supports foreground and background job control, including stopping processes with Ctrl+Z, 
resuming them in the foreground or background, and maintaining a job list using a linked list. 
The shell handles signals (SIGINT, SIGTSTP, SIGCHLD) to manage process interruptions and terminations gracefully. 
It also supports piped commands, tracks the last command’s exit status, and provides a colorized,
customizable prompt for enhanced usability, making it a functional mini version of a Linux shell.

*/

#include "mini.h"
char *external_commands[154];
char prompt[25];
char input_string[25];
int main()
{
    // Clear the terminal screen at the start of the shell
    system("clear");

    // Initialize the shell prompt
    strcpy(prompt,"minishell:~");

    // Start the main input loop to read and execute commands
    scan_input(prompt,input_string);
}