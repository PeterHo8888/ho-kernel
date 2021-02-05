#include <kernel/kernel.h>
#include <kernel/keyboard.h>
#include <kernel/types.h>
#include <kernel/interrupts.h>
#include <kernel/fb.h>
#include <stdio.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
void keyboard_isr()
{
    uint8_t status;
    uint8_t keycode;

    status = read_port(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);

        // Change this to fill a buffer
        uint8_t ascii = keycode_to_ascii(keycode);
        if (ascii)
            putchar(ascii);
    }

    /* write EOI */
    write_port(0x20, 0x20);
}

void kb_init()
{
    write_port(0x21, 0xfd);
}

static const char *cpu_reserved_interrupt_str[0x13] = {
    "DIV_BY_0",
    "DEBUG",
    "NON_MASKABLE_INT",
    "BREAKPOINT",
    "INTO_DETECT_OVERFLOW",
    "OUT_OF_BOUNDS",
    "INVALID_OPCODE",
    "NO_COPROCESSOR",
    "DOUBLE_FAULT",
    "COPROCESSOR_SEGMENT_OVERRUN",
    "BAD_TSS",
    "SEGMENT_NOT_PRESENT",
    "STACK_FAULT",
    "GENERAL_PROTECTION_FAULT",
    "PAGE_FAULT",
    "UNKNOWN_INTERRUPT",
    "COPROCESSOR_FAULT",
    "ALIGNMENT_CHECK",
    "MACHINE_CHECK"
};

void isr_handler(struct cpu_ctx regs)
{
    *(uint16_t*)(0xb8000 + 79 * 2) = (14 << 8) | (regs.int_no + '0');

    if (regs.int_no < 0x13)
        kprintf("Received interrupt %s\n", cpu_reserved_interrupt_str[regs.int_no]);
    else if (regs.int_no >= 0x13 && regs.int_no < 0x21)
        kprintf("Received reserved interrupt\n");
}
