#ifndef KPRINT_H
#define KPRINT_H

#define VID_MEM 0xb8000
#define COLUMNS 80
#define ROWS 25

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5
#define VGA_LOW_INDEX_REG 0xF
#define VGA_HIGH_INDEX_REG 0xE

void update_vga_cursor();
void kputc(unsigned char c);
void clear_screen();
void scroll_up();

#endif
