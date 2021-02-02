CC := gcc -m32 -fno-stack-protector -I src/include
AS := nasm -f elf32
SUBDIRS = src/kernel

obj = boot.o
kernel_objs = src/kernel/fb.o src/kernel/interrupts.o src/kernel/io.o src/kernel/kernel.o src/kernel/sys.o

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
	ld -m elf_i386 -T $< -o $@ $(obj) $(kernel_objs)
	rm -f *.o

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	rm -f *.o kernel.img
