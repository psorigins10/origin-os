bits 64

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text

global kernel_entry
extern kernel_main

kernel_entry:
    cli
    lea rsp, [rel stack_top]
    xor rbp, rbp
    call kernel_main 

.hang:
    cli
    hlt
    jmp .hang