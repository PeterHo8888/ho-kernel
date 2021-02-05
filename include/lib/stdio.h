#pragma once

#include <stdarg.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char *format, ...);
int putchar(int c);
int puts(const char *s);

int vprintf(const char *format, va_list args);

#ifdef __cplusplus
}
#endif
