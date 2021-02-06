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

void kdump_trace(size_t max_frames)
{
    struct stackframe *s;
    asm volatile("movl %%ebp, %0" : "=r"(s));
    kprintf("Call Stack:\n");
    for (size_t i = 0; s && i < max_frames; ++i) {
        kprintf("\t0x%08x\n", s->eip);
        s = s->ebp;
    }
}

void kdump_regs(struct cpu_ctx ctx)
{
    kprintf("Call Trace:\n");
    kprintf("CS=%8x\tDS=%8x\tES=%8x\tFS=%8x\tGS=%8x\n", ctx.cs, ctx.ds, ctx.es, ctx.fs, ctx.gs);
    kprintf("EDI=%8x\tESI=%8x\tEBP=%8x\tESP=%8x\n", ctx.edi, ctx.esi, ctx.ebp, ctx.esp);
    kprintf("EAX=%8x\tEBX=%8x\tECX=%8x\tEDX=%8x\n", ctx.eax, ctx.ebx, ctx.ecx, ctx.edx);
    kprintf("EIP=%8x\tEFLAGS=%8x\tESP_ALIGN=%8x\n", ctx.eip, ctx.eflags, ctx.esp_align);
}

void debug_isr(struct cpu_ctx ctx)
{
    kprintf("DEBUG:\n");
    kdump_regs(ctx);
    kdump_trace(10);
}

void kmain(void)
{
    fb_init();

    kclear_screen();
    printf("Hello kernel!\nHello world!\n\n");

    init();

    asm volatile("int $0x1");

    return;
}
