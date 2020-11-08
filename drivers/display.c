#include "display.h"
#include "ports.h"

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

void print_char_at_offset(char character, int offset) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;

    offset += 2;
    set_cursor(offset);
}

/*
 * Print character on screen at specified position.
 */
void print_char_at(char character, int col, int row) {
    print_char_at_offset(character, get_offset(col, row));
}