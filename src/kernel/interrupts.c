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
    "DIV_BY_0",                     // 0
    "DEBUG",                        // 1
    "NON_MASKABLE_INT",             // 2
    "BREAKPOINT",                   // 3
    "INTO_DETECT_OVERFLOW",         // 4
    "OUT_OF_BOUNDS",                // 5
    "INVALID_OPCODE",               // 6
    "NO_COPROCESSOR",               // 7
    "DOUBLE_FAULT",                 // 8
    "COPROCESSOR_SEGMENT_OVERRUN",  // 9
    "BAD_TSS",                      // A
    "SEGMENT_NOT_PRESENT",          // B
    "STACK_FAULT",                  // C
    "GENERAL_PROTECTION_FAULT",     // D
    "PAGE_FAULT",                   // E
    "UNKNOWN_INTERRUPT",            // F
    "COPROCESSOR_FAULT",            // 10
    "ALIGNMENT_CHECK",              // 11
    "MACHINE_CHECK"                 // 12
};

void isr_handler(struct cpu_ctx ctx)
{
    *(uint16_t*)(0xb8000 + 79 * 2) = (14 << 8) | (ctx.int_no + '0');

    switch (ctx.int_no) {
    case 0x21:
        keyboard_isr();
        break;
    default:
        kprintf("Unhandled interrupt %d: %s\n",
                ctx.int_no,
                (ctx.int_no < 0x13) ? cpu_reserved_interrupt_str[ctx.int_no] : "undefined"
               );
        break;
    }
}
