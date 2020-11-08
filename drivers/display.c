#include "display.h"
#include "ports.h"

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

/*
 * TODO:
 * - handle illegal offset (print error message somewhere)
 * - handle newline characters (move cursor to beginning of next line)
 */
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

/*
 * Print character at cursor.
 */
void print_char(char character) {
    print_char_at_offset(character, get_cursor());
}

void print_string(char* string) {
    int i = 0;
    while (string[i] != 0) {
        print_char(string[i++]);
    }
}
