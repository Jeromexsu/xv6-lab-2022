#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int readline(int fd,char **line) {
    char buf[512];
    char c;
    char *p = buf;
    while (read(fd,&c,sizeof c) != 0) {
        if(c == '\n') {
            *p = '\0';
            break;
        }
        *p++ = c;
    }
    *line = malloc(sizeof(char)*strlen(buf));
    strcpy(*line,buf);
    return p - buf; 
}

int split(char *string, char ***tokens) {
    char *slow = string, *fast = string;
    
    int N = 1;
    while(*fast != '\0') {
        if(*fast == ' ') N++;
        fast++;
    }

    fast = string;
    *tokens = malloc(sizeof(char*) * N);
    int i = 0;
    while(i < N) {
        while(*fast != '\0' && *fast != ' ') fast++;
        (*tokens)[i] = malloc(sizeof(char)*(fast-slow));
        *fast = '\0';
        strcpy((*tokens)[i],slow);
        i++;
        slow = ++fast;
    }
    return N;
}
int Fork() {
    int pid;
    pid = fork();
    if(pid < 0) {
        fprintf(2,"%d: fork error\n",getpid());
        exit(-1);
    }
    return pid;
}
int Exec(char *cmd, char *argv[]) {
    exec(cmd,argv);
    fprintf(2,"%d: exec error\n",getpid());
    exit(-1);
}

void Wait() {
    int status;
    int pid = wait(&status);
    if(status != 0) {
        fprintf(2,"%d: child[%d] exit unexceptionally with code %d\n",getpid(),pid,status);
        exit(-1);
    }
}

void forkexec(char *cmd, char *argv[]) {
    int pid = Fork();
    if(pid > 0) {
        Wait();
    } else {
        Exec(cmd,argv);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) exit(0);
    char *cmd = argv[1];
    
    char *totalArgv[MAXARG];

    // read in command line args
    int initArgc = argc-1;
    for(int i = 0; i < initArgc; i++) {
        totalArgv[i] = argv[i+1];
    }
    
    // read in args on left
    char *line;
    while(readline(0,&line) != 0) {
        char **tokens;
        int inArgc = split(line,&tokens);
        if(initArgc+inArgc > MAXARG) {
            fprintf(2,"too many args\n");
            exit(-1);
        }
        for(int i = 0; i < inArgc; i++) {
            totalArgv[i+initArgc] = tokens[i];
        }
        forkexec(cmd,totalArgv);
    }  
    exit(0);
}

