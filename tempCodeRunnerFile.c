#include <stdio.h>
void test() {
    printf("what\n");
}
void call(void (*f)()) {
    f();
    printf("%p\n",f);
}

int main() {
    printf("%p\n",test);
    call(test);
    return 0;
}