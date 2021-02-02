#include <kernel/kernel.h>
#include <kernel/fb.h>
#include <kernel/types.h>

#define FB_WIDTH 80
#define FB_HEIGHT 25
#define vga_fmt(c, f, b) (((uint16_t)(b << 4 | f) << 8) | (uint16_t)c)

#define VGA_ADDRESS 0xB8000   /* video memory begins here */

#define FG_COLOR YELLOW
#define BG_COLOR BLACK

/* VGA provides support for 16 colors */
enum VGA_COLOR {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    ORANGE = 6,
    LGRAY = 7,
    DGRAY = 8,
    LBLUE = 9,
    LGREEN = 10,
    LCYAN = 11,
    LRED = 12,
    LMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15,
};

uint16_t *terminal_buffer;
uint32_t vga_index;

void fb_init()
{
    terminal_buffer = (uint16_t *)VGA_ADDRESS;
    vga_index = 0;
}

static void update_cursor()
{
    write_port(0x3d4, 0x0f);
    write_port(0x3d5, (uint8_t)(vga_index & 0xff));
    write_port(0x3d4, 0x0e);
    write_port(0x3d5, (uint8_t)((vga_index >> 8) & 0xff));
}

void kclear_screen()
{
    for (vga_index = 0; vga_index < FB_WIDTH * FB_HEIGHT; ++vga_index)
        terminal_buffer[vga_index] = vga_fmt(' ', FG_COLOR, BG_COLOR);
    vga_index = 0;
    update_cursor();
}

void kputc(char c)
{
    switch (c) {
    case '\n':
        vga_index += FB_WIDTH - vga_index % FB_WIDTH;
        break;
    case '\b':
        terminal_buffer[--vga_index] = vga_fmt(' ', FG_COLOR, BG_COLOR);
        break;
    case '\t':
        vga_index += 4;
        break;
    default:
        terminal_buffer[vga_index++] = vga_fmt(c, FG_COLOR, BG_COLOR);
        break;
    }
    update_cursor();
}

void kputs(char *str)
{
    while (*str) {
        kputc(*str++);
    }
}
