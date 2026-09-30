#include "gdt.h"

#define GDT_ENTRIES 3

static struct gdt_entry gdt[GDT_ENTRIES];
static struct gdt_ptr gdt_pointer;

extern void gdt_flush(uint32_t);

void set_gdt_entry(
    int index,
    uint32_t offset,
    uint32_t limit,
    uint8_t access,
    uint8_t granularity
){
    gdt[index].limit = limit & 0xFFFF;

    gdt[index].offset_low = offset & 0xFFFF;
    gdt[index].offset_middle = (offset >> 16) & 0xFF;
    gdt[index].offset_high = (offset >> 24) & 0xFF;

    gdt[index].access = access;

    gdt[index].granularity =
        ((limit >> 16) & 0x0F) |
        (granularity & 0xF0);
}

void init_gdt(void)
{
    gdt_pointer.limit = sizeof(gdt) - 1;
    gdt_pointer.base = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);

    set_gdt_entry(
        1,
        0,
        0xFFFFFFFF,
        0x9A,
        0xCF
    );

    set_gdt_entry(
        2,
        0,
        0xFFFFFFFF,
        0x92,
        0xCF
    );

    gdt_flush((uint32_t)&gdt_pointer);
}
