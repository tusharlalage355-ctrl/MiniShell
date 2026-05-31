#include "mini.h"

int pid;
int status;
int last_exit_status;
int job_id;
Dlist *head=NULL,*tail=NULL;

// Signal handler for SIGINT and SIGTSTP
void my_handler(int signum)
{
    last_exit_status=128+signum; // update last exit status based on signal

    if(signum==SIGINT && pid==0)
    {
        printf(GREEN "\n%s"RESET,prompt); // print prompt on Ctrl+C
        fflush(stdout);
    }
    if(signum==SIGTSTP && pid==0)
    {
        printf(GREEN"\n%s"RESET,prompt); // print prompt on Ctrl+Z
        fflush(stdout);
    }
    else if(signum==SIGTSTP && pid>0)
    {
        insert_last(&head,&tail,job_id,pid,input_string); // save stopped job
        job_id++;
    }
}

// Main loop to read user input and execute commands
void scan_input(char *prompt,char *input_string)
{
    signal(SIGINT,my_handler);  // attach handler for Ctrl+C
    signal(SIGTSTP,my_handler); // attach handler for Ctrl+Z

    extract_external_commands(external_commands); // load external commands

    while(1)
    {
        printf(GREEN"%s"RESET,prompt); // display prompt
        scanf("%[^\n]",input_string);  // read full line
        getchar();                     // consume newline

        if(strncmp(input_string,"PS1=",4)==0)  // validate for PS1 command
        {
            int i=4,len;
            len=strlen(input_string);

            while(i>0 && input_string[len-1]==' ')
            {
                input_string[i-1]='\0'; // remove trailing spaces
                len--;
            }

            while(input_string[i]!='\0')
            {
                if(input_string[i]==' ')  // check for spaces in new prompt
                {
                    printf("PS1: command not found\n");
                    break;
                }
                i++;
            }

            if(input_string[i]=='\0')
            {
                strcpy(prompt,input_string+4); // set new prompt
            }
        }
        else
        {
            char *command=get_command(input_string); // extract command

            if(check_command_type(command)==BUILTIN)
            {
                execute_internal_commands(input_string); // execute built-in
                last_exit_status=0;
            }
            else if(check_command_type(command)==EXTERNAL)
            {
                pid=fork(); // fork for external command
                if(pid==0)
                {
                    signal(SIGINT,SIG_DFL);  // reset signals in child
                    signal(SIGTSTP,SIG_DFL);
                    execute_external_commands(input_string); // execute external
                }
                else if(pid>0)
                {
                    waitpid(pid,&status,WUNTRACED); // wait for child
                    if (WIFEXITED(status))
                        last_exit_status = WEXITSTATUS(status); // store exit code
                    else if (WIFSIGNALED(status))
                        last_exit_status = 128 + WTERMSIG(status); // store signal code
                    pid=0;
                }
            }
            else
            {
                printf("%s command not found\n",command); // unknown command
                last_exit_status=127;
            }
        }
    }
}