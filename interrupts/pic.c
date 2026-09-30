#include "pic.h"
#include "../drivers/port.h"

void pic_remap(void){

    //icw1 -> initializing the PIC
    outb(PICM_COMMAND, PIC_INITIALIZE);
    io_wait();
    outb(PICS_COMMAND, PIC_INITIALIZE);
    io_wait();

    //icw2 -> telling the pic where the IRQs start at (Interrupt Request)
    outb(PICM_DATA, PICM_OFFSET);
    io_wait();
    outb(PICS_DATA, PICS_OFFSET);
    io_wait();

    //icw3 -> telling the PIC how master and slave PIC are connected
    outb(PICM_DATA, 0x04);
    io_wait();
    outb(PICS_DATA, 0x02);
    io_wait();

    //icw4 -> telling the PIC which mode is it that we want to use
    outb(PICM_DATA, PIC_MODE);
    io_wait();
    outb(PICS_DATA, PIC_MODE);
    io_wait();

    //mask all 8 IRQs in master except the keyboard IRQ (1) and mask all slave IRQs
    outb(PICM_DATA, 0xFC);
    io_wait();
    outb(PICS_DATA, 0xFF);
    io_wait();
}

void pic_send_eoi(unsigned char irq){
    if(irq >= 8){
        outb(PICS_COMMAND, PIC_EOI);
    }
    outb(PICM_COMMAND, PIC_EOI);
}
