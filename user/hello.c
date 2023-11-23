#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // printf("hello from user space!\n");
    hello();
    exit(0);
}