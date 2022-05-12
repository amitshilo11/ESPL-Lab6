#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "LineParser.h"

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
        curr_line = parseCmdLines(user_line);

        if(cmp_arg0(curr_line, "quit")) // if "quit" then go out of loop
            break;

        execute(curr_line);
        
        freeCmdLines(curr_line);
    }
    printf("%s\n", "exiting..");

return 0;
}