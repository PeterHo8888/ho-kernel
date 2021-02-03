CC := gcc -m32 -fno-stack-protector
AS := nasm -f elf32
SUBDIRS = src/kernel src/lib

obj = boot.o
kernel_objs = src/kernel/fb.o src/kernel/interrupts.o src/kernel/io.o src/kernel/kernel.o src/kernel/sys.o
libc_obj = src/lib/libc.a

.PHONY: all clean
all: kernel.img

%.o: %.c
	$(CC) -c -o $@ $<

boot.o: src/boot.s
	$(AS) -o $@ $<

kernel.img: link.ld $(obj)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	ld -m elf_i386 -T $< -o $@ $(obj) $(kernel_objs) $(libc_obj)
	rm -f *.o

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	rm -f *.o kernel.img
