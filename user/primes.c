#include "kernel/types.h"
#include "user/user.h"
#define N 35

void screen(int left[], int end);

int main(int agrc, char *argv[]) {
    int p[2];
    pipe(p);

    int end[2]; 
    pipe(end);
    
    int pid = fork();
    if(pid < 0) {
        printf("fork error\n");
        exit(-1);
    }

    if(pid > 0) {
        // feed numbers
        close(p[0]);    
        for(int i = 2; i < N; i++) {
            write(p[1],&i,sizeof i);
        }
        close(p[1]);

        // wait for termination
        close(end[1]);
        read(end[0],(void *) 0,1);
        close(end[0]);
    } else {
        close(end[0]);
        screen(p,end[1]);
    }

    exit(0);
}

void screen(int left[], int end) {
    close(left[1]);
    int prime;
    int readCount = read(left[0],&prime,sizeof prime);
    if(readCount == 0) {
        close(left[0]);
        close(end);
        return;
    }
    if(readCount < 0) {
        printf("read error\n");
        return;
    }
    printf("prime %d [pid:%d]\n",prime,getpid());

    int right[2];
    pipe(right);

    int pid = fork();
    if(pid < 0) {
        printf("fork error\n");
        exit(-1);
    }
    if(pid > 0) {
        close(end);
        close(right[0]);
        int in;
        while(read(left[0],&in,sizeof in) != 0) {
            if(in % prime != 0) {
                write(right[1],&in,sizeof in);
            }
        }
        close(left[0]);
        close(right[1]);
    } else {
        close(left[0]);
        screen(right,end);
    }
}