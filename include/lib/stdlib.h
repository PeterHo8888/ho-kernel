#pragma once

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__notreturn__)) void abort();

char *itoa(int val, char *s);
int abs(int j);

#ifdef __cplusplus
}
#endif
