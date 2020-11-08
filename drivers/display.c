#include "display.h"
#include "ports.h"
#include "../kernel/util.h"

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
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

void set_char_at_video_memory(char character, int offset) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

void scroll_ln() {
    for (int row = 1; row < MAX_ROWS; row++) {
        memory_copy((char*) (get_offset(0, row) + VIDEO_ADDRESS),
                    (char*) (get_offset(0, row - 1) + VIDEO_ADDRESS),
                    MAX_COLS * 2);
    }

    for (int col = 0; col < MAX_COLS * 2; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }
    
    set_cursor(get_cursor() - 2 * MAX_COLS);
}

/*
 * TODO:
 * - handle illegal offset (print error message somewhere)
 * - handle newline characters (move cursor to beginning of next line)
 */
void print_char_at_offset(char character, int offset) {
    set_char_at_video_memory(character, offset);

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
    if (get_cursor() >= MAX_ROWS * MAX_COLS * 2) {
        scroll_ln();
    }
    print_char_at_offset(character, get_cursor());
}

void print_string(char *string) {
    int i = 0;
    while (string[i] != 0) {
        print_char(string[i++]);
    }
}

int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

void print_nl() {
    int offset = get_cursor();
    int row = get_row_from_offset(offset);
    set_cursor(get_offset(0, row + 1));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}
