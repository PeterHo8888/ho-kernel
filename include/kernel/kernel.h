#pragma once

#include <kernel/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stackframe {
    struct stackframe *ebp;
    uint32_t eip;
};

struct cpu_ctx {
    uint32_t esp_align, junk0, junk1;
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((__packed__));

extern void write_port(unsigned short port, unsigned char data);
extern char read_port(unsigned short port);
extern void kdump_trace();
extern void kdump_regs(struct cpu_ctx ctx);
extern void debug_isr(struct cpu_ctx ctx);

#ifdef __cplusplus
}
#endif
