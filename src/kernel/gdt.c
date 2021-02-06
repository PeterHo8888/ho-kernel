#include <kernel/gdt.h>
#include <kernel/fb.h>
#include <stdio.h>

/*
   Access byte:

   7 = P (is present?)
   6-5 = DPL (descriptor priv level; ring 0 - 3)
   4 = DT (descriptor type)
   3-0 = type (segment type; code/data)
 */
/*
   Granularity byte:

   7 = G (granularity; 0 = 1 byte, 1 = 1kbyte)
   6 = D (operand size; 0 = 16bit, 1 = 32bit)
   5 = 0 (always 0)
   4 = A (system use; always 0)
   3-0 = limit:19:16
 */
struct gdt_entry {
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity_limit_hi;
    uint8_t base_hi;
} __attribute__((__packed__));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((__packed__));

/*
   GDT_NULL = 0,
   GDT_CODE = 1,
   GDT_DATA = 2,
   GDT_TSS = 3
 */
extern void write_gdt(struct gdt_ptr *ptr);

static struct gdt_entry GDT[5];
static struct gdt_ptr ptr = {.limit = sizeof(GDT) - 1, .base = (uint32_t)&GDT};

static void encode_gdt_entry(struct gdt_entry *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    entry->base_lo = base & 0xffff;
    entry->base_mid = (base >> 16) & 0xff;
    entry->base_hi = (base >> 24) & 0xff;

    entry->limit_lo = limit & 0xffff;
    entry->granularity_limit_hi = (limit >> 16) & 0xf;

    entry->granularity_limit_hi |= granularity & 0xf0;
    entry->access = access;
}

static uint32_t decode_base(struct gdt_entry *entry)
{
    return entry->base_lo | (entry->base_mid << 16) | (entry->base_hi << 24);
}

static uint32_t decode_limit(struct gdt_entry *entry)
{
    return entry->limit_lo | (entry->granularity_limit_hi & 0xf) << 16;
}

#define SEGMENT_SIZE 0xFFFFFFFF

#define KERNEL_CODE_ACCESS 0x9A // (ring 0)
#define KERNEL_DATA_ACCESS 0x92 // (ring 0)
#define USER_CODE_ACCESS 0xFA   // (ring 3)
#define USER_DATA_ACCESS 0xF2   // (ring 3)
#define GRANULARITY 0xCF        // (1kbyte, 32bit)
void gdt_init()
{
    encode_gdt_entry(&GDT[0], 0, 0, 0, 0);
    encode_gdt_entry(&GDT[1], 0, SEGMENT_SIZE, KERNEL_CODE_ACCESS, GRANULARITY);
    encode_gdt_entry(&GDT[2], 0, SEGMENT_SIZE, KERNEL_DATA_ACCESS, GRANULARITY);
    encode_gdt_entry(&GDT[3], 0, SEGMENT_SIZE, USER_CODE_ACCESS, GRANULARITY);
    encode_gdt_entry(&GDT[4], 0, SEGMENT_SIZE, USER_DATA_ACCESS, GRANULARITY);

    for (size_t i = 0; i < sizeof(GDT)/sizeof(*GDT); ++i)
        kprintf("gdt: entry=%2x, base=%8x limit=%8x\n", sizeof(GDT[0]) * (&GDT[i] - &GDT[0]), decode_base(&GDT[i]), decode_limit(&GDT[i]));

    write_gdt(&ptr);
}
