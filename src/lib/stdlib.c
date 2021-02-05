#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <kernel/fb.h>

__attribute__((__noreturn__)) void abort()
{
    kfatal("abort()\n");
    while (1)
        ;
    __builtin_unreachable();
}

static void reverse(char *s)
{
    int i, j;
    for (i = 0, j = strlen(s) - 1; i < j; ++i, --j) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char *itoa(int val, char *s, int base)
{
    static const char lookup[16] = {
        '0', '1', '2', '3', '4', '5',
        '6', '7', '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F'
    };
    int i = 0;
    int sign = val;

    do {
        int digit = abs(val % base);
        s[i++] = lookup[digit];
        val /= base;
    } while (val);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return s;
}

int abs(int j)
{
    // sign extend for XOR + sign bit
    // flips bits and adds 1 when negative
    return (j ^ (j >> 31)) + ((unsigned int)j >> 31);
}
