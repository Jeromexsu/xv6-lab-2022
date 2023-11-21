#include "kernel/types.h"
#include "user/user.h"
int
main(int argc, char *argv[]) {
    char buf[512];

    char *pong = "pong";
    char *ping = "ping";

    int p[2];
    pipe(p);

    //fork
    int pid = fork();
    if(pid < 0) {
        printf("fork error\n");
        exit(-1);
    }

    if(pid == 0) {
        read(p[0],buf,sizeof buf);
        printf("%d: received %s\n",getpid(),buf);
        write(p[1],pong,strlen(pong));
    } else {
        write(p[1],ping,strlen(ping));
        read(p[0],buf,sizeof buf);
        printf("%d: received %s\n",getpid(),buf);
    }

    close(p[0]);
    close(p[1]);

    exit(0);
}