#ifndef IDT_H
#define IDT_H

#define NULL_DESCRIPTOR 0X00
#define KERNEL_CODE_SEGMENT 0X08
#define KERNEL_DATA_SEGMENT 0X10

#define IDT_ATTRIBUTE_TYPE 0x8E

#include <stdint.h>

    struct idt_entry{
        uint16_t offset_low;
        uint16_t selector;
        uint8_t zero;
        uint8_t type_attr;
        uint16_t offset_high;
    }__attribute__((packed));

    struct idt_ptr{
        uint16_t limit;
        uint32_t base;
    }__attribute__((packed));

    void init_idt(void);
    void set_idt_gate(unsigned int vector, uint32_t handler);
    extern void load_idt(struct idt_ptr* idt_pointer);
    extern void keyboard_stub(void);
    extern void pit_stub(void);
    extern void enable_interrupts(void);
    extern void halt_cpu(void);

#endif
