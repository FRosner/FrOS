print16:
    pusha

; strings will be terminated by 0 byte in memory
print16_loop:
    mov al, [bx] ; 'bx' is the base address for the string
    cmp al, 0
    je print16_done

    mov ah, 0x0e ; tty
    int 0x10 ; 'al' already contains the char

    ; increment pointer and do next loop
    add bx, 1
    jmp print16_loop

print16_done:
    popa
    ret

print16_nl:
    pusha

    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10

    popa
    ret

print16_cls:
    pusha

    mov ah, 0x00
    mov al, 0x03  ; text mode 80x25 16 colours
    int 0x10

    popa
    ret

; receiving the data in 'dx'
; For the examples we'll assume that we're called with dx=0x1234
print16_hex:
    pusha

    mov cx, 0 ; our index variable

; Strategy: get the last char of 'dx', then convert to ASCII
; Numeric ASCII values: '0' (ASCII 0x30) to '9' (0x39), so just add 0x30 to byte N.
; For alphabetic characters A-F: 'A' (ASCII 0x41) to 'F' (0x46) we'll add 0x40
; Then, move the ASCII byte to the correct position on the resulting string
print16_hex_loop:
    cmp cx, 4 ; loop 4 times
    je print16_hex_end

    ; 1. convert last char of 'dx' to ascii
    mov ax, dx ; we will use 'ax' as our working register
    and ax, 0x000f ; 0x1234 -> 0x0004 by masking first three to zeros
    add al, 0x30 ; add 0x30 to N to convert it to ASCII "N"
    cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
    jle print16_hex_step2
    add al, 7 ; 'A' is ASCII 65 instead of 58, so 65-58=7

print16_hex_step2:
    ; 2. get the correct position of the string to place our ASCII char
    ; bx <- base address + string length - index of char
    mov bx, PRINT16_HEX_OUT + 5 ; base + length
    sub bx, cx  ; our index variable
    mov [bx], al ; copy the ASCII char on 'al' to the position pointed by 'bx'
    ror dx, 4 ; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234

    ; increment index and loop
    add cx, 1
    jmp print16_hex_loop

print16_hex_end:
    ; prepare the parameter and call the function
    ; remember that print receives parameters in 'bx'
    mov bx, PRINT16_HEX_OUT
    call print16

    popa
    ret

PRINT16_HEX_OUT:
    db '0x0000',0 ; reserve memory for our new string
