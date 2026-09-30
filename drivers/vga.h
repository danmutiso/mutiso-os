#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define BLACK           0x00
#define BLUE            0x01
#define GREEN           0x02
#define CYAN            0x03
#define RED             0x04
#define MAGENTA         0x05
#define BROWN           0x06
#define LIGHT_GRAY      0x07
#define DARK_GRAY       0x08
#define LIGHT_BLUE      0x09
#define LIGHT_GREEN     0x0A
#define LIGHT_CYAN      0x0B
#define LIGHT_RED       0x0C
#define LIGHT_MAGENTA   0x0D
#define YELLOW          0x0E
#define WHITE           0x0F

void vga_clear(void);
void newline(void);
void backspace(void);
void vga_putc(const char c, const unsigned char atrr);
void vga_puts(const char *string, unsigned char atrr);
void vga_put_uint(uint32_t number);
void vga_set_cursor(int new_row, int new_column);

#endif
