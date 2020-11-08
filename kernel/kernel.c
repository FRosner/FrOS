#include "../drivers/display.h"
#include "util.h"

void main() {
    clear_screen();
    char* line;
    for (int i = 0; i < 100; ++i) {
        int_to_string(i, line, 10);
        print_string(line);
        print_nl();
    }
}