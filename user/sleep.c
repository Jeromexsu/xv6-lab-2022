#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    if(argc == 1) {
        char * errorMsg = "sleep for n clock ticks\nusage: sleep n\n";
        write(1,errorMsg,strlen(errorMsg));
    }
    int duration = atoi(argv[1]);
    sleep(duration);
    exit(0);
}