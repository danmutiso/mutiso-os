# Mutiso OS

A personal x86 operating-system development project built from scratch as a hands-on study of low-level systems programming, computer architecture, interrupts, hardware interaction, and kernel development.

## Current Focus

The project currently explores:

- x86 protected mode
- Multiboot booting
- GDT (Global Descriptor Table)
- IDT (Interrupt Descriptor Table)
- PIC interrupt remapping
- Hardware interrupt handling
- Keyboard IRQ handling
- VGA text-mode output
- PIT (Programmable Interval Timer)
- Timer ticks and timekeeping
- `hlt`-based CPU idle waiting
- C and x86 assembly interoperability

## Project Structure

```text
mutiso-os/
├── boot/              # Bootloader / Multiboot assembly
├── drivers/           # Hardware and device drivers
├── interrupts/        # GDT, IDT, PIC, PIT and ISR code
├── kernel/            # Kernel entry and core logic
├── grub/              # GRUB configuration
├── build.py           # Build automation
├── linker.ld          # Kernel linker script
├── Makefile           # Build commands
└── .gitignore

```

## Building

The project uses a cross-compiled `i686-elf` toolchain and produces a bootable ISO.

Build commands and toolchain requirements are defined by the project's `Makefile` and build scripts.

Generated files are placed under `build/` and are intentionally excluded from version control.

## Architecture

The project combines:

- **C** for kernel and driver logic
- **x86 assembly** for low-level CPU operations, boot code and interrupt entry points
- **GRUB/Multiboot** for initial kernel loading
- **QEMU** for running and testing the operating system

## Learning Journey

This repository is primarily a learning project. The goal is to understand what happens beneath high-level operating systems by progressively implementing kernel subsystems rather than relying on an existing operating-system framework.

The current development path is moving from hardware interrupts and timer programming toward scheduling and context switching.

## Status

Active development.

This is not intended to be a production operating system. It is a continuously evolving systems-programming and operating-system-development project.

## License

No license specified yet.
