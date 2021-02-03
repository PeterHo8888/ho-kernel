CC = i686-elf-gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector

CXX = i686-elf-g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-use-cxa-atexit -fno-exceptions -fno-rtti

AS = nasm -f elf32

SUBDIRS = src/kernel src/lib

obj = boot.o
kernel_objs = src/kernel/*.o
libc_obj = src/lib/libc.a

.PHONY: all clean
all: kernel.img

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

boot.o: src/boot.s
	$(AS) -o $@ $<

kernel.img: link.ld $(obj)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	$(CXX) -T $< -o $@ -ffreestanding -O2 -nostdlib $(obj) $(kernel_objs) $(libc_obj) -lgcc
	rm -f *.o

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	rm -f *.o kernel.img
