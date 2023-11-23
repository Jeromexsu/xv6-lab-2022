#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // printf("hello from user space!\n");
    if(argc < 2) {
        printf("usage: hello name");
    }
    hello(argv[1]);
    exit(0);
}