#pragma once

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__)) void abort();

char *itoa(int val, char *s, int base);
int abs(int j);

#ifdef __cplusplus
}
#endif
