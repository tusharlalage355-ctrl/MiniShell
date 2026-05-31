#include "mini.h"

// List of built-in shell commands
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","jobs","bg","fg", NULL};

// Extract the command part (first word) from input string
char *get_command(char *input_string)
{
    int i=0;
    char *command=malloc(30);
    while(input_string[i]!=' ' && input_string[i] != '\0')
    {
      command[i]=input_string[i];
      i++;
    }
    command[i]='\0';
    return command;
}

// Determine if command is BUILTIN, EXTERNAL, or NO_COMMAND
int check_command_type(char *command)
{
    int i=0;
    while(builtins[i]!=NULL)
    {
        if(strcmp(command,builtins[i])==0)
        {
            return BUILTIN;
        }
        i++;
    }

    i=0;
    while(external_commands[i]!=NULL)
    {
        if(strcmp(command,external_commands[i])==0)
        {
            return EXTERNAL;
        }
        i++;
    }

    return NO_COMMAND;
}

// Load external commands from file into external_commands array
void extract_external_commands(char **external_commands)
{
    int fd=open("extr_cmt.txt",O_RDONLY);

    char buffer[30];
    char ch;
    int i=0,j=0;
    int flag=0;
    while(read(fd,&ch,1)>0)
    {
        if(ch!='\n')
        {
            buffer[i++]=ch;
        }
        else
        {
            flag=1;
            buffer[i]='\0';
        }

        if(flag)
        {
            int len=strlen(buffer)+1;
            external_commands[j]=malloc(len);
            strcpy(external_commands[j++],buffer);
            flag=0;
            i=0;
        }
    }
    external_commands[j]=malloc(5);

    external_commands[j]=NULL;
}

// Execute built-in commands or manage jobs
void execute_internal_commands(char *input_string)
{
    if(strcmp(input_string,"exit")==0)
    {
        exit(0); // exit shell
    }
    else if(strcmp(input_string,"pwd")==0)
    {
        char buffer[50];
        getcwd(buffer,50);
        printf("%s\n",buffer); // print current directory
    }
    else if(strncmp(input_string,"cd",2)==0)
    {
        chdir(input_string+3); // change directory
        char buffer[50];
        getcwd(buffer,50);
        printf("%s\n",buffer); // print new directory
    }
    else if(strncmp(input_string,"echo",4)==0)
    {
        char *buffer;
        buffer=input_string+5;
        if(strcmp(buffer,"$$")==0)
        {
            printf("%d\n",getpid()); // print shell PID
        }
        else if(strcmp(buffer,"$SHELL")==0)
        {
            printf("%s\n",getenv("SHELL")); // print shell path
        }
        else if(strcmp(buffer,"$?")==0)
        {
            printf("%d\n",last_exit_status); // print last command status
        }
        else
        {
            printf("%s\n",buffer); // print text
        }

    }
    else if(strcmp(input_string,"jobs")==0)
    {
        print_list(); // display background jobs
    }
    else if(strcmp(input_string,"fg")==0)
    {
        if(head == NULL)
        {
            printf("bash: fg: no current job\n");
            return;
        }
        printf("%s\n",tail->cmd);
        kill((tail)->job_pid,SIGCONT); // resume last job
        waitpid((tail)->job_pid,&status,WUNTRACED);
        delete_last_node(); // remove from job list
        pid=0;
    }
    else if(strcmp(input_string,"bg")==0)
    {
        if(head == NULL)
        {
            printf("bash: bg: no current job\n");
            return;
        }
        kill(tail->job_pid,SIGCONT); // resume job in background
        delete_last_node(); // remove from job list
    }

}

// Execute external commands including handling pipes
void execute_external_commands(char *input_string)
{
    int row = 1, i = 0;

    while(input_string[i] != '\0')
    {
        if(isspace(input_string[i]))
        {
            row++; // count arguments
        }
        i++;
    }

    char *argv[row + 1];

    char *token;
    i = 0;
    token = strtok(input_string, " ");

    while(token != NULL)
    {
        argv[i] = token; // split arguments
        i++;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;

    // detect pipes and prepare for execution
    i = 0;
    int index = 0;
    int pipe_count = 0;
    int cmd[10];
    cmd[index++] =0;
    while(argv[i] != NULL)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            cmd[index++] =i+1;
            pipe_count++;
            argv[i] = NULL;
        }
        i++;
    }

    if(pipe_count == 0)
    {
        execvp(argv[0], argv); // execute single command
        perror("execvp");
        exit(1);
    }
    else
    {
        int fd[2];

        for(i = 0; i < index; i++)
        {
            if(i != index-1)
            {
                pipe(fd); // create pipe for communication
            }

            int ret = fork();

            if(ret == 0)
            {
                if(i !=index-1)
                {
                    close(fd[0]);
                    dup2(fd[1], 1); // redirect output to pipe
                    close(fd[1]);
                }

                execvp(argv[cmd[i]], argv + cmd[i]); // execute piped command
                perror("execvp");
                exit(1);
            }
            else if(ret>0)
            {
                wait(NULL); // wait for child

                if(i !=index-1)
                {
                    close(fd[1]);
                    dup2(fd[0], 0); // redirect input from pipe
                    close(fd[0]);
                }
            }
        }
        exit(0);
    }
}

// Insert job at end of job list
int insert_last(Dlist **head,Dlist **tail,int id,int jpid,char *cmd_name)
{
    Dlist *new=malloc(sizeof(Dlist));

    new->job_id=id;
    new->job_pid=jpid;
    strcpy(new->cmd,cmd_name);
    new->prev=NULL;
    new->next=NULL;

    if(*head==NULL)
    {
        *head=new;
        *tail=new;
    }
    else
    {
        (*tail)->next=new;
        new->prev=(*tail);
        (*tail)=new;
    }

    printf("%d\t%s\t\t%s\t%d\n",job_id,"Stopped",input_string,jpid); // print job info
    return 0;
}

// Print all jobs in job list
void print_list()
{
    Dlist *temp=(head);

    while(temp)
    {
        printf("%d\t%s\t\t%s\n",temp->job_id,"Stopped",temp->cmd);
        temp=temp->next;
    }
}

// Delete last job from job list
int delete_last_node()
{
    Dlist *temp=(tail);

    if (tail->prev == NULL) // only one node
    {
        head = NULL;
        tail = NULL;
    }
    else
    {
        (tail)=(tail)->prev;
        (tail)->next=NULL;
    }
    free(temp);

    return 1;
}