global read_port
global write_port
global load_idt
global keyboard_handler
global write_gdt

extern keyboard_handler_main    ; io/keyboard.c

; arg0 = port
read_port:
    mov edx, [esp + 4]
    in al, dx
    ret

; arg0 = port
; arg1 = data
write_port:
    mov edx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

keyboard_handler:
    call keyboard_handler_main
    iretd

write_gdt:
    mov eax, [esp + 4]
    lgdt [eax]
    jmp 0x08:.reload_cs
.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
