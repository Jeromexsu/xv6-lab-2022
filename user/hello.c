#include "kernel/types.h"
#include "user/user.h"
int
main(int argc, char *argv[]) {
    char *welcomeMessage = "hello\n";
    write(1,welcomeMessage,strlen(welcomeMessage));
    exit(0);
}