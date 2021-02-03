//#include <lib/string.h>
#include <string.h>

/*
   dir = 0 = copy from beginning of src
   dir = 1 = copy from end of src
*/
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
    return internal_memcpy(dest, src, n, 0);
}

void *memmove(void *dest, const void *src, size_t n)
{
    if (dest == src)
        return dest;

    if ((dest - src >= n) || dest < src)
        return internal_memcpy(dest, src, n, 0);

    return internal_memcpy(dest, src, n, 1);
}

char *strcpy(char *dest, const char *src)
{
    while (*src)
        *(dest++) = *(src++);
    *dest = '\0';
    return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    for (size_t i = 0; i < n && *src; ++i)
        dest[i] = src[i];
    *dest = '\0';
    return dest;
}

char *strcat(char *dest, const char *src)
{
    while (*dest++)
        ;
    return strcpy(dest, src);
}

char *strncat(char *dest, const char *src, size_t n)
{
    while (*dest++)
        ;
    return strncpy(dest, src, n);
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        int diff = ((char *)s1)[i] - ((char *)s2)[i];
        if (diff)
            return diff;
    }
    return 0;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        int diff = *s1++ - *s2++;
        if (diff)
            return diff;
    }

    if (*s1)
        return *s1;
    else if (*s2)
        return -*s2;

    return 0;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    // TODO
    return 0;
}

void *memset(void *s, int c, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        ((char *)s)[i] = c;
}
