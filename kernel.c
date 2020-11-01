void caller() {
    int my_var = 0xbaba;
    my_func(my_var);
}

int my_func(int arg) {
    return arg;
}

void func() {
    char* string = "Hello";
}
