#ifndef MINI_H
#define MINI_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include<ctype.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

extern char *external_commands[154];
extern char prompt[25];
extern char input_string[25];
extern int pid;
extern int status;
extern int last_exit_status;
extern int job_id;


typedef struct node
{
    int job_id;
    char cmd[20];
    int job_pid;
    struct node *next;
    struct node *prev;
}Dlist;

extern Dlist *head;
extern Dlist *tail;

void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);

void execute_external_commands(char *input_string);

void my_handler(int signum);

int insert_last(Dlist **head,Dlist **tail,int id,int jpid,char *cmd_name);

void print_list();

int delete_last_node();



#endif
