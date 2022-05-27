#include    <stdio.h>
#include    <string.h>
#include    <unistd.h>
#include    <sys/wait.h>
#include    <readline/readline.h>
#include    <readline/history.h>
#include    "define.h"
#include    "signal.h"

int takeInput(char* str)
{
    char* buf;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s]",cwd);
    buf = readline("$ ");
    if(buf == 0x0){
	printf("\n");
	eof_handler();
    }else{
    	if (strlen(buf) != 0) {
        	add_history(buf);
        	strcpy(str, buf);
        	return 0;
    	} else {
        	return 1;
    	}
    }
}
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork(); 
  
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("%s: command not found\n",parsed[0]);
        }
        exit(127);
    } else {
        // waiting for child to terminate
        wait(NULL); 
        return;
    }
}
void execArgsPiped(char** parsed, char** parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2]; 
    pid_t p1, p2;
  
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }
  
    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
  
        if (execvp(parsed[0], parsed) < 0) {
            printf("%s: command not found",parsed[0]);
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();
  
        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }
  
        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("%s: command not found",parsedpipe[0]);
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}
void openHelp()
{
    puts("\n A few integrated terminal commands"
         "\n Unix and Linux commands\n");
    return;
}
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 4, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
  
    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
  
    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }
  
    switch (switchOwnArg) {
    case 1:
        printf("\nexit\n");
        exit(0);
    case 2:
    //TODO FIX null
        if(parsed[1] == "~"  || parsed[1] == "" || parsed[1] == ){
            chdir(getenv("HOME"));
            return 1;
	    }else{
            int ch_stat = chdir(parsed[1]);
            if(ch_stat) {
                printf("%s: No such file or directory found\n",parsed[1]);
                return 1;
            }
            return 1;
	    }
    case 3:
        openHelp();
        return 1;
    case 4:
        printf("\nHello %s.\nMind that this is "
            "not a place to play around."
            "\nUse help to know more..\n",
            username);
        return 1;
    default:
        break;
    }
  
    return 0;
}
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }
  
    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}
void parseSpace(char* str, char** parsed)
{
    int i;
  
    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
  
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}
  
int processString(char* str, char** parsed, char** parsedpipe)
{
  
    char* strpiped[2];
    int piped = 0;
  
    piped = parsePipe(str, strpiped);
  
    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
  
    } else {
  
        parseSpace(str, parsed);
    }
  
    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}
  
int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
  
    while (1) {
        // Controller
        signal_controller(signal_handler);
	takeInput(inputString);
        // process
        execFlag = processString(inputString,parsedArgs, parsedArgsPiped);
        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);
        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    exit(0);
}
