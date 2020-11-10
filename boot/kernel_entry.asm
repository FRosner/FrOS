global _start;
[bits 32]

_start:
    [extern start_kernel] ; Define calling point. Must have same name as kernel.c 'main' function
    call start_kernel ; Calls the C function. The linker will know where it is placed in memory
    jmp $