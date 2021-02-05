#include <kernel/kernel.h>
#include <kernel/idt.h>
#include <kernel/types.h>
#include <string.h>

struct idt_entry {
    uint16_t base_lo; // offset bits 0..15
    uint16_t sel;     // code segment selector in GDT or LDT
    uint8_t zero;     // unused, must be 0
    uint8_t flags;    // type and attributes, see below
    uint16_t base_hi; // offset bits 16..31
} __attribute__((__packed__));

/*
idt_info:
    dw idt_end - idt_start - 1
    dd idt_start
*/
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((__packed__));

extern void write_idt(struct idt_ptr *desc);

#define IDT_SIZE 256
#define IDT_RESERVED_SIZE 0x256
struct idt_entry IDT[IDT_SIZE];
static void (*isr_fn[IDT_RESERVED_SIZE]) = {
    isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9,
    isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19,
    isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29,
    isr30, isr31, isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39,
    isr40, isr41, isr42, isr43, isr44, isr45, isr46, isr47, isr48, isr49,
};

static void encode_idt_entry(struct idt_entry *entry, uint32_t base, uint16_t sel, uint8_t flags)
{
    entry->base_lo = base & 0xffff;
    entry->base_hi = (base >> 16) & 0xffff;

    entry->sel = sel;
    entry->zero = 0;
    entry->flags = flags;
}

void idt_init()
{
    memset(IDT, 0, sizeof(IDT));

    for (size_t i = 0; i < IDT_RESERVED_SIZE; ++i)
        if (isr_fn[i])
            encode_idt_entry(&IDT[i], (uint32_t)isr_fn[i], 0x08, 0x8e);

    /*
     * Ports
     *         PIC1  PIC2
     * Command 0x20  0xA0
     * Data    0x21  0xA1
     */

    /* ICW1 - begin initialization */
    write_port(0x20 , 0x11);
    write_port(0xA0 , 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
     * In x86 protected mode, we have to remap the PICs beyond 0x20 because
     * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
     */
    write_port(0x21 , 0x20);
    write_port(0xA1 , 0x28);

    /* ICW3 - setup cascading */
    write_port(0x21 , 0x00);
    write_port(0xA1 , 0x00);

    /* ICW4 - environment info */
    write_port(0x21 , 0x01);
    write_port(0xA1 , 0x01);
    /* Initialization finished */

    /* mask interrupts */
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);

    /* fill the IDT descriptor */
    //idt_address = (uint32_t)IDT ;
    //idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    //idt_ptr[1] = idt_address >> 16 ;

    struct idt_ptr idt = {
        sizeof(IDT) - 1,
        (uint32_t)&IDT
    };

    write_idt(&idt);
}
