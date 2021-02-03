INCLUDE = -I "../../include" -I "../../include/lib"
CC = i686-elf-gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector $(INCLUDE)

obj = string.o stdlib.o stdio.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

libc.a: $(obj)
	ar rcs libc.a $(obj)

.PHONY: clean
clean:
	rm -f *.o kernel.img
