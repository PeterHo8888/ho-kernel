bits 32
section .multiboot
    dd 0x1BADB002
    dd 0x0
    dd - (0x1BADB002 + 0x0)

global start
extern kmain

section .text

start:
    cli
    mov esp, stack_space
    call kmain
forever:
    hlt
    jmp forever

section .bss
resb 16384
stack_space:
