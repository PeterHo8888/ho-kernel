CC := gcc -m32 -fno-stack-protector -I ../../include

obj = string.o

%.o: %.c
	$(CC) -c -o $@ $<

libc.a: $(obj)
	ar rcs libc.a $(obj)
