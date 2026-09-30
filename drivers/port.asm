[BITS 32]

section .text
    global inb
    global outb
    global io_wait

inb:
    push ebp
    mov ebp, esp
    mov edx, [ebp + 8]
    in al, dx
    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp
    mov edx, [ebp + 8]
    mov eax, [ebp + 12]
    out dx, al
    pop ebp
    ret

io_wait:
    mov al, 0
    out 0x80, al
    ret
