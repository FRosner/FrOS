#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../drivers/display.h"

#include "util.h"

void start_kernel() {
    clear_screen();
    print_string("Installing interrupt service routines (ISRs).\n");
    isr_install();

    asm volatile("int $3");
}