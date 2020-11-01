# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Notice how dependencies are built as needed
kernel.bin: kernel_entry.o kernel.o
	x86_64-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel_entry.o: boot/kernel_entry.asm
	nasm $< -f elf -o $@

kernel.o: kernel/kernel.c
	x86_64-elf-gcc -m32 -ffreestanding -c $< -o $@

# Disassemble
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

echo: os-image.bin
	xxd $<

clean:
	$(RM) *.bin *.o *.dis
