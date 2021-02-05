#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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
    int result;
    va_list args;
    va_start(args, format);
    result = vprintf(format, args);
    va_end(args);
    return result;
}

int vprintf(const char *format, va_list args)
{
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
        size_t pad = 0;
        while (*++format >= '0' && *format <= '9')
            pad = pad * 10 + *format - '0';

        size_t len;
        switch (*format) {
        case 'c':
            const char c = (char) va_arg(args, int);
            if (!print(&c, sizeof(c)))
                return -1;
            ++format;
            ++bytes_written;
            break;
        case 'd':
        case 'x':
            const int d = va_arg(args, int);
            char buf[80];
            itoa(d, buf, (*format == 'd') ? 10 : 16);
            len = strlen(buf);
            if (pad > len) {
                memmove(buf + pad - len, buf, len + 1);
                memset(buf, '0', pad - len);
                len = pad;
            }
            if (!print(buf, len))
                return -1;
            ++format;
            bytes_written += len;
            break;
        case 's':
            const char *s = va_arg(args, const char *);
            len = strlen(s);
            if (!print(s, len))
                return -1;
            ++format;
            bytes_written += len;
            break;
        }
    }
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
