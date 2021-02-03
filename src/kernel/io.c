#include <kernel/kernel.h>
#include <kernel/io.h>
#include <kernel/fb.h>
#include <kernel/keyboard.h>
#include <kernel/types.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
void keyboard_handler_main()
{
    uint8_t status;
    uint8_t keycode;

    status = read_port(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if(keycode < 0)
            return;

        // Change this to fill a buffer
        uint8_t ascii = keycode_to_ascii(keycode);
        if (ascii)
            kputc(ascii);
    }

    /* write EOI */
    write_port(0x20, 0x20);
}

void kb_init()
{
    write_port(0x21, 0xfd);
}
