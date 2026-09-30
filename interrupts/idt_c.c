#include "idt.h"

extern void pit_stub(void);
#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_pointer;

void set_idt_gate(unsigned int vector, uint32_t handler){
    idt[vector].offset_low = handler & 0xFFFF;
    idt[vector].selector = KERNEL_CODE_SEGMENT;
    idt[vector].zero = 0;
    idt[vector].type_attr = IDT_ATTRIBUTE_TYPE;
    idt[vector].offset_high = (handler >> 16) & 0XFFFF;
}

void init_idt(void){

    idt_pointer.limit = sizeof(idt) - 1;
    idt_pointer.base = (uint32_t)&idt;

    for(unsigned int i = 0; i < IDT_ENTRIES; i++){
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attr = 0;
        idt[i].offset_high = 0;
    }

    set_idt_gate(32, (uint32_t)pit_stub);
    set_idt_gate(33, (uint32_t)keyboard_stub);
    load_idt(&idt_pointer);
}
