#include <kernel/kernel.h>
#include <kernel/fb.h>
#include <kernel/interrupts.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <stdio.h>

#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

#if !defined(__i386__)
#error "Only ix86 is supported, please compile with an ix86-elf compiler"
#endif

void __cxa_pure_virtual()
{
    // Needed for pure virtual functions
}

void init()
{
    gdt_init();
    idt_init();
    kb_init();
}

void kmain(void)
{
    fb_init();

    kclear_screen();
    printf("Hello kernel!\nHello world!\n\n");

    init();

    asm volatile("int $0x3");

    return;
}
