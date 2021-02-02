#include "string.h"

static void *internal_memcpy(void *dest, const void *src, size_t n, int dir)
{
    if (dir == 0) {
        for (size_t i = 0; i < n; ++i)
            ((char *)dest)[i] = ((char *)src)[i];
    } else {
        for (size_t i = n - 1; i >= 0; --i)
            ((char *)dest)[i] = ((char *)src)[i];
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    return internal_memcpy(dest, src, n);
}

void *memmove(void *dest, const void *src, size_t n)
{
    if (dest == src)
        return dest;

    if ((dest - src >= n) || dest < src)
        return internal_memcpy(dest, src, n, 0);

    return internal_memcpy(dest, src, n, 1);
}

void *strcpy(char *dest, const char *src)
{
    while (*src)
        *(dest++) = *(src++);
    *dest = '\0';
    return dest;
}

void *strncpy(char *dest, const char *src, size_t n)
{
    for (size_t i = 0; i < n && *src; ++i)
        dest[i] = src[i];
    *dest = '\0';
    return dest;
}

void *strcat(char *dest, const char *src)
{
    while (*dest++)
        ;
    return strcpy(dest, src);
}

void *strncat(char *dest, const char *src, size_t n)
{
    while (*dest++)
        ;
    return strncpy(dest, src, n);
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        int diff = *(s1++) - *(s2++);
        if (diff)
            return diff;
    }
    return 0;
}

int strcmp(const void *s1, const void *s2)
{
    while (*s1 && *s2) {
        int diff = *(s1++) - *(s2++);
        if (diff)
            return diff;
    }

    if (*s1)
        return *s1;
    else if (*s2)
        return -*s2;

    return 0;
}

int strncmp(const void *s1, const void *s2, size_t n)
{
    // TODO
    return 0;
}
