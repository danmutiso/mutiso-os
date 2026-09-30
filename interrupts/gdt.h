#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_entry{
    uint16_t limit;
    uint16_t offset_low;
    uint8_t offset_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t offset_high;
}__attribute__((packed));

struct gdt_ptr{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

void init_gdt(void);

#endif
