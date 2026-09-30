#include "vga.h"

#define VGA_MEMORY ((volatile unsigned char*)0XB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static int row = 0;
static int column = 0;

void vga_clear(void){
    for(unsigned int x = 0; x < VGA_WIDTH * VGA_HEIGHT; x++){
        VGA_MEMORY[x * 2] = ' ';
        VGA_MEMORY[x * 2 + 1] = 0x07;
    }
    row = 0;
    column = 0;
}

void newline(void){
    row++;
    column = 0;
}

void backspace(void){
    if (column > 0) {
        column--;

        vga_putc(' ', WHITE);

        column--;
    }
}

void vga_putc(const char c, const unsigned char attr){

    if(column >= VGA_WIDTH){
        column = 0;
        row++;
    }

    if(row >= VGA_HEIGHT){
        row = 0;
        column = 0;
    }

    unsigned int position = (row * VGA_WIDTH + column) * 2;

    VGA_MEMORY[position] = c;
    VGA_MEMORY[position + 1] = attr;

    column++;
}

void vga_puts(const char *string, const unsigned char attr){
    while(*string){
        vga_putc(*string, attr);
        string++;
    }
}

void vga_put_uint(uint32_t number){

    char digits[10];
    unsigned int i = 0;

    if(number == 0){
        vga_putc('0', WHITE);
        return;
    }

    while(number > 0){
        digits[i] = number % 10;
        number = number / 10;
        i++;
    }

    while(i > 0){
        i--;
        vga_putc('0' + digits[i], WHITE);
    }
}

void vga_set_cursor(int new_row, int new_column){
    row = new_row;
    column = new_column;
}

