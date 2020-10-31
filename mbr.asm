[org 0x7c00] ; tell the assembler that our offset is bootsector code

; The main routine makes sure the parameters are ready and then calls the function
mov bx, HELLO
call print
call print_nl

mov bp, 0x8000 ; set the stack safely away from us
mov sp, bp

mov bx, 0x9000 ; es:bx = 0x0000:0x9000 = 0x09000
mov dh, 2 ; read 2 sectors
; the bios sets 'dl' for our boot disk number
; if you have trouble, use the '-fda' flag: 'qemu -fda file.bin'
call disk_load

mov dx, [0x9000] ; retrieve the first loaded word, 0xcafe
call print_hex
call print_nl

mov dx, [0x9000 + 512] ; first word from second loaded sector, 0xface
call print_hex
call print_nl

mov bx, GOODBYE
call print
call print_nl

; that's it! we can hang now
jmp $

; remember to include subroutines below the hang
%include "print.asm"
%include "disk.asm"

; data
HELLO:
    db 'Hello, World', 0

GOODBYE:
    db 'Goodbye', 0

; padding and magic number
times 510-($-$$) db 0
dw 0xaa55

; boot sector = sector 1 of cyl 0 of head 0 of hdd 0
; from now on = sector 2 ...
times 256 dw 0xcafe ; sector 2 = 512 bytes
times 256 dw 0xface ; sector 3 = 512 bytes
