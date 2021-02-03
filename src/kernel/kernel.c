#include <kernel/kernel.h>
#include <kernel/fb.h>
#include <kernel/io.h>
#include <kernel/interrupts.h>
#include <stdio.h>

void init()
{
    idt_init();
    kb_init();
    fb_init();
}

void kmain(void)
{
    init();

    kclear_screen();
    printf("Hello kernel!\nHello world!\n\n");

    return;
}
