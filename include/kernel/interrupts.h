#pragma once

#include <kernel/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void kb_init();

struct cpu_ctx {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((__packed__));

#ifdef __cplusplus
}
#endif
