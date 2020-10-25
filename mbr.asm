mov ah, 0x0e ; tty mode http://vitaly_filatov.tripod.com/ng/asm/asm_023.15.html
mov al, 'H'
int 0x10 ; https://en.wikipedia.org/wiki/INT_10H
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
int 0x10 ; 'l' is still on al
mov al, 'o'
int 0x10

jmp $ ; jump to current address = infinite loop

; padding and magic number
times 510 - ($-$$) db 0
dw 0xaa55 
