#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <kernel/fb.h>
#include <kernel/types.h>

// https://gitlab.com/sortie/meaty-skeleton/-/blob/master/libc/stdio/printf.c
static int print(const char *data, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        if (putchar(data[i]) == EOF)
            return false;
    return true;
}

int printf(const char *format, ...)
{
    va_list list;
    va_start(list, format);

    int bytes_written = 0;

    while (*format) {
        // Check for % sign and anything else
        if (format[0] != '%' || format[1] == '%') {
            // %%
            if (*format == '%')
                ++format;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                ++amount;
            // Found a % not attached to prior %
            if (!print(format, amount))
                return -1;
            format += amount;
            bytes_written += amount;
            continue;
        }

        // We have a %
        switch (*++format) {
        case 'c':
            ++format;
            const char c = (char) va_arg(list, int);
            if (!print(&c, sizeof(c)))
                return -1;
            ++bytes_written;
            break;
        case 's':
            ++format;
            const char *s = va_arg(list, const char *);
            size_t len = strlen(s);
            if (!print(s, len))
                return -1;
            ++bytes_written;
            break;
        }
    }
    va_end(list);
    return bytes_written;
}

int putchar(int c)
{
    kputc(c);
    return c;
}

int puts(const char *s)
{
    return printf("%s\n", s);
}
