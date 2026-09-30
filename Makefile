CROSS = /home/dan/opt/cross/bin/i686-elf
AS = $(CROSS)-as
CC = $(CROSS)-gcc
LD = $(CROSS)-ld
NASM = nasm

CFLAGS = -ffreestanding -fno-stack-protector

BUILD = build

KERNEL = $(BUILD)/kernel.elf
ISO = $(BUILD)/mutiso-os.iso

all: $(ISO)

$(BUILD)/boot.o: boot/boot.asm
	mkdir -p $(BUILD)
	$(NASM) -f elf32 boot/boot.asm -o build/boot.o

$(BUILD)/kernel.o: kernel/kernel.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/vga.o: drivers/vga.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/port.o: drivers/port.asm
	mkdir -p $(BUILD)
	$(NASM) -f elf32 drivers/port.asm -o build/port.o

$(BUILD)/keyboard.o: drivers/keyboard.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: interrupts/idt.asm
	mkdir -p $(BUILD)
	$(NASM) -f elf32 interrupts/idt.asm -o build/idt.o

$(BUILD)/isr.o: interrupts/isr.asm
	mkdir -p $(BUILD)
	$(NASM) -f elf32 interrupts/isr.asm -o build/isr.o

$(BUILD)/idt_c.o: interrupts/idt_c.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pic.o: interrupts/pic.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/gdt.o: interrupts/gdt.asm
	mkdir -p $(BUILD)
	$(NASM) -f elf32 interrupts/gdt.asm -o build/gdt.o

$(BUILD)/gdt_c.o: interrupts/gdt.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pit.o: interrupts/pit.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/vga.o $(BUILD)/port.o $(BUILD)/keyboard.o $(BUILD)/idt.o $(BUILD)/idt_c.o $(BUILD)/isr.o $(BUILD)/pic.o $(BUILD)/gdt.o $(BUILD)/gdt_c.o $(BUILD)/pit.o linker.ld
	$(LD) -T linker.ld -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/vga.o $(BUILD)/port.o $(BUILD)/keyboard.o $(BUILD)/isr.o $(BUILD)/idt.o $(BUILD)/idt_c.o $(BUILD)/pic.o $(BUILD)/gdt.o $(BUILD)/gdt_c.o $(BUILD)/pit.o

$(ISO): $(KERNEL) grub/grub.cfg
	mkdir -p $(BUILD)/isodir/boot/grub
	cp $(KERNEL) $(BUILD)/isodir/boot/kernel.elf
	cp grub/grub.cfg $(BUILD)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(BUILD)/isodir

clean:
	rm -rf $(BUILD)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

.PHONY: all clean run
