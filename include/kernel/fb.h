#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern void kclear_screen(void);
extern void kputs(char *str);
extern void kputc(char c);
extern void kprintf(const char *format, ...);
extern void kerror(const char *format, ...);
extern void fb_init();

#ifdef __cplusplus
}
#endif
