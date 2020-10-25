build:
	nasm -f bin boot_sect_simple.asm -o out/boot_sect_simple.bin

run: build
	qemu-system-x86_64 out/boot_sect_simple.bin
