
bits 64
section .text

global divide_error_handler
global double_fault_handler
global general_protection_handler
global page_fault_handler

extern exception_dispatcher

; #DE: CPU does not push an error code.
divide_error_handler:
    push 0                  ; Synthetic error code
    push 0                  ; Vector 0
    jmp exception_common

; #DF: CPU already pushed its error code.
; Keep this path isolated for now.
double_fault_handler:
    push 8                  ; Vector 8
    cli
.df_halt:
    hlt
    jmp .df_halt

; Shared entry path for exceptions that use the
; normalized frame and the C dispatcher.
exception_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; RSP now points to the ORIGIN_EXCEPTION_FRAME.
    ; R12 is saved in the frame, and the C ABI preserves it.
    mov r12, rsp
    mov rdi, r12

    ; Clear direction flag before entering C.
    cld

    ; Align stack for the x86-64 SysV calling convention.
    and rsp, -16
    call exception_dispatcher

    ; If the dispatcher returns, restore the original frame.
    mov rsp, r12

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16              ; Remove vector and error code
    iretq

; #GP: CPU already pushed error code
general_protection_handler:
    push 13
    jmp exception_common


page_fault_handler:
    push 14
    jmp exception_common
