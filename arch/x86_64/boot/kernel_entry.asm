; ORIGIN OS — x86-64 kernel entry point.
;
; Establishes the kernel stack, initializes the GDT/TSS,
; and transfers control to the C kernel entry point.

bits 64
default rel

; ---------------------------------------------------------------------------
; Kernel stack
; ---------------------------------------------------------------------------

section .bss

align 16
stack_bottom:
    resb 16384                 ; Reserve a 16 KiB kernel bootstrap stack.
stack_top:

; ---------------------------------------------------------------------------
; Kernel entry
; ---------------------------------------------------------------------------

section .text

global kernel_entry
extern gdt_init
extern kernel_main

kernel_entry:
    cli                        ; Disable maskable interrupts during setup.

    ; Establish a known stack and frame-pointer state.
    lea rsp, [stack_top]
    xor rbp, rbp

    ; Preserve the bootloader-provided BootInfo pointer.
    ; RDI is the first argument register in the x86-64 System V ABI.
    ;
    ; RSP is 16-byte aligned here. Subtracting 16 reserves an aligned
    ; stack slot without changing RSP's alignment before either call.
    sub rsp, 16
    mov [rsp], rdi

    ; Initialize the GDT and TSS before entering C kernel code.
    call gdt_init

    ; Restore the BootInfo pointer as kernel_main's first argument.
    mov rdi, [rsp]
    add rsp, 16

    ; Enter the main kernel initialization routine.
    call kernel_main

.hang:
    ; kernel_main should not return. Halt safely if it does.
    cli
    hlt
    jmp .hang