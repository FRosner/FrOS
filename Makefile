build:
	nasm -f bin mbr.asm -o out/mbr.bin

run: build
	qemu-system-x86_64 out/mbr.bin

echo: build
	xxd out/mbr.bin

c:
	gcc -ffreestanding -c basic.c -o out/basic.o
	objdump -d out/basic.o
