void test() {
    printf("what");
}
void call(void (*function)()) {
    function();
}

int main() {
    call(test);
    return 0;
}