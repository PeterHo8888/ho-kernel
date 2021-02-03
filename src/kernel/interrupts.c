#include <kernel/kernel.h>
#include <kernel/interrupts.h>
#include <kernel/types.h>
#include <string.h>


struct IDTDesc {
    uint16_t offset_1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t zero;      // unused, set to 0
    uint8_t type_attr; // type and attributes, see below
    uint16_t offset_2; // offset bits 16..31
} __attribute__((__packed__));

/*
idt_info:
    dw idt_end - idt_start - 1
    dd idt_start
*/
struct IDTInfo {
    uint16_t size;
    uint32_t addr;
} __attribute__((__packed__));

#define IDT_SIZE 256
struct IDTDesc IDT[IDT_SIZE];

extern void load_idt(struct IDTInfo *desc);
extern void keyboard_handler();

void idt_init()
{
    uint32_t keyboard_address;

    memset(IDT, '\0', sizeof(IDT));

    keyboard_address = (uint32_t)keyboard_handler;
    IDT[0x21].offset_1 = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[0x21].offset_2 = keyboard_address >> 16;

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

    struct IDTInfo idt_desc = {
        sizeof(IDT),
        (uint32_t)IDT
    };

    load_idt(&idt_desc);
}

