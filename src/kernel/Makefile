INC_DIR := "../../include"
CC := gcc -m32 -fno-stack-protector -I $(INC_DIR)
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
