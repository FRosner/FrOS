[org 0x7c00] ; bootloader offset
    mov bp, 0x9000 ; set the stack
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print16 ; This will be written after the BIOS messages

    call print16_cls

    call switch_to_pm
    jmp $

%include "print-16bit.asm"
%include "print-32bit.asm"
%include "disk.asm"
%include "gdt.asm"
%include "switch-to-32bit.asm"

[bits 32]
BEGIN_PM: ; after the switch we will get here
    mov ebx, MSG_PROT_MODE
    call print32 ; Note that this will be written at the top left corner
    jmp $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

; bootsector
times 510-($-$$) db 0
dw 0xaa55
