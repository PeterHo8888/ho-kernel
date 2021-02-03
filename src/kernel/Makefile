INCLUDES := -I "../../include" -I "../../libc"
CC := gcc -m32 -fno-stack-protector $(INCLUDES) -nostdlib -nodefaultlibs -nolibc -ffreestanding -Wall -Wextra
AS := nasm -f elf32

obj = kernel.o io.o interrupts.o sys.o fb.o

.PHONY: all clean

all: $(obj)

%.o: %.c $(INC_DIR)/kernel/%.h
	$(CC) -c -o $@ $<

sys.o: sys.s
	$(AS) -o sys.o $<

clean:
	rm -f *.o
