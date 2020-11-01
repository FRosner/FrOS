run: build-bootsector build-kernel
	qemu-system-x86_64 out/mbr.bin

build-bootsector:
	nasm -f bin mbr.asm -o out/mbr.bin

echo-bootsector: build-bootsector
	xxd out/mbr.bin

build-kernel:
	x86_64-elf-gcc -ffreestanding -c kernel.c -o out/kernel.o
	x86_64-elf-ld -o out/kernel.bin -Ttext 0x0 --oformat binary out/kernel.o

echo-kernel: build-kernel
	xxd out/kernel.bin

decompile-kernel: build-kernel
	ndisasm -b 32 out/kernel.bin
