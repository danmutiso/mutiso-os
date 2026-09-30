#include "../drivers/vga.h"
#include "../interrupts/pic.h"
#include "../interrupts/idt.h"
#include "../interrupts/gdt.h"
#include "../interrupts/pit.h"

void kernel_main(void){

    vga_clear();
    init_gdt();
    vga_puts("(kernel(gdt)) global descriptor table inititiated", WHITE);
    newline();
    pic_remap();
    vga_puts("(kernel(pic)) programmable interrupt controller inititiated", WHITE);
    newline();
    init_idt();
    vga_puts("(kernel(idt)) interrupt descriptor table inititiated", WHITE);
    newline();
    init_pit();
    vga_puts("(kernel(pit)) programmable interval timer inititated", WHITE);
    newline();
    enable_interrupts();
    vga_puts("(kernel(interrupts)) interrupts have been enabled", WHITE);
    newline();

    pit_sleep_ms(500);

    while(1){

        //vga_set_cursor(2,7);
        //vga_put_uint(pit_get_hours());
        //vga_puts(" hrs ", LIGHT_GREEN);
        //vga_put_uint(pit_get_minutes());
        //vga_puts(" min ", LIGHT_GREEN);
        //vga_put_uint(pit_get_seconds());
        //vga_puts(" seconds", LIGHT_GREEN);
        halt_cpu();
    }
}

