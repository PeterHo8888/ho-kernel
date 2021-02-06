INCLUDES = -I "../../include" -I "../../include/lib"

CC = i686-elf-gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector $(INCLUDES)

CXX = i686-elf-g++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-use-cxa-atexit -fno-exceptions -fno-rtti $(INCLUDES)

AS = nasm -f elf32

obj = kernel.o interrupts.o sys.o fb.o gdt.o idt.o isr.o

.PHONY: all clean

all: $(obj)

%.o: %.c $(INC_DIR)/kernel/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cc $(INC_DIR)/kernel/%.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sys.o: sys.s
	$(AS) -o $@ $<

isr.o: isr.s
	$(AS) -o $@ $<

clean:
	rm -f *.o
