#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "LineParser.h"
//#include "list.c"

int BUF_SIZE = 2048;

int cmp_arg0(cmdLine* cmd_line, char* str) {
    if(strcmp(cmd_line->arguments[0], str) == 0)
        return 1;
    return 0;
}

int execute(cmdLine *pCmdLine) {
    //char err_str[1024];
    int status, pid;

    if(!(pid = fork())) {
        if (pCmdLine->inputRedirect != NULL) {
            fclose(stdin);
            fopen(pCmdLine->inputRedirect, "r");
        }

        if (pCmdLine->outputRedirect != NULL) {
            fclose(stdout);
            fopen(pCmdLine->outputRedirect, "a");
        }
        execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        perror("");
        exit(1);
    }
    
    if(pCmdLine->blocking == 1)
        waitpid(pid, &status, 0);
}

int main(int argc, char **argv) {

    int exit_flag;
    cmdLine* curr_line;

    char ** history_list; 
    history_list = malloc(100*sizeof(char*));
    for(int i=0;i<100;i++){
        history_list[i]= malloc((2048)*sizeof(char));
    }
    int step=0;

    printf("%s\n", "myshell is running...");

    while(1) {
        char user_line[BUF_SIZE];
        char path[BUF_SIZE];

        // gets and prints working path
        getcwd(path, BUF_SIZE);
        printf("myshell~%s ", path);

        // gets a line from user
        fgets(user_line, BUF_SIZE, stdin);
        if(user_line[0] == '\n')        // if "enter" then go to begin of loop
            continue;

        for(int i=0;i<100;i++){
            if(user_line[i] == EOF) {
                history_list[step][i] = EOF;
                break;
            }
            history_list[step][i] = user_line[i];
        }
        
        curr_line = parseCmdLines(user_line);
       // printf("%s",history_list[step]);

        if(cmp_arg0(curr_line, "quit")) // if "quit" then go out of loop
            break;

        else if(cmp_arg0(curr_line, "cd")) {
                int status = chdir(curr_line->arguments[1]);
                if(status == -1)
                    perror("Failed to change directory: ");
        }
        else if(cmp_arg0(curr_line, "history")) {
            for(int i=0; i<=step; i++) {
                printf("%s\n", history_list[i]);
            }
            continue;
        }
        else {
            execute(curr_line);
        }
        
        freeCmdLines(curr_line);
        step++;

    }
    printf("%s\n", "exiting..");
    for(int i=0;i<=step;i++){
        free(history_list[step]);
    }

return 0;
}