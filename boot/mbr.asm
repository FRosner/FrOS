[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel

mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
mov bp, 0x9000
mov sp, bp

mov bx, MSG_16BIT_MODE
call print16
call print16_nl

call load_kernel ; read the kernel from disk
call switch_to_32bit ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
jmp $ ; Never executed

%include "boot/print-16bit.asm"
%include "boot/print-32bit.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/switch-to-32bit.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print16
    call print16_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, 31
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_32BIT:
    mov ebx, MSG_32BIT_MODE
    call print32
    call KERNEL_OFFSET ; Give control to the kernel
    jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_16BIT_MODE db "Started in 16-bit Real Mode", 0
MSG_32BIT_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
