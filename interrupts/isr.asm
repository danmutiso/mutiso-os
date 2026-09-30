[BITS 32]

section .text

    extern keyboard_handler
    extern pit_handler

    global keyboard_stub
    global enable_interrupts
    global halt_cpu
    global pit_stub

keyboard_stub:
    pusha
    call keyboard_handler
    popa
    iret

pit_stub:
    pusha
    call pit_handler
    popa
    iret

enable_interrupts:
    sti
    ret

halt_cpu:
    hlt
    ret
